#ifndef __ANYSHAKE_MCU_UTILS_UART_UART2_H
#define __ANYSHAKE_MCU_UTILS_UART_UART2_H

#include <stdbool.h>
#include <stdint.h>

#include "Core/Inc/usart.h"
#include "Utils/Inc/delay.h"
#include "Utils/Inc/uart/isr.h"
#include "stm32f1xx_hal_uart.h"

void mcu_utils_uart2_init(uint32_t baudrate, bool is_rtos);
void mcu_utils_uart2_write(uint8_t* tx_data, uint16_t tx_len, bool wait);
void mcu_utils_uart2_end(void);
void mcu_utils_uart2_flush(void);
uint8_t mcu_utils_uart2_read(void);
bool mcu_utils_uart2_hasdata(void);
uint16_t mcu_utils_uart2_read_buf(uint8_t* dat, uint16_t len);
int16_t mcu_utils_uart2_read_line(uint8_t* str, uint16_t len);

#endif
