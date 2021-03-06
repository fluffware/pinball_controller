/*  HID Class */
/* Class code */
#define HID_CLASS			0x03

/* Subclass codes */
#define HID_SUBCLASS_NONE 0x00
#define HID_SUBCLASS_BOOT 0x01

/* Protocol codes */
#define HID_PROTOCOL_NONE 0x00
#define HID_PROTOCOL_KEYBOARD 0x01
#define HID_PROTOCOL_MOUSE 0x02

/* Interface subclass codes */


/* Protocols */

/* Country codes */
#define COUNTRY_NOT_SUPPORTED 0
#define COUNTRY_ARABIC 1
#define COUNTRY_BELGIAN 2
#define COUNTRY_CANADIAN_BILINGUAL 3
#define COUNTRY_CANADIAN_FRENCH 4
#define COUNTRY_CZECH_REPUBLIC 5
#define COUNTRY_DANISH 6
#define COUNTRY_FINNISH 7
#define COUNTRY_FRENCH 8
#define COUNTRY_GERMAN 9
#define COUNTRY_GREEK 10
#define COUNTRY_HEBREW 11
#define COUNTRY_HUNGARY 12
#define COUNTRY_INTERNATIONAL 13
#define COUNTRY_ITALIAN 14
#define COUNTRY_JAPAN 15
#define COUNTRY_KOREAN 16
#define COUNTRY_LATIN_AMERICAN 17

#define NETHERLANDS_DUTCH 18
#define COUNTRY_NORWEGIAN 19
#define COUNTRY_PERSIAN 20
#define COUNTRY_POLAND 21
#define COUNTRY_PORTUGUESE 22
#define COUNTRY_RUSSIA 23
#define COUNTRY_SLOVAKIA 24
#define COUNTRY_SPANISH 25
#define COUNTRY_SWEDISH 26
#define COUNTRY_SWISS_FRENCH 27
#define COUNTRY_SWISS_GERMAN 28
#define COUNTRY_SWITZERLAND 29
#define COUNTRY_TAIWAN 30
#define COUNTRY_TURKISH_Q 31 
#define COUNTRY_UK 32
#define COUNTRY_US 33
#define COUNTRY_YUGOSLAVIA 34
#define COUNTRY_TURKISH_F 35

/* Descriptor types */
#define HID_DESCRIPTOR 0x21
#define HID_REPORT 0x22

/* Main */
#define TAG_INPUT 0x80
#define TAG_OUTPUT 0x90
#define TAG_FEATURE 0xb0
#define TAG_COLLECTION 0xa0
#define TAG_END_COLLECTION 0xc0

/* Global */
#define TAG_USAGE_PAGE 0x04
#define TAG_LOGICAL_MIN 0x14
#define TAG_LOGICAL_MAX 0x24
#define TAG_PHYSICAL_MIN 0x34
#define TAG_PHYSICAL_MAX 0x44
#define TAG_UNIT_EXPONENT 0x54
#define TAG_UNIT 0x64
#define TAG_REPORT_SIZE 0x74
#define TAG_REPORT_ID 0x84
#define TAG_REPORT_COUNT 0x94
#define TAG_PUSH 0xa4
#define TAG_POP 0xb4

/*Local */
#define TAG_USAGE 0x08
#define TAG_USAGE_MIN 0x18
#define TAG_USAGE_MAX 0x28
#define TAG_DESIGNATOR 0x38
#define TAG_DESIGNATOR_MIN 0x48
#define TAG_DESIGNATOR_MAX 0x58
#define TAG_STRING 0x78
#define TAG_STRING_MIN 0x88
#define TAG_STRING_MAX 0x98
#define TAG_DELIMITER 0xa8

#define USB_REQ_GET_REPORT 0x01
#define USB_REQ_SET_REPORT 0x09
#define USB_REQ_GET_IDLE 0x02
#define USB_REQ_SET_IDLE 0x0a
#define USB_REQ_GET_PROTOCOL 0x03
#define USB_REQ_SET_PROTOCOL 0x0b

#define USB_HID_CLASS 0x03

#define HID_DATA 0x00
#define HID_CONSTANT 0x01
#define HID_ARRAY 0x00
#define HID_VARIABLE 0x02
#define HID_ABSOLUTE 0x00
#define HID_RELATIVE 0x04
#define HID_NO_WRAP 0x00
#define HID_WRAP 0x08
#define HID_LINEAR 0x00
#define HID_NON_LINEAR 0x10
#define HID_PREFERRED_STATE 0x00
#define HID_NO_PREFERRED_STATE 0x20
#define HID_NO_NULL_STATE 0x00
#define HID_NULL_STATE 0x40
#define HID_NON_VOLATILE 0x00
#define HID_VOLATILE 0x80
#define HID_BIT_FIELD 0x00
#define HID_BUFFERED_BYTES 0x100

/* Usage page */
#define USAGE_PAGE_GENERIC_DESKTOP 0x01
#define USAGE_PAGE_BUTTON 0x09
#define USAGE_PAGE_ORDINAL 0x0a
#define USAGE_PAGE_ALPHANUMERIC_DISPLAY 0x14
#define USAGE_PAGE_ARCADE 0x91

/* Generic Desktop page */
#define USAGE_CP_POINTER 0x01
#define USAGE_CA_MOUSE 0x02
#define USAGE_CA_JOYSTICK 0x04
#define USAGE_CA_GAMEPAD 0x05
#define USAGE_CA_KEYBOARD 0x06
#define USAGE_CA_KEYPAD 0x07
#define USAGE_CA_MULTIAXIS_CONTROLLER 0x08

#define USAGE_DV_X 0x30
#define USAGE_DV_Y 0x31
#define USAGE_DV_Z 0x32
#define USAGE_DV_Rx 0x33
#define USAGE_DV_Ry 0x34
#define USAGE_DV_Rz 0x35

#define USAGE_DV_SLIDER 0x36
#define USAGE_DV_DIAL 0x37
#define USAGE_DV_WHEEL 0x38
#define USAGE_DV_HAT_SWITCH 0x39



/* Button page */
#define USAGE_BUTTON(x) (x)

/* Alphanumeric Display Page */
#define USAGE_CA_ALPHANUMERIC_DISPLAY 0x01
#define USAGE_CA_BITMAPPED_DISPLAY 0x02

#define USAGE_CL_DISPLAY_ATTRIBUTES_REPORT 0x20
#define USAGE_SF_ASCII_CHARACTER_SET 0x21
#define USAGE_SF_DATA_READ_BACK 0x22
#define USAGE_SF_DATA_FONT_BACK 0x23
#define USAGE_CL_DISPLAY_CONTROL_REPORT 0x24
#define USAGE_DF_CLEAR_DISPLAY 0x25
#define USAGE_DF_DISPLAY_ENABLE 0x26
#define USAGE_SV_SCREEN_SAVER_DELAY 0x27 
#define USAGE_DV_SCREEN_SAVER_DELAY 0x27 
#define USAGE_DF_SCREEN SAVER ENABLE 0x28 
#define USAGE_SF_VERTICAL_SCROLL 0x29
#define USAGE_DF_VERTICAL_SCROLL 0x29
#define USAGE_SF_HORIZONTAL_SCROLL 0x2A
#define USAGE_DF_HORIZONTAL_SCROLL 0x2A
#define USAGE_CL_CHARACTER_REPORT 0x2B
#define USAGE_DV_DISPLAY_DATA 0x2C
#define USAGE_CL_DISPLAY_STATUS 0x2D
#define USAGE_SEL_STAT_NOT_READY 0x2E
#define USAGE_SEL_STAT_READY 0x2F
#define USAGE_SEL_ERR_NOT_A_LOADABLE_CHARACTER 0x30
#define USAGE_SEL_ERR_FONT_DATA_CANNOT_BE_READ 0x31
#define USAGE_CL_CURSOR_POSITION_REPORT 0x32
#define USAGE_DV_ROW 0x33
#define USAGE_DV_COLUMN 0x34
#define USAGE_SV_ROWS 0x35
#define USAGE_SV_COLUMNS 0x36
#define USAGE_SF_CURSOR_PIXEL_POSITIONING 0x37
#define USAGE_DF_CURSOR_MODE 0x38
#define USAGE_DF_CURSOR_ENABLE  0x39
#define USAGE_DF_CURSOR_BLINK 0x3A
#define USAGE_CL_FONT_REPORT 0x3B
#define USAGE_BUFFERED_BYTE_FONT_DATA 0x3C
#define USAGE_SV_CHARACTER_WIDTH 0x3D
#define USAGE_SV_CHARACTER_HEIGHT 0x3E
#define USAGE_SV_CHARACTER_SPACING_HORIZONTAL 0x3F
#define USAGE_SV_CHARACTER_SPACING_VERTICAL 0x40
#define USAGE_SF_UNICODE_CHARACTER_SET 0x41
#define USAGE_SF_FONT_7_SEGMENT 0x42
#define USAGE_SF_7_SEGMENT_DIRECT_MAP 0x43
#define USAGE_SF_FONT_14_SEGMENT 0x44
#define USAGE_SF_14_SEGMENT_DIRECT_MAP 0x45
#define USAGE_DV_DISPLAY_BRIGHTNESS 0x46
#define USAGE_DV_DISPLAY_CONTRAST 0x47
#define USAGE_CL_CHARACTER_ATTRIBUTE 0x48
#define USAGE_SF_ATTRIBUTE_READBACK 0x49
#define USAGE_DV_ATTRIBUTE_DATA 0x4A
#define USAGE_OOC_CHAR_ATTR_ENHANCE 0x4B
#define USAGE_OOC_CHAR_ATTR_UNDERLINE 0x4C
#define USAGE_OOC_CHAR_ATTR_BLINK 0x4D
#define USAGE_SV_BITMAP_SIZE_X  0x80 
#define USAGE_SV_BITMAP_SIZE_Y 0x81
#define USAGE_SV_BIT_DEPTH_FORMAT 0x83
#define USAGE_DV_DISPLAY_ORIENTATION 0x84
#define USAGE_CL_PALETTE_REPORT 0x85
#define USAGE_SV_PALETTE_DATA_SIZE 0x86
#define USAGE_SV_PALETTE_DATA_OFFSET 0x87
#define USAGE_BUFFERED_BYTES_PALETTE_DATA 0x88
#define USAGE_CL_BLIT_REPORT 0x8A
#define USAGE_SV_BLIT_RECTANGLE_X1 0x8B
#define USAGE_SV_BLIT_RECTANGLE_Y1 0x8C
#define USAGE_SV_BLIT_RECTANGLE_X2 0x8D
#define USAGE_SV_BLIT_RECTANGLE_Y2 0x8E
#define USAGE_BUFFERED_BYTES_BLIT_DATA 0x8F
#define USAGE_CL_SOFT_BUTTON 0x90
#define USAGE_SV_SOFT_BUTTON_ID 0x91
#define USAGE_SV_SOFT_BUTTON_SIDE 0x92
#define USAGE_SV_SOFT_BUTTON_OFFSET_1 0x93
#define USAGE_SV_SOFT_BUTTON_OFFSET_2 0x94
#define USAGE_SV_SOFT_BUTTON_REPORT 0x95

/* Arcade page */
#define USAGE_CA_GPIO_BOARD 0x01
#define USAGE_DV_GP_AIN_STATE 0x30
#define USAGE_DV_GP_DIN_STATE 0x31
#define USAGE_DV_GP_OPTICAL_DIN_STATE 0x32
#define USAGE_DV_GP_DOUT_STATE 0x33


/* Collection types*/
#define COLLECTION_PHYSICAL 0x00
#define COLLECTION_APPLICATION 0x01
#define COLLECTION_LOGICAL 0x02
#define COLLECTION_REPORT 0x03
#define COLLECTION_NAMED_ARRAY 0x04
#define COLLECTION_USAGE_SWITCH 0x05
#define COLLECTION_USAGE_MODIFIER 0x06
