#include "ads1256/mode.h"

void adc_set_mode(uint8_t mode) {
    spi_begin();
    // Select chip
    gpio_bit_reset(GPIOA, CS_PIN);
    if (mode != ADC_MODE_CONTINUOUS) {
        // Issue SDATAC command
        spi_transfer(DEVICE_COMMAND_SDATAC);
    }
    // Issue RDATAC command
    spi_transfer(DEVICE_COMMAND_RDATAC);
    // Deselect chip
    gpio_bit_set(GPIOA, CS_PIN);
    spi_end();
}
