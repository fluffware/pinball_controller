#include <usb.gen.h>
#include <hid-defs.h>

struct hid_descriptor
{
  uint8_t  bLength;             /* Size of this descriptor in bytes */
  uint8_t  bDescriptorType;	/* CS_INTERFACE descriptor type */
  uint16_t bcdHID = 0x0111;	/* BCD HID Class specification relese */
  uint8_t  bCountryCode;	/* Country code of localized hardware */
  uint8_t  bNumDescriptors;	/* Nimber of class descriptors */
  struct {
    uint8_t  bDescriptorType;
    uint16_t wDescriptorLength;
  } descr[bNumDescriptors];
};

