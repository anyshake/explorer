#ifndef __MCU_UTILS_GPIO_HPP
#define __MCU_UTILS_GPIO_HPP

#include <Arduino.h>
#include <stdint.h>

#define GPIO_MODE_INPUT 0
#define GPIO_MODE_OUTPUT 1

void gpio_mode(uint8_t pin, uint8_t mode);
void gpio_high(uint8_t pin);
void gpio_low(uint8_t pin);
uint8_t gpio_read(uint8_t pin);

#endif
