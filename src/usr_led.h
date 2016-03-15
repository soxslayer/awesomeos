#pragma once

#include "types.h"

#define USR_LED0 21
#define USR_LED1 22
#define USR_LED2 23
#define USR_LED3 24

void usr_led_init();
void usr_led_on(uint32_t led);
void usr_led_off(uint32_t led);
void usr_led_nibble(uint8_t digit);
void usr_led_dbg(uint8_t digit);
