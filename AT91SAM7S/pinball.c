#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/autostart.h>
#include <sys/etimer.h>
#include <usb-api.h>
#include <usb-core.h>
#include <hid-descriptors-consts.h>
#include <ws2812.h>

#define NUM_WS2812 64
static struct WS2812BitBuffer ws_buffer[NUM_WS2812];

PROCESS(usb_pinball_process, "USB process");

static uint8_t recv_data[128];
USBBuffer recv_buffer;


USBBuffer xmit_buffer[4];

#define SERIAL_IN 0x81
#define SERIAL_OUT 0x02
#define INTERRUPT_IN 0x83

static void
submit_recv(void)
{
  if (!(recv_buffer.flags & USB_BUFFER_SUBMITTED)) {
    recv_buffer.next = NULL;
    recv_buffer.data = recv_data;
    recv_buffer.left = sizeof(recv_data);
    recv_buffer.flags = USB_BUFFER_NOTIFY | USB_BUFFER_PACKET_END;
    recv_buffer.id = 0;
    usb_submit_recv_buffer(SERIAL_OUT, &recv_buffer);
    /* printf("submit_recv: %d\n",i); */
  }
}

#define GET_DESCRIPTOR 0x06
#define GET_REPORT 0x01
#define GET_IDLE 0x02
#define GET_PROTOCOL 0x03
#define SET_REPORT 0x09
#define SET_IDLE 0x0a
#define SET_PROTOCOL 0x0b


#define SIZE_X 64
#define SIZE_Y 1

struct DisplayReport
{
  uint8_t id;
  uint8_t sizeX;
  uint8_t sizeY;
  uint8_t depth;
};

struct DisplayReport display_report =
  {
    1,
    SIZE_X,SIZE_Y,
    0x30 /* RGB 888 */
  };

static uint8_t blit_buffer[256];

struct BlitReport
{
  uint8_t id;
  uint8_t x1;
  uint8_t y1;
  uint8_t x2;
  uint8_t y2;
  struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
  } pixels[1];
};

static void
handle_blit(uint8_t *data, unsigned int length)
{
  int x;
  int y;
  struct BlitReport *blit;
  uint8_t *p;
  if (length < sizeof(struct BlitReport)) {
    usb_error_stall();
    return;
  }
  blit = (struct BlitReport*)data;
  printf("Blit: (%d,%d) - (%d,%d)\n",blit->x1,blit->y1,blit->x2,blit->y2);
  if (sizeof(struct BlitReport) 
      + (((blit->x2-blit->x1) * (blit->y2-blit->y1) - 1) 
	 * sizeof(blit->pixels[0])) > length) {
    printf("Not enough blit data\n");
    usb_error_stall();
    return;
  }
  if (blit->x2 > SIZE_X || blit->x1 > blit->x2
      || blit->y2 > SIZE_Y || blit->y1 > blit->y2) {
    printf("Illegal rect\n");
    usb_error_stall();
    return;
  }
  p = (uint8_t*)&blit->pixels;
  for(y = blit->y1; y < blit->y2; y++) {
    for(x = blit->x1; x < blit->x2; x++) {
      ws2812_encode_rgb(ws_buffer + x + y * SIZE_X, p[0], p[1], p[2]);
      p += 3;
    }
  }
  usb_send_ctrl_status();
}

static unsigned int
handle_hid_requests()
{
  printf("HID request %02x %02x\n", usb_setup_buffer.bmRequestType, usb_setup_buffer.bRequest);
  switch(usb_setup_buffer.bmRequestType) {
  case 0x81:
    switch(usb_setup_buffer.bRequest) {
    case GET_DESCRIPTOR:
      if (usb_setup_buffer.wValue == 0x2200 
	  && usb_setup_buffer.wLength >= sizeof(descriptor_wrapper.hid_report_descriptor)) {
	usb_send_ctrl_response((const uint8_t*)&descriptor_wrapper.hid_report_descriptor,
			       sizeof(descriptor_wrapper.hid_report_descriptor));
	return 1;
      }
      return 0;
    }
    break;
  case 0x21:
    switch(usb_setup_buffer.bRequest) {
    case SET_IDLE:
      if (usb_setup_buffer.wLength == 0) {
	printf("Dur: %d, id: %d\n", 
	       usb_setup_buffer.wLength>>8,
	       usb_setup_buffer.wLength & 0xff);
	usb_send_ctrl_status();
	return 1;
      }
      break;
    case SET_REPORT:
      {
	unsigned int len = usb_setup_buffer.wLength;
	if (len > sizeof(blit_buffer))
          len = sizeof(blit_buffer);
        usb_get_ctrl_data(blit_buffer, len,
                          handle_blit);
	printf("Got blit report: %d\n",len);
	return 1;
      }
      break;
    }
    break;
  case 0xa1:
    switch(usb_setup_buffer.bRequest) {
    case GET_REPORT:
      printf("GET_REPORT\n");
      if (usb_setup_buffer.wValue == 0x0301 
	  && usb_setup_buffer.wLength >= sizeof(display_report)) {
	usb_send_ctrl_response((const uint8_t*)&display_report,
			       sizeof(display_report));
	return 1;
      }
      return 0;
    }
    break;
  }
  return 0;
}

static const struct USBRequestHandler hid_request_handler =
  {
    0x81, 0x0f,
    0x00, 0x00,
    handle_hid_requests
  };

static struct USBRequestHandlerHook hid_request_hook =
  {
    NULL,
    &hid_request_handler
  };

void
usb_hid_setup()
{
  usb_register_request_handler(&hid_request_hook);
}

PROCESS_THREAD(usb_pinball_process, ev , data)
{
  static struct etimer timer;
  PROCESS_BEGIN();
  
  usb_setup();
  usb_hid_setup();
  printf("usb_setup done\n");
  usb_set_global_event_process(process_current);
  etimer_set(&timer,1*CLOCK_SECOND);
  
  while(1) {
    PROCESS_WAIT_EVENT();
    if (ev == PROCESS_EVENT_EXIT) break;
    if (ev == PROCESS_EVENT_POLL) {
      unsigned int events = usb_get_global_events();
      if (events) {
	if (events & USB_EVENT_RESET) {
	  printf("USB_EVENT_RESET\n");
	}
	if (events & USB_EVENT_CONFIG) {
	  printf("USB_EVENT_CONFIG %d\n",usb_get_current_configuration());
	  if (usb_get_current_configuration() == 1) {
	    usb_setup_bulk_endpoint(SERIAL_OUT);
	    usb_setup_bulk_endpoint(SERIAL_IN);
	    usb_setup_interrupt_endpoint(INTERRUPT_IN);
	    usb_set_ep_event_process(SERIAL_OUT, process_current);
	    usb_set_ep_event_process(SERIAL_IN, process_current);
	    usb_set_ep_event_process(INTERRUPT_IN, process_current);
	    submit_recv(); 
	  } else {
	    usb_disable_endpoint(SERIAL_OUT);
	    usb_disable_endpoint(SERIAL_IN);
	    usb_disable_endpoint(INTERRUPT_IN);
	  }
	}
      }
      events = usb_get_ep_events(SERIAL_OUT);
      if (events) {
	if (events & USB_EP_EVENT_NOTIFICATION) {
	  if (!(recv_buffer.flags & USB_BUFFER_SUBMITTED)) {
	  }
	}
      }
      events = usb_get_ep_events(SERIAL_IN);
      if (events) {
	if (events & USB_EP_EVENT_NOTIFICATION) {
	}
      }
      events = usb_get_ep_events(INTERRUPT_IN);
      if (events) {
	if (events & USB_EP_EVENT_NOTIFICATION) {
	}
      }
    }
    if (ev == PROCESS_EVENT_TIMER) {
      USBBuffer *buf;
      buf = &xmit_buffer[0];
      if (!(buf->flags & USB_BUFFER_SUBMITTED)) {
	static const uint8_t report[3] = {0x03, 0x10,0x10};
	buf->next = NULL;
	buf->data = (uint8_t*)report;
	buf->left = 4;
	buf->flags = 0;
	buf->id = 5;
	usb_submit_xmit_buffer(INTERRUPT_IN, buf);
	printf("+");
      } else {
	printf("-");
      }
      etimer_reset(&timer);
    }
  }
  PROCESS_END();
}

#define EXT_CLK_FREQ 4000000
#define PWM_TOP (MCK/(EXT_CLK_FREQ*2))
static void
setup_ext_clock(void)
{
}

static void
clear_ws_buffer(struct WS2812BitBuffer *bits, unsigned int len)
{
  unsigned int i;
  for(i = 0; i < len; i++) {
    ws2812_encode_rgb(&bits[i], 0,0,0);
  }
}

PROCESS(pinball_process, "Pinball controller process");

static volatile int dummy;


PROCESS_THREAD(pinball_process, ev , data)
{
  static uint8_t color = 0;
  static struct etimer timer;
  PROCESS_BEGIN();
  setup_ext_clock();

  clear_ws_buffer(ws_buffer, NUM_WS2812);
  ws2812_init();

  etimer_set(&timer,CLOCK_SECOND/20);
  process_start(&usb_pinball_process, NULL);
  while(1) {
    PROCESS_WAIT_EVENT();
    if (ev == PROCESS_EVENT_TIMER) {
      etimer_reset(&timer);
      color = (color+1) % 256;
      ws2812_start(ws_buffer,NUM_WS2812);
    } else if (ev == PROCESS_EVENT_POLL) {
      printf("Poll\n");
    }
  }
  
  PROCESS_END();
}


AUTOSTART_PROCESSES(&pinball_process);
