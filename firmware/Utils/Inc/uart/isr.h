#ifndef __ANYSHAKE_MCU_UTILS_UART_ISR_H
#define __ANYSHAKE_MCU_UTILS_UART_ISR_H

#include <stdint.h>

#include "Core/Inc/usart.h"
#include "stm32f1xx_hal_uart.h"

typedef struct {
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint8_t buffer[256];
} mcu_utils_uart_buffer_t;

#endif
