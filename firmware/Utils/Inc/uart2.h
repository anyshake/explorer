#ifndef __ANYSHAKE_MCU_UTILS_UART2_H
#define __ANYSHAKE_MCU_UTILS_UART2_H

#include <stdbool.h>
#include <stdint.h>

#include "Core/Inc/usart.h"
#include "Utils/Inc/delay.h"
#include "stm32f1xx_hal_uart.h"

#define MCU_UTILS_UART2_BUFFER_SIZE 256

typedef struct {
    volatile uint16_t head;
    volatile uint16_t tail;
    uint8_t buffer[MCU_UTILS_UART2_BUFFER_SIZE];
} mcu_utils_uart2_buffer_t;

void mcu_utils_uart2_init(uint32_t baudrate, bool is_rtos);
void mcu_utils_uart2_write(uint8_t* tx_data, uint16_t tx_len, bool wait);
void mcu_utils_uart2_end(void);
void mcu_utils_uart2_flush(void);
uint8_t mcu_utils_uart2_read(void);
bool mcu_utils_uart2_hasdata(void);

#endif
