#include <AT91SAM7S64.h>

#include <ws2812.h>
#include <ws2812-interrupt.h>

#define TX_PIN AT91C_PA17_TD
#define BITCLK 2400000 





void
ws2812_init()
{
  
  *AT91C_PMC_PCER = (1 << AT91C_ID_SSC); /* Turn on clock */
  *AT91C_SSC_CR = AT91C_SSC_SWRST;
  *AT91C_SSC_CMR = ((MCK/2+BITCLK/2)/BITCLK); /* Set bit clock */
  *AT91C_SSC_TCMR = AT91C_SSC_CKO_CONTINOUS;
  *AT91C_SSC_TFMR = AT91C_SSC_MSBF | 7; /* 8 bits, high bit first */

  /* Setup output pin */
  *AT91C_PIOA_ASR = TX_PIN;
  *AT91C_PIOA_PDR = TX_PIN;
  *AT91C_PIOA_PPUDR = TX_PIN;

  *AT91C_SSC_CR = AT91C_SSC_TXEN;

}

void
ws2812_start(struct WS2812BitBuffer *buffer, unsigned int len)
{
  *AT91C_SSC_TPR = (uint32_t)buffer;
  *AT91C_SSC_TCR = len*sizeof(struct WS2812BitBuffer);
  *AT91C_SSC_PTCR = AT91C_PDC_TXTEN;
}

void
ws2812_stop(struct WS2812BitBuffer *buffer, unsigned int len)
{
  *AT91C_SSC_TCR = 0;
}

static void
encode_bits(uint8_t *bits, uint8_t v)
{
  /* First 3 bits */
  static const uint8_t high_byte[8] = {0x92,0x93,0x9a,0x9b,
				       0xd2,0xd3,0xda,0xdb};

  /* Middle 2 bits */
  static const uint8_t mid_byte[4] = {0x49,0x4d,0x69,0x6d};

  /* Last 3 bits */
  static const uint8_t low_byte[8] = {0x24, 0x26, 0x34, 0x36, 
				      0xa4, 0xa6, 0xb4, 0xb6};

  bits[0] = high_byte[(v>>5) & 7];
  bits[1] = mid_byte[(v>>3) & 3];
  bits[2] = low_byte[v & 7];
}

void
ws2812_encode_rgb(struct WS2812BitBuffer *bits, uint8_t r, uint8_t g, uint8_t b)
{
  
  encode_bits(bits->red, r);
  encode_bits(bits->green, g);
  encode_bits(bits->blue, b);
};
  
