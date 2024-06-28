#include "mcu_utils/uart.hpp"

void mcu_utils_uart_init(uint32_t baudrate) {
    Serial.begin(baudrate);
}

void mcu_utils_uart_writech(uint8_t ch) {
    Serial.write(ch);
}

void mcu_utils_uart_flush() {
    Serial.flush();
}

uint8_t mcu_utils_uart_hasdata() {
    return Serial.available();
}

uint8_t mcu_utils_uart_readch() {
    return Serial.read();
}
