#include "Utils/Inc/uart/isr.h"

mcu_utils_uart_buffer_t uart1_rx_buffer;
uint8_t uart1_rx_char;

mcu_utils_uart_buffer_t uart2_rx_buffer;
uint8_t uart2_rx_char;

void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART1) {
        __HAL_UART_CLEAR_OREFLAG(huart);
        if (huart->RxState == HAL_UART_STATE_READY) {
            HAL_UART_Receive_DMA(huart, &uart1_rx_char, sizeof(uart1_rx_char));
        }
    }

    if (huart->Instance == USART2) {
        __HAL_UART_CLEAR_OREFLAG(huart);
        if (huart->RxState == HAL_UART_STATE_READY) {
            HAL_UART_Receive_DMA(huart, &uart2_rx_char, sizeof(uart2_rx_char));
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART1) {
        uint16_t next_head = (uart1_rx_buffer.head + 1) % sizeof(uart1_rx_buffer.buffer);
        if (next_head != uart1_rx_buffer.tail) {
            uart1_rx_buffer.buffer[uart1_rx_buffer.head] = uart1_rx_char;
            uart1_rx_buffer.head = next_head;
        }
        HAL_UART_Receive_DMA(huart, &uart1_rx_char, sizeof(uart1_rx_char));
    }

    if (huart->Instance == USART2) {
        uint16_t next_head = (uart2_rx_buffer.head + 1) % sizeof(uart2_rx_buffer.buffer);
        if (next_head != uart2_rx_buffer.tail) {
            uart2_rx_buffer.buffer[uart2_rx_buffer.head] = uart2_rx_char;
            uart2_rx_buffer.head = next_head;
        }
        HAL_UART_Receive_DMA(huart, &uart2_rx_char, sizeof(uart2_rx_char));
    }
}
