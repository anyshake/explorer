#include "spi.h"

void spi_begin() {
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SPI_SCK | SPI_MOSI);
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, SPI_MISO);
}

uint8_t spi_transfer(uint8_t dat) {
    uint8_t rx = 0;

    gpio_bit_set(GPIOA, SPI_SCK);
    for (uint8_t i = 0; i < 8; i++) {
        if (dat & 0x80) {
            gpio_bit_set(GPIOA, SPI_MOSI);
        } else {
            gpio_bit_reset(GPIOA, SPI_MOSI);
        }

        dat <<= 1;
        delayMicroseconds(1);

        gpio_bit_set(GPIOA, SPI_SCK);
        delayMicroseconds(1);

        rx <<= 1;
        rx |= gpio_input_bit_get(GPIOA, SPI_MISO);

        gpio_bit_reset(GPIOA, SPI_SCK);
    }

    return rx;
}

void spi_end(void) {
    ;
}
