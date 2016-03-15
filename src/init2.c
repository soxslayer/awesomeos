#include "char_dev.h"
#include "irq.h"
#include "usr_led.h"

extern void vectors_init();
extern void main();

void init2()
{
  vectors_init();
  char_dev_init();
  irq_init();
  usr_led_init();

  main();
}
