#include "mcu_utils/spi.hpp"

void mcu_utils_spi_init() {
    SPI.begin();
    SPI.beginTransaction(SPISettings(MCU_UTILS_SPI_CLKDIV, MCU_UTILS_SPI_ORDER,
                                     MCU_UTILS_SPI_MODE));
}

uint8_t mcu_utils_spi_write(uint8_t tx_data) {
    return SPI.transfer(tx_data);
}
