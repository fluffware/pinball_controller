#include <interrupt-utils.h>
#include <AT91SAM7S64.h>

#define ATTR

static void  __attribute__((noinline))
ws2812_int_safe (void) 
{
}

static void NACKEDFUNC ATTR
ws2812_int (void) /* SSC Interrupt Handler */ 
{
  ISR_ENTRY();
  ws2812_int_safe();
  *AT91C_AIC_EOICR = 0;                   /* End of Interrupt */
  ISR_EXIT();
}

void
ws2812_int_enable (void) /* SSC Interrupt Handler */ 
{
 
  
  AT91C_AIC_SMR[AT91C_ID_SSC] = AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 5;
  AT91C_AIC_SVR[AT91C_ID_SYS] = (unsigned long) ws2812_int;
  *AT91C_AIC_IECR = (1 << AT91C_ID_SSC);
 
}

void
ws2812_int_disable(void)
{
  *AT91C_AIC_IDCR = (1 << AT91C_ID_SSC);
}
