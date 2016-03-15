#include "arm.h"

void __dsb()
{
  asm("dsb");
}
