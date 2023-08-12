#include "ads1256/utils.h"

void adc_begin() {
    // Set GPIO pin mode
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CS_PIN | RST_PIN);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, DRDY_PIN);
    // Hard reset ADS1256
    gpio_bit_reset(GPIOA, RST_PIN);
    delay(250);
    gpio_bit_set(GPIOA, RST_PIN);
    delay(250);
    // Enable calibration
    adc_write_register(ADC_REGISTER_STATUS, 0x04);
}


void adc_reset() {
    spi_begin();
    // Select chip
    gpio_bit_reset(GPIOA, CS_PIN);
    // Wait a moment
    delayMicroseconds(10);
    // Send reset command
    spi_transfer(DEVICE_COMMAND_RESET);
    // Wait for reset to complete
    delay(2);
    // Issue SDATAC
    spi_transfer(DEVICE_COMMAND_SDATAC);
    delayMicroseconds(100);
    // Deselect chip
    gpio_bit_set(GPIOA, CS_PIN);
    spi_end();
}

// Get voltage from ADC
double adc_get_voltage(int32_t value) {
    // if the 24th bit (sign) is not 0, the number is negative
    if (value >> 23) {
        // conversion for the negative sign "mirroring" around zero
        value -= 16777216;
    }

    // 2.5 = Vref; 8388607 = 2^{23} - 1
    double voltage = ((2 * VREF) / 8388607) * value;
    return voltage;
}
