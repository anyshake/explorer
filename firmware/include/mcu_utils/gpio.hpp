#ifndef __MCU_UTILS_GPIO__
#define __MCU_UTILS_GPIO__

#include <Arduino.h>
#include <stdint.h>

#define MCU_GPIO_MODE_INPUT 0
#define MCU_GPIO_MODE_OUTPUT 1

void mcu_utils_gpio_mode(uint8_t pin, uint8_t mode);
void mcu_utils_gpio_high(uint8_t pin);
void mcu_utils_gpio_low(uint8_t pin);
uint8_t mcu_utils_gpio_read(uint8_t pin);

#endif
