#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/autostart.h>
#include <sys/etimer.h>
#include <usb-api.h>
#include <usb-core.h>
#include <hid-descriptors-consts.h>

PROCESS(usb_isp_process, "USB process");

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

static uint8_t reply_head[5];
static uint8_t reply_tail[1];

#define BUFFER_ID_HEAD 1
#define BUFFER_ID_MSG 2
#define BUFFER_ID_TAIL 3

static uint8_t
calc_chksum(const uint8_t *data, unsigned int len)
{
  uint8_t chksum = 0;
  while(len-- > 0) {
    chksum ^= *data++;
  }
  return chksum;
}

static uint8_t reply_chksum;

static void
send_head(uint8_t seq_no, unsigned int len)
{
  USBBuffer *buf = &xmit_buffer[0];
  reply_head[0] = 77;
  reply_head[1] = seq_no;
  reply_head[2] = len >> 8;
  reply_head[3] = len;
  reply_head[4] = 0;
  reply_chksum = calc_chksum(reply_head, sizeof(reply_head));
  buf->next = NULL;
  buf->data = reply_head;
  buf->left = sizeof(reply_head);
  buf->flags = 0;
  buf->id = BUFFER_ID_HEAD;
  usb_submit_xmit_buffer(SERIAL_IN, buf);
}

static void
send_tail()
{
  USBBuffer *buf = &xmit_buffer[3];
  reply_tail[0] = reply_chksum;
  buf->next = NULL;
  buf->data = reply_tail;
  buf->left = sizeof(reply_tail);
  buf->flags = USB_BUFFER_NOTIFY;
  buf->id = BUFFER_ID_TAIL;
  usb_submit_xmit_buffer(SERIAL_IN, buf);
}

static void
send_data(const uint8_t *data, unsigned int len)
{
  static uint8_t current_buffer = 0;
  USBBuffer *buf;
  buf = &xmit_buffer[current_buffer + 1];
  while(buf->flags & USB_BUFFER_SUBMITTED);
  reply_chksum ^= calc_chksum(data, len);
  buf->next = NULL;
  buf->data = (uint8_t*)data;
  buf->left = len;
  buf->flags = 0;
  buf->id = BUFFER_ID_MSG;
  usb_submit_xmit_buffer(SERIAL_IN, buf);
  current_buffer ^= 1;
}
#if 0
static void
send_reply(uint8_t seq_no, const uint8_t *data, unsigned int len)
{
  if (xmit_buffer[3].flags & USB_BUFFER_SUBMITTED) {
    printf("overrun\n");
    return;
  }
  send_head(seq_no, len);
  send_data(data, len);
  send_tail();
}
#endif
#define MAX_MSG_SIZE 275

static struct pt input_pt;
static uint8_t *input_data;
static uint8_t *input_end;

#define WAIT_CHAR() PT_WAIT_UNTIL(&input_pt,input_end > input_data)

static const uint8_t reply_chksum_err[] =
  {42, 42};


PT_THREAD(handle_input())
{
  PT_BEGIN(&input_pt);
  while(1) {
    WAIT_CHAR();
    printf("%02x\n", *input_data++);
  }
  PT_END(&input_pt);
}

#define GET_DESCRIPTOR 0x06

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
  }
  return 0;
}

static const struct USBRequestHandler hid_request_handler =
  {
    0x81, 0xff,
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

PROCESS_THREAD(usb_isp_process, ev , data)
{
  PROCESS_BEGIN();
  
  usb_setup();
  usb_hid_setup();
  printf("usb_setup done\n");
  usb_set_global_event_process(process_current);

  PT_INIT(&input_pt);
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
	    submit_recv(); 
	  } else {
	    usb_disable_endpoint(SERIAL_OUT);
	    usb_disable_endpoint(SERIAL_IN);
	  }
	}
      }
      events = usb_get_ep_events(SERIAL_OUT);
      if (events) {
	if (events & USB_EP_EVENT_NOTIFICATION) {
	  if (!(recv_buffer.flags & USB_BUFFER_SUBMITTED)) {
	    unsigned int len = sizeof(recv_data) - recv_buffer.left;
	    input_end = &recv_data[len];
	    input_data = recv_data;
	    (void)PT_SCHEDULE(handle_input());
	    submit_recv();
	  }
	}
      }
      events = usb_get_ep_events(SERIAL_IN);
      if (events) {
	if (events & USB_EP_EVENT_NOTIFICATION) {
	}
      }
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
     
PROCESS(avrisp_process, "AVR ISP process");

static volatile int dummy;


PROCESS_THREAD(avrisp_process, ev , data)
{
  static struct etimer timer;
  PROCESS_BEGIN();
  setup_ext_clock();
  etimer_set(&timer,CLOCK_SECOND);
  process_start(&usb_isp_process, NULL);
  while(1) {
    PROCESS_WAIT_EVENT();
    if (ev == PROCESS_EVENT_TIMER) {
    /*   printf("PB: %04x %04x %04x\n", (int)GPIOB->IDR, (int)DMA1_Channel6->CNDTR, pertec_get_dma_pos()); */
      etimer_reset(&timer);
    } else if (ev == PROCESS_EVENT_POLL) {
      printf("Poll\n");
    }
  }

  PROCESS_END();
}


AUTOSTART_PROCESSES(&avrisp_process);
