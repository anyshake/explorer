#include "utils/uart.h"

void mcu_utils_uart_init(uint32_t baudrate, bool is_rtos) {
    Serial.begin(baudrate);
    mcu_utils_delay_ms(100, is_rtos);
}

void mcu_utils_uart_write(uint8_t* tx_data, uint16_t tx_len, bool wait) {
    Serial.write(tx_data, tx_len);
}

void mcu_utils_uart_flush(void) {
    Serial.flush();
}

void mcu_utils_uart_end(void) {
    Serial.end();
}

bool mcu_utils_uart_hasdata(void) {
    return Serial.available() > 0;
}

uint8_t mcu_utils_uart_read(void) {
    return Serial.read();
}
