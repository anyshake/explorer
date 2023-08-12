#include "ads1256/rw.h"

void adc_wait() {
    while (gpio_input_bit_get(GPIOA, DRDY_PIN)) {
        ;
    }
}

uint8_t adc_read_register(uint8_t reg) {
    adc_wait();
    spi_begin();

    // Select chip and read
    gpio_bit_reset(GPIOA, CS_PIN);
    spi_transfer(DEVICE_COMMAND_RREG | reg);
    spi_transfer(DEVICE_COMMAND_WAKEUP);

    // Get register value
    delayMicroseconds(5);
    uint8_t result = spi_transfer(0xFF);

    // Deselect chip
    gpio_bit_set(GPIOA, CS_PIN);
    spi_end();

    return result;
}

void adc_write_register(uint8_t reg, uint8_t value) {
    adc_wait();
    spi_begin();

    // Select chip and read
    gpio_bit_reset(GPIOA, CS_PIN);
    spi_transfer(DEVICE_COMMAND_WREG | reg);
    spi_transfer(DEVICE_COMMAND_WAKEUP);
    spi_transfer(value);

    // Deselect chip
    gpio_bit_set(GPIOA, CS_PIN);
    spi_end();
}
