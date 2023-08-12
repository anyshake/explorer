#include "ads1256/reader.h"

int32_t adc_get_one_shot() {
    adc_wait();
    spi_begin();

    // CS must stay LOW during the entire
    gpio_bit_reset(GPIOA, CS_PIN);
    // Issue RDATA command
    spi_transfer(DEVICE_COMMAND_RDATA);
    delayMicroseconds(10);

    // Get the 24-bit result
    int32_t result = (int32_t)spi_transfer(DEVICE_COMMAND_SDATAC) << 16 |
                     (int32_t)spi_transfer(DEVICE_COMMAND_SDATAC) << 8 |
                     (int32_t)spi_transfer(DEVICE_COMMAND_SDATAC);

    // Deselect the chip
    gpio_bit_set(GPIOA, CS_PIN);
    spi_end();

    return result;

}

int32_t adc_get_continuous() {
    adc_wait();
    spi_begin();

    // CS must stay LOW during the entire
    gpio_bit_reset(GPIOA, CS_PIN);

    // Get the 24-bit result
    int32_t result = (int32_t)spi_transfer(DEVICE_COMMAND_SDATAC) << 16 |
                     (int32_t)spi_transfer(DEVICE_COMMAND_SDATAC) << 8 |
                     (int32_t)spi_transfer(DEVICE_COMMAND_SDATAC);

    // Deselect the chip
    gpio_bit_set(GPIOA, CS_PIN);
    spi_end();

    return result;
}

int32_t adc_get_differential(uint8_t positiveCh,
                             uint8_t negativeCh) {
    adc_wait();
    spi_begin();

    // CS must stay LOW during the entire
    gpio_bit_reset(GPIOA, CS_PIN);
    spi_transfer(DEVICE_COMMAND_WREG | ADC_REGISTER_MUX);
    spi_transfer(0x00);

    // Write the positive and negative channels to MUX
    spi_transfer((positiveCh << 4) | negativeCh);

    // Request a conversion
    spi_transfer(DEVICE_COMMAND_SYNC);
    delayMicroseconds(4);
    spi_transfer(DEVICE_COMMAND_WAKEUP);
    spi_transfer(DEVICE_COMMAND_RDATA);
    delayMicroseconds(5);

    // Get the 24-bit result
    int32_t result = (int32_t)spi_transfer(DEVICE_COMMAND_SDATAC) << 16 |
                     (int32_t)spi_transfer(DEVICE_COMMAND_SDATAC) << 8 |
                     (int32_t)spi_transfer(DEVICE_COMMAND_SDATAC);

    // Deselect the chip
    gpio_bit_set(GPIOA, CS_PIN);
    spi_end();

    return result;
}
