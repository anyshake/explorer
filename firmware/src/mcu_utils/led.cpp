#include "mcu_utils/led.hpp"

void led_blink(uint8_t pin, uint8_t t) {
    gpio_mode(pin, GPIO_MODE_OUTPUT);
    for (uint8_t i = 0; i < (t > 0 ? t : 1); i++) {
        gpio_low(pin);
        delay(50);
        gpio_high(pin);
        delay(50);
    }
}
