#include "Utils/Inc/uart2.h"

static mcu_utils_uart2_buffer_t uart2_rx_buffer;
static uint8_t uart2_dma_rx_buffer[MCU_UTILS_UART2_DMA_RX_SIZE];

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART2) {
        for (uint16_t i = 0; i < MCU_UTILS_UART2_DMA_RX_SIZE / 2; i++) {
            uint8_t ch = uart2_dma_rx_buffer[i];
            uint16_t next_head = (uart2_rx_buffer.head + 1) % MCU_UTILS_UART2_BUFFER_SIZE;
            if (next_head != uart2_rx_buffer.tail) {
                uart2_rx_buffer.buffer[uart2_rx_buffer.head] = ch;
                uart2_rx_buffer.head = next_head;
            }
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART2) {
        for (uint16_t i = MCU_UTILS_UART2_DMA_RX_SIZE / 2; i < MCU_UTILS_UART2_DMA_RX_SIZE; i++) {
            uint8_t ch = uart2_dma_rx_buffer[i];
            uint16_t next_head = (uart2_rx_buffer.head + 1) % MCU_UTILS_UART2_BUFFER_SIZE;
            if (next_head != uart2_rx_buffer.tail) {
                uart2_rx_buffer.buffer[uart2_rx_buffer.head] = ch;
                uart2_rx_buffer.head = next_head;
            }
        }
    }
}

void mcu_utils_uart2_init(uint32_t baudrate, bool is_rtos) {
    if (HAL_UART_GetState(&huart2) == HAL_UART_STATE_RESET) {
        huart2.Instance = USART2;
        huart2.Init.BaudRate = baudrate;
        huart2.Init.WordLength = UART_WORDLENGTH_8B;
        huart2.Init.StopBits = UART_STOPBITS_1;
        huart2.Init.Parity = UART_PARITY_NONE;
        huart2.Init.Mode = UART_MODE_TX_RX;
        huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart2.Init.OverSampling = UART_OVERSAMPLING_16;
        HAL_UART_Init(&huart2);

        HAL_UART_Receive_DMA(&huart2, uart2_dma_rx_buffer, MCU_UTILS_UART2_DMA_RX_SIZE);
        HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

        uart2_rx_buffer.head = 0;
        uart2_rx_buffer.tail = 0;
    }

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
    if (HAL_UART_GetState(&huart2) != HAL_UART_STATE_RESET) {
        HAL_UART_DMAStop(&huart2);
        HAL_UART_DeInit(&huart2);
        HAL_NVIC_DisableIRQ(DMA1_Channel6_IRQn);
    }
}

bool mcu_utils_uart2_hasdata(void) {
    return uart2_rx_buffer.head != uart2_rx_buffer.tail;
}

uint8_t mcu_utils_uart2_read(void) {
    if (uart2_rx_buffer.head == uart2_rx_buffer.tail) {
        return 0;
    }

    uint8_t data = uart2_rx_buffer.buffer[uart2_rx_buffer.tail];
    uart2_rx_buffer.tail = (uart2_rx_buffer.tail + 1) % MCU_UTILS_UART2_BUFFER_SIZE;
    return data;
}
