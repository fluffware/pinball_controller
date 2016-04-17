#include <hid-descriptors-consts.h>
#define STRUCTGEN
#include "descriptors.h"
#include "string-descriptors.h"
#include <stdio.h>

const struct usb_st_configuration_descriptor const *configuration_head =
  &descriptor_wrapper.configuration_block.configuration;


inline const struct usb_string_descriptors_languages *
usb_string_descriptors_languages_get(const struct usb_string_descriptors *descr,
				     unsigned int i)
{
  return 0;
}
  
const uint8_t *
usb_class_get_string_descriptor(uint16_t lang, uint8_t index)
{
  printf("usb_class_get_string_descriptor: %d %d\n", lang, index);
  if (index == 0) {
    const struct usb_st_language_descriptor *lang;
    lang = &hid_string_descriptors.string_descr.var.lang_descr.fix;
    return (uint8_t*)lang;
  } else {
    unsigned int nlang =
      (hid_string_descriptors.string_descr.var.lang_descr.var.bLength - 2)/2;
    const uint16_t *langid =
      hid_string_descriptors.string_descr.var.lang_descr.var.wlangid;
    uint8_t *strings =
      (uint8_t*)&hid_string_descriptors.string_descr.var.languages;
    unsigned int l;
    for (l = 0; l < nlang; l++) {
      if (*langid == lang) {
	if (index <= *strings) {
	  uint16_t offset = uint16_get(strings+1+(index - 1)*2);
	  printf("usb_class_get_string_descriptor: %d %d\n", *strings, offset);
	  return ((uint8_t*)&hid_string_descriptors) + offset;
	}
      }
      strings += *strings * 2 + 1;
    }
    printf("usb_class_get_string_descriptor: No match\n");
    return 0;
  }
}

