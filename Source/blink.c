#include "blink.h"

void blink_led(uint8_t times, uint16_t interval) {
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14);

    for (uint8_t i = 0; i < times; i++) {
        gpio_bit_set(GPIOB, GPIO_PIN_14);
        delay(interval);
        gpio_bit_reset(GPIOB, GPIO_PIN_14);
        delay(interval);
    }
}
