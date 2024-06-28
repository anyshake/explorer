#ifndef __MCU_UTILS_LED__
#define __MCU_UTILS_LED__

#include <Arduino.h>
#include <stdint.h>
#include "mcu_utils/delay.hpp"
#include "mcu_utils/gpio.hpp"

void mcu_utils_led_blink(uint8_t pin, uint8_t t, uint8_t rtos);

#endif
