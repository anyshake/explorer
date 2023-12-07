#ifndef __MCU_UTILS_INIT_HPP
#define __MCU_UTILS_INIT_HPP

#include <Arduino.h>
#include <stdint.h>

#include "mcu_utils/gpio.hpp"

void led_blink(uint8_t pin, uint8_t t);

#endif
