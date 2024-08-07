#include "mcu_utils/uart.hpp"

void uart_init(uint32_t baudrate) {
    Serial.begin(baudrate);
}

void uart_writebuf(uint8_t *tx_buf, uint16_t len) {
    Serial.write(tx_buf, len);
}
