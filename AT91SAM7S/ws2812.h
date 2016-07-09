#ifndef __WS2812_H__UOBH9FMSXE__
#define __WS2812_H__UOBH9FMSXE__

#include <stdint.h>
struct WS2812BitBuffer
{
  uint8_t green[3];
  uint8_t red[3];
  uint8_t blue[3];
};

void
ws2812_init();

void
ws2812_start(struct  WS2812BitBuffer *buffer, unsigned int len);

void
ws2812_stop(struct  WS2812BitBuffer *buffer, unsigned int len);

void
ws2812_encode_rgb(struct WS2812BitBuffer *bits, uint8_t r, uint8_t g, uint8_t b);

#endif /* __WS2812_H__UOBH9FMSXE__ */
