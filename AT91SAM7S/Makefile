TARGET=stepper-robot

vpath %.c $(BUILTSRCDIR)
all: pinball

CONTIKI_NO_NET=1
#CONTIKI=/tmp/contiki-2.x
CONTIKI=/home/ksb/projects/contiki-2.x

CONTIKI_TARGET_MAIN=usb-main.c 

PROJECT_SOURCEFILES=hid-descriptors.c hid-descriptors-consts.c ws2812.c ws2812-interrupt.c 


#USB_CDC_ACM_CLASS=1

clean: project_clean

.PHONY: project_clean

project_clean:

include $(CONTIKI)/Makefile.include

MCK=47923200

