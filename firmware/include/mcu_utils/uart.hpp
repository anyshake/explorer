#ifndef __MCU_UTILS_UART_HPP
#define __MCU_UTILS_UART_HPP

#include <Arduino.h>
#include <stdint.h>

void uart_init(uint32_t baudrate);
void uart_writech(uint8_t ch);
uint8_t uart_hasdata();
uint8_t uart_readch();

#endif
