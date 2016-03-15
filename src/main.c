//#include "stdio.h"
#include "char_dev.h"
#include "irq.h"
#include "mem.h"
#include "usr_led.h"

static int i;

void main()
{
  int u;
  
  u = char_dev_open(":uart0");

  irq_enable(IRQ_ENABLE);

  //printf ("\nRunning...\nfdsa\n");
  //printf ("asdf\n");
  //printf ("Something else\n");
  //printf ("This is one long ass string that is surely going to overflow the\nUART hardware buf\n");
  i = char_dev_write(u, "This is one long ass string that is surely going to \r\noverflow the UART hardware bux\r\n", 86);
  //write(u, "This is one long ass string that is surely going to \r\noverflow the UART hardware bux\r\n", 86);
  //write(u, "AwesomeOS\r\n", 11);
  //write(u, "This is one long ass string that is surely going to \r\noverflow the UART hardware bux\r\n", 86);

  //INTC_ILR(98) = 0x4;
  //INTC_MIR_CLEAR3 = INTC_INT32_BIT(98);

  //GPIO_IRQSTATUS_SET_0(GPIO1_BASE) = _b(USR_LED3);
  //GPIO_RISINGDETECT(GPIO1_BASE) = _b(USR_LED3);

  while (1);
}
