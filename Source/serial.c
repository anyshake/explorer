#include "serial.h"

uint8_t UART0_RX_WORD = 0;

void USART0_IRQHandler() {
    if (SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) {
        UART0_RX_WORD = usart_data_receive(USART0);
    }
}

void serial_begin(uint32_t baudRate) {
    // Set priority group
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(USART0_IRQn, 0, 0);

    // Enable PA9, PA10 as serial port
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);

    // TXD: PA9 / RXD: PA10
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    // Init serial port
    usart_deinit(USART0);
    usart_baudrate_set(USART0, baudRate);

    // Configure serial port
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_parity_config(USART0, USART_PM_NONE);

    // Enable RX & TX
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

    // Enable DMA interrupt
    usart_interrupt_enable(USART0, USART_INT_RBNE);
    usart_enable(USART0);
}

void serial_write(uint8_t ch) {
    usart_data_transmit(USART0, ch);
    while (RESET == usart_flag_get(USART0, USART_FLAG_TBE)) {
        ;
    }
}

void serial_print(uint8_t* s) {
    while (*s) {
        serial_write(*s++);
    }
}

uint8_t serial_available() {
    return UART0_RX_WORD != 0;
}

uint8_t serial_read() {
    uint8_t buf = UART0_RX_WORD;
    UART0_RX_WORD = 0;

    return buf;
}
