#include "utils/gpio.h"

void mcu_utils_gpio_init(bool is_rtos) {
    mcu_utils_delay_ms(100, is_rtos);
}

void mcu_utils_gpio_mode(mcu_utils_gpio_t gpio, uint8_t mode) {
    pinMode(gpio.pin, mode);
}

void mcu_utils_gpio_high(mcu_utils_gpio_t gpio) {
    digitalWrite(gpio.pin, HIGH);
}

void mcu_utils_gpio_low(mcu_utils_gpio_t gpio) {
    digitalWrite(gpio.pin, LOW);
}

uint8_t mcu_utils_gpio_read(mcu_utils_gpio_t gpio) {
    return digitalRead(gpio.pin);
}
