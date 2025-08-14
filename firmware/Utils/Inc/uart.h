#ifndef __ANYSHAKE_MCU_UTILS_UART_H
#define __ANYSHAKE_MCU_UTILS_UART_H

#include <stdbool.h>
#include <stdint.h>

#include "Core/Inc/usart.h"
#include "Utils/Inc/delay.h"
#include "stm32f1xx_hal_uart.h"

#define MCU_UTILS_UART_BUFFER_SIZE 64

typedef struct {
    volatile uint16_t head;
    volatile uint16_t tail;
    uint8_t buffer[MCU_UTILS_UART_BUFFER_SIZE];
} mcu_utils_uart_buffer_t;

void mcu_utils_uart_init(uint32_t baudrate, bool is_rtos);
void mcu_utils_uart_write(uint8_t* tx_data, uint16_t tx_len, bool wait);
void mcu_utils_uart_end(void);
void mcu_utils_uart_flush(void);
uint8_t mcu_utils_uart_read(void);
bool mcu_utils_uart_hasdata(void);
int32_t mcu_utils_uart_read_buf(uint8_t* dat, uint16_t len);
int32_t mcu_utils_uart_read_line(uint8_t* str, uint16_t len);

#endif
