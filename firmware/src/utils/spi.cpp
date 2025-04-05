#include "utils/spi.h"

void mcu_utils_spi_init(bool is_rtos) {
    mcu_utils_gpio_mode(SPI_SS, OUTPUT);
    mcu_utils_gpio_high(SPI_SS);

    SPI.begin();
    SPI.setHwCs(false);
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE2));

    mcu_utils_delay_ms(100, is_rtos);
}

void mcu_utils_spi_end(void) {
    SPI.end();
}

uint8_t mcu_utils_spi_transfer(uint8_t tx_data) {
    uint8_t rx_data = SPI.transfer(tx_data);
    return rx_data;
}
