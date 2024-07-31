#ifndef __ANYSHAKE_MCU_UTILS_GPIO_H
#define __ANYSHAKE_MCU_UTILS_GPIO_H

#include <stdbool.h>
#include <stdint.h>

#include "Core/Inc/gpio.h"
#include "Utils/Inc/delay.h"
#include "stm32f1xx_hal.h"

#define MCU_UTILS_GPIO_MODE_INPUT 0
#define MCU_UTILS_GPIO_MODE_OUTPUT 1

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} mcu_utils_gpio_t;

void mcu_utils_gpio_init(bool is_rtos);
void mcu_utils_gpio_mode(mcu_utils_gpio_t gpio, uint8_t mode);
void mcu_utils_gpio_high(mcu_utils_gpio_t gpio);
void mcu_utils_gpio_low(mcu_utils_gpio_t gpio);
uint8_t mcu_utils_gpio_read(mcu_utils_gpio_t gpio);

#endif
