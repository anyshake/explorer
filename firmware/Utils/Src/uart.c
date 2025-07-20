#include "Utils/Inc/uart.h"

static mcu_utils_uart_buffer_t uart_rx_buffer;

void USART1_IRQHandler(void) {
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
        uint8_t received_byte;
        HAL_UART_Receive(&huart1, &received_byte, 1, 0);
        uint16_t next_head = (uart_rx_buffer.head + 1) % MCU_UTILS_UART_BUFFER_SIZE;
        if (next_head != uart_rx_buffer.tail) {
            uart_rx_buffer.buffer[uart_rx_buffer.head] = received_byte;
            uart_rx_buffer.head = next_head;
        }
    }
    HAL_UART_IRQHandler(&huart1);
}

void mcu_utils_uart_init(uint32_t baudrate, bool is_rtos) {
    if (HAL_UART_GetState(&huart1) == HAL_UART_STATE_RESET) {
        huart1.Instance = USART1;
        huart1.Init.BaudRate = baudrate;
        huart1.Init.WordLength = UART_WORDLENGTH_8B;
        huart1.Init.StopBits = UART_STOPBITS_1;
        huart1.Init.Parity = UART_PARITY_NONE;
        huart1.Init.Mode = UART_MODE_TX_RX;
        huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart1.Init.OverSampling = UART_OVERSAMPLING_16;
        HAL_UART_Init(&huart1);

        HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);

        uart_rx_buffer.head = 0;
        uart_rx_buffer.tail = 0;
    }

    mcu_utils_delay_ms(100, is_rtos);
}

void mcu_utils_uart_write(uint8_t* tx_data, uint16_t tx_len, bool wait) {
    HAL_UART_Transmit_DMA(&huart1, tx_data, tx_len);
    while (wait && __HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET) {
        ;
    }
}

void mcu_utils_uart_flush(void) {
    uart_rx_buffer.head = 0;
    uart_rx_buffer.tail = 0;
}

void mcu_utils_uart_end(void) {
    if (HAL_UART_GetState(&huart1) != HAL_UART_STATE_RESET) {
        __HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE);
        HAL_UART_DeInit(&huart1);
    }
}

bool mcu_utils_uart_hasdata(void) {
    return uart_rx_buffer.head != uart_rx_buffer.tail;
}

uint8_t mcu_utils_uart_read(void) {
    if (uart_rx_buffer.head == uart_rx_buffer.tail) {
        return 0;
    }

    uint8_t data = uart_rx_buffer.buffer[uart_rx_buffer.tail];
    uart_rx_buffer.tail = (uart_rx_buffer.tail + 1) % MCU_UTILS_UART_BUFFER_SIZE;
    return data;
}
