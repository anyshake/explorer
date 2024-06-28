#ifndef __MCU_UTILS_UART__
#define __MCU_UTILS_UART__

#include <Arduino.h>
#include <stdint.h>

void mcu_utils_uart_init(uint32_t baudrate);
void mcu_utils_uart_writech(uint8_t ch);
void mcu_utils_uart_flush();
uint8_t mcu_utils_uart_readch();
uint8_t mcu_utils_uart_hasdata();

#endif
