#include "Utils/Inc/uart.h"

static mcu_utils_uart_buffer_t uart_rx_buffer;
static uint8_t uart_it_rx_char;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART1) {
        uint16_t next_head = (uart_rx_buffer.head + 1) % MCU_UTILS_UART_BUFFER_SIZE;
        HAL_UART_Receive_IT(&huart1, &uart_it_rx_char, 1);
        if (next_head != uart_rx_buffer.tail) {
            uart_rx_buffer.buffer[uart_rx_buffer.head] = uart_it_rx_char;
            uart_rx_buffer.head = next_head;
        }
    }
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

        mcu_utils_uart_flush();

        HAL_UART_Receive_IT(&huart1, &uart_it_rx_char, sizeof(uart_it_rx_char));
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
        HAL_UART_AbortReceive_IT(&huart1);
        HAL_NVIC_DisableIRQ(USART1_IRQn);
        HAL_UART_DeInit(&huart1);
        mcu_utils_uart_flush();
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

int32_t mcu_utils_uart_read_buf(uint8_t* dat, uint16_t len) {
    if (len == 0) {
        return 0;
    }

    uint16_t available = uart_rx_buffer.head >= uart_rx_buffer.tail
                             ? uart_rx_buffer.head - uart_rx_buffer.tail
                             : MCU_UTILS_UART_BUFFER_SIZE - uart_rx_buffer.tail + uart_rx_buffer.head;

    uint16_t to_read = (len <= available) ? len : available;
    for (uint16_t i = 0; i < to_read; i++) {
        dat[i] = uart_rx_buffer.buffer[uart_rx_buffer.tail];
        uart_rx_buffer.tail = (uart_rx_buffer.tail + 1) % MCU_UTILS_UART_BUFFER_SIZE;
    }

    return to_read;
}

int32_t mcu_utils_uart_read_line(uint8_t* str, uint16_t len) {
    if (len == 0) {
        return 0;
    }

    uint16_t tmp_tail = uart_rx_buffer.tail;
    for (uint16_t count = 0; tmp_tail != uart_rx_buffer.head;) {
        uint8_t ch = uart_rx_buffer.buffer[tmp_tail];
        tmp_tail = (tmp_tail + 1) % MCU_UTILS_UART_BUFFER_SIZE;

        if (count + 1 >= len) {
            return -1;
        }

        str[count] = ch;
        count++;

        if (ch == '\n') {
            uart_rx_buffer.tail = tmp_tail;
            return count;
        }
    }

    return 0;
}
