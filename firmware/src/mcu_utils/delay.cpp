#include "mcu_utils/delay.hpp"

void delay_ms(uint32_t ms) {
    delay(ms);
}

void delay_us(uint32_t us) {
    delayMicroseconds(us);
}
