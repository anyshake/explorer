#include "mcu_utils/uart2.hpp"

SoftwareSerial SSerial(MCU_UART2_PIN_RXD, MCU_UART2_PIN_TXD);

void mcu_utils_uart2_init(uint32_t baudrate) {
    SSerial.begin(baudrate);
}

void mcu_utils_uart2_writech(uint8_t ch) {
    SSerial.write(ch);
}

void mcu_utils_uart2_flush() {
    Serial.flush();
}

uint8_t mcu_utils_uart2_hasdata() {
    return SSerial.available();
}

uint8_t mcu_utils_uart2_readch() {
    return SSerial.read();
}
