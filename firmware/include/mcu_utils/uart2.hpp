#ifndef __MCU_UTILS_UART2__
#define __MCU_UTILS_UART2__

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stdint.h>

#define MCU_UART2_PIN_TXD PB15
#define MCU_UART2_PIN_RXD PB14

void mcu_utils_uart2_init(uint32_t baudrate);
void mcu_utils_uart2_writech(uint8_t ch);
void mcu_utils_uart2_flush();
uint8_t mcu_utils_uart2_readch();
uint8_t mcu_utils_uart2_hasdata();

#endif
