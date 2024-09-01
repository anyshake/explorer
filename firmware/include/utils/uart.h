#ifndef __ANYSHAKE_MCU_UTILS_UART_H
#define __ANYSHAKE_MCU_UTILS_UART_H

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

#include "utils/delay.h"

void mcu_utils_uart_init(uint32_t baudrate, bool is_rtos);
void mcu_utils_uart_write(uint8_t* tx_data, uint16_t tx_len, bool wait);
void mcu_utils_uart_end(void);
void mcu_utils_uart_flush(void);
uint8_t mcu_utils_uart_read(void);
bool mcu_utils_uart_hasdata(void);

#endif
