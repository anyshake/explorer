#ifndef __MCU_UTILS_UART_HPP
#define __MCU_UTILS_UART_HPP

#include <Arduino.h>
#include <stdint.h>

void uart_init(uint32_t baudrate);
void uart_writebuf(uint8_t* tx_buf, uint16 len);

#endif
