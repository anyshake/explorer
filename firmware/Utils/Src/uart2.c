#include "Utils/Inc/uart2.h"

mcu_utils_uart2_buffer_t uart2_rx_buffer;

void USART2_IRQHandler(void) {
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE)) {
        uint8_t received_byte;
        HAL_UART_Receive(&huart2, &received_byte, 1, 0);
        uint16_t next_head =
            (uart2_rx_buffer.head + 1) % MCU_UTILS_UART2_BUFFER_SIZE;
        if (next_head != uart2_rx_buffer.tail) {
            uart2_rx_buffer.buffer[uart2_rx_buffer.head] = received_byte;
            uart2_rx_buffer.head = next_head;
        }
    }
    HAL_UART_IRQHandler(&huart2);
}

void mcu_utils_uart2_init(uint32_t baudrate, bool is_rtos) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = baudrate;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    uart2_rx_buffer.head = 0;
    uart2_rx_buffer.tail = 0;
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);

    mcu_utils_delay_ms(100, is_rtos);
}

void mcu_utils_uart2_write(uint8_t* tx_data, uint16_t tx_len, bool wait) {
    HAL_UART_Transmit(&huart2, tx_data, tx_len, 1000);
}

void mcu_utils_uart2_flush(void) {
    uart2_rx_buffer.head = 0;
    uart2_rx_buffer.tail = 0;
}

void mcu_utils_uart2_end(void) {
    HAL_UART_DeInit(&huart2);
}

bool mcu_utils_uart2_hasdata(void) {
    return uart2_rx_buffer.head != uart2_rx_buffer.tail;
}

uint8_t mcu_utils_uart2_read(void) {
    if (uart2_rx_buffer.head == uart2_rx_buffer.tail) {
        return 0;
    }

    uint8_t data = uart2_rx_buffer.buffer[uart2_rx_buffer.tail];
    uart2_rx_buffer.tail =
        (uart2_rx_buffer.tail + 1) % MCU_UTILS_UART2_BUFFER_SIZE;
    return data;
}
