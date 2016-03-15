#include "usr_led.h"

#include "gpio.h"
#include "types.h"

void usr_led_mask(uint8_t mask)
{
}

void usr_led_init()
{
  gpio_init(&gpio1);
  gpio_enable_output(&gpio1, USR_LED0);
  gpio_enable_output(&gpio1, USR_LED1);
  gpio_enable_output(&gpio1, USR_LED2);
  gpio_enable_output(&gpio1, USR_LED3);
}

void usr_led_on(uint32_t led)
{
  if (led >= USR_LED0 && led <= USR_LED3)
    gpio_pin_high(&gpio1, led);
}

void usr_led_off(uint32_t led)
{
  if (led >= USR_LED0 && led <= USR_LED3)
    gpio_pin_low(&gpio1, led);
}

void usr_led_nibble(uint8_t digit)
{
  gpio_mask_low(&gpio1, 0xf << USR_LED0);
  gpio_mask_high(&gpio1, (digit & 0xf) << USR_LED0);
}

void usr_led_dbg(uint8_t digit)
{
  while (1)
    usr_led_nibble(digit);
}
