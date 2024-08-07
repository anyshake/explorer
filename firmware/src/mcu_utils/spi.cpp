#include "mcu_utils/spi.hpp"

void spi_init() {
    SPI.begin();
    SPI.beginTransaction(SPISettings(MCU_SPI_CLK, MCU_SPI_ORDER, MCU_SPI_MODE));
}

uint8_t spi_write(uint8_t tx_data) {
    return SPI.transfer(tx_data);
}
