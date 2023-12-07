#include "mcu_utils/uart.hpp"

void uart_init(uint32_t baudrate) {
    Serial.begin(baudrate);
}

void uart_writech(uint8_t ch) {
    Serial.write(ch);
}

void uart_println(const char* str) {
    Serial.println(str);
}

uint8_t uart_available() {
    return Serial.available();
}

uint8_t uart_readch() {
    return Serial.read();
}
