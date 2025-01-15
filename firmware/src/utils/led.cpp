#include "utils/led.h"

void mcu_utils_led_blink(mcu_utils_gpio_t gpio, uint8_t t, bool is_rtos) {
    if (t) {
        for (uint8_t i = 0; i <= t; i++) {
            mcu_utils_gpio_high(gpio);
            mcu_utils_delay_ms(50, is_rtos);
            mcu_utils_gpio_low(gpio);
            mcu_utils_delay_ms(50, is_rtos);
        }
    } else {
        while (true) {
            mcu_utils_gpio_high(gpio);
            mcu_utils_delay_ms(50, is_rtos);
            mcu_utils_gpio_low(gpio);
            mcu_utils_delay_ms(50, is_rtos);
        }
    }
}
