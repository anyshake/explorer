#include "mcu_utils/uart.hpp"

void uart_init(uint32_t baudrate) {
    Serial.begin(baudrate);
}

void uart_writech(uint8_t ch) {
    Serial.write(ch);
}

uint8_t uart_hasdata() {
    return Serial.available();
}

uint8_t uart_readch() {
    return Serial.read();
}
