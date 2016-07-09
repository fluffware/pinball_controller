#include <hid.gen.h>
#include <hid-defs.h>

const struct usb_st_device_descriptor device_descriptor =
  {
    sizeof(device_descriptor),
    DEVICE,
    0x0210,
    0x00, /* Class defined on interface level */
    0x00,
    0x00,
    undef,
    0xffff,
    0xffff,
    0x0030,
    2,
    1,
    3,
    1
  };



struct short_item {
  uint8_t bHead;
  uint8_t data[(1<<(bHead & 0x03)) >> 1];
};

struct long_item {
  uint8_t bHead = 0b1111110;
  uint8_t bDataSize;
  uint8_t bLongItemTag;
  uint8_t data[bDataSize];
};

#define ITEM4(tag,value) {(tag) | 0x03, {((value) & 0xff), (((value)>>8) & 0xff), (((value)>>16) & 0xff), ((value)>>24)}}
#define ITEM2(tag,value) {(tag) | 0x02, {((value) & 0xff), ((value)>>8)}}
#define ITEM1(tag,value) {(tag) | 0x01, {(value)}}
#define ITEM0(tag) {(tag),{}}
#define USAGE(u) ITEM1(TAG_USAGE, (u))
#define USAGE2(p,u) ITEM4(TAG_USAGE, ((p)<<16)|(u))

#define DIN_ITEM(n)				\
  USAGE2(USAGE_PAGE_ORDINAL, n),	       	\
    ITEM1(TAG_COLLECTION, COLLECTION_LOGICAL),	\
    ITEM1(TAG_USAGE, USAGE_DV_GP_DIN_STATE),	\
    ITEM2(TAG_INPUT, HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
    ITEM0(TAG_END_COLLECTION)

const struct {
  struct {
    struct short_item items[120] =
      {
	ITEM1(TAG_USAGE_PAGE, USAGE_PAGE_ALPHANUMERIC_DISPLAY),
	ITEM1(TAG_USAGE, USAGE_CA_BITMAPPED_DISPLAY), /* Bitmap display */
	ITEM1(TAG_COLLECTION, COLLECTION_APPLICATION),
	/* Bitmap attributes */
	ITEM1(TAG_USAGE, USAGE_CL_DISPLAY_ATTRIBUTES_REPORT),
	ITEM1(TAG_COLLECTION, COLLECTION_LOGICAL),
	ITEM1(TAG_REPORT_ID, 1),
	ITEM1(TAG_USAGE,USAGE_SV_BITMAP_SIZE_X),
	ITEM1(TAG_USAGE, USAGE_SV_BITMAP_SIZE_Y),
	ITEM1(TAG_USAGE, USAGE_SV_BIT_DEPTH_FORMAT),
	ITEM1(TAG_LOGICAL_MIN, 0),
	ITEM2(TAG_LOGICAL_MAX, 255),
	ITEM1(TAG_REPORT_SIZE, 8),
	ITEM1(TAG_REPORT_COUNT, 3),
	ITEM2(TAG_FEATURE, HID_CONSTANT | HID_VARIABLE | HID_ABSOLUTE),
	ITEM0(TAG_END_COLLECTION),

	/* Blit report */
	ITEM1(TAG_USAGE, USAGE_CL_BLIT_REPORT),
	ITEM1(TAG_COLLECTION, COLLECTION_LOGICAL),
	ITEM1(TAG_REPORT_ID, 2),
	ITEM1(TAG_USAGE, USAGE_SV_BLIT_RECTANGLE_X1),
	ITEM1(TAG_USAGE, USAGE_SV_BLIT_RECTANGLE_Y1),
	ITEM1(TAG_USAGE, USAGE_SV_BLIT_RECTANGLE_X2),
	ITEM1(TAG_USAGE, USAGE_SV_BLIT_RECTANGLE_Y2),
	ITEM1(TAG_REPORT_SIZE, 8),
	ITEM1(TAG_REPORT_COUNT, 4),    
	ITEM1(TAG_OUTPUT, HID_DATA | HID_VARIABLE | HID_ABSOLUTE),
	ITEM1(TAG_USAGE, USAGE_BUFFERED_BYTES_BLIT_DATA),
	ITEM1(TAG_REPORT_SIZE, 8),
	ITEM2(TAG_REPORT_COUNT, 8*8*3),
	ITEM2(TAG_OUTPUT, HID_DATA | HID_VARIABLE | HID_ABSOLUTE | HID_BUFFERED_BYTES),
	ITEM0(TAG_END_COLLECTION),


	
	ITEM0(TAG_END_COLLECTION),

	/* Inputs */
	ITEM1(TAG_USAGE_PAGE, USAGE_PAGE_ARCADE),
	ITEM1(TAG_USAGE, USAGE_CA_GPIO_BOARD),
	ITEM1(TAG_COLLECTION, COLLECTION_APPLICATION),
	ITEM1(TAG_REPORT_ID, 3),
	ITEM1(TAG_LOGICAL_MIN, 0),
	ITEM1(TAG_LOGICAL_MAX, 1),
	ITEM1(TAG_REPORT_SIZE, 1),
	ITEM1(TAG_REPORT_COUNT, 1),
	DIN_ITEM(1),
	DIN_ITEM(2),
	DIN_ITEM(3),
	DIN_ITEM(4),
	DIN_ITEM(5),
	DIN_ITEM(6),
	DIN_ITEM(7),
	DIN_ITEM(8),
	DIN_ITEM(9),
	DIN_ITEM(10),
	DIN_ITEM(11),
	DIN_ITEM(12),
	DIN_ITEM(13),
	DIN_ITEM(14),
	DIN_ITEM(15),
	DIN_ITEM(16),
	ITEM0(TAG_END_COLLECTION)
      };
  } hid_report_descriptor = undef;
  

  struct {
    struct usb_st_configuration_descriptor configuration =
      {
	sizeof(configuration),
	CONFIGURATION,
	sizeof(configuration_block),
	1,
	1,
	0,
	0x80,
	50
      };
    struct usb_st_interface_descriptor comm =
      {
	sizeof(comm),
	INTERFACE,
	0,
	0,
	1,
	HID_CLASS,
	HID_SUBCLASS_NONE,
	HID_PROTOCOL_NONE,
	0
      };
    hid_descriptor hid =  {
      sizeof(hid),
      HID_DESCRIPTOR,
      undef,
      COUNTRY_NOT_SUPPORTED,
      1,
      {
	{HID_REPORT, sizeof(hid_report_descriptor)}
      }
    };
    struct usb_st_endpoint_descriptor ep_notification =
      {
	sizeof(configuration_block.ep_notification),
	ENDPOINT,
	0x83,
	0x03,
	64,
	10
      };
  
  } configuration_block = undef;
} descriptor_wrapper = undef;

const struct {
  usb_string_descriptors string_descr = {
    {
      undef, undef,
      {USBLANG_EN, USBLANG_SV}
    },
    {
      {undef, {offsetof(product), offsetof(vendor), offsetof(version),
	       offsetof (interface)}},
      {undef, {offsetof(product), offsetof(vendor), offsetof(version),
	       offsetof (interface)}}
    }
  };
  usb_st_string_descriptor vendor = USBSTR(Fluffware);
  usb_st_string_descriptor product = USBSTR(Pinball controller);
  usb_st_string_descriptor version = USBSTR(0.1);
  usb_st_string_descriptor interface = USBSTR(HID);
} hid_string_descriptors = undef;

  
