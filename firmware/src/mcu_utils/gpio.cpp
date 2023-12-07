#include "mcu_utils/gpio.hpp"

void gpio_mode(uint8_t pin, uint8_t mode) {
    pinMode(pin, mode == GPIO_MODE_INPUT ? INPUT : OUTPUT);
    if (mode == OUTPUT) {
        digitalWrite(pin, HIGH);
    }
}

void gpio_high(uint8_t pin) {
    digitalWrite(pin, HIGH);
}

void gpio_low(uint8_t pin) {
    digitalWrite(pin, LOW);
}

uint8_t gpio_read(uint8_t pin) {
    return !!digitalRead(pin) ? HIGH : LOW;
}
