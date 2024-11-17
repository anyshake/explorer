#include "Utils/Inc/gpio.h"

void mcu_utils_gpio_init(bool is_rtos) {
    MX_GPIO_Init();
    mcu_utils_delay_ms(100, is_rtos);
}

void mcu_utils_gpio_mode(mcu_utils_gpio_t gpio, uint8_t mode) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = gpio.pin;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Mode = mode == MCU_UTILS_GPIO_MODE_INPUT
                               ? GPIO_MODE_INPUT
                               : GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(gpio.port, &GPIO_InitStruct);
}

void mcu_utils_gpio_high(mcu_utils_gpio_t gpio) {
    HAL_GPIO_WritePin(gpio.port, gpio.pin, GPIO_PIN_SET);
}

void mcu_utils_gpio_low(mcu_utils_gpio_t gpio) {
    HAL_GPIO_WritePin(gpio.port, gpio.pin, GPIO_PIN_RESET);
}

uint8_t mcu_utils_gpio_read(mcu_utils_gpio_t gpio) {
    GPIO_PinState state = HAL_GPIO_ReadPin(gpio.port, gpio.pin);
    return state == GPIO_PIN_SET;
}
