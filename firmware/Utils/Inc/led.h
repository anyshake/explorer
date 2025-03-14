#ifndef __ANYSHAKE_MCU_UTILS_LED_H
#define __ANYSHAKE_MCU_UTILS_LED_H

#include <stdbool.h>
#include <stdint.h>

#include "Utils/Inc/delay.h"
#include "Utils/Inc/gpio.h"

void mcu_utils_led_blink(mcu_utils_gpio_t gpio, uint8_t t, bool is_rtos);

#endif
