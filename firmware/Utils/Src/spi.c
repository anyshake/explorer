#include "Utils/Inc/spi.h"

void mcu_utils_spi_init(bool is_rtos) {
    MX_SPI1_Init();
    mcu_utils_delay_ms(100, is_rtos);
}

void mcu_utils_spi_end(void) {
    HAL_SPI_DeInit(&hspi1);
}

uint8_t mcu_utils_spi_transfer(uint8_t tx_data) {
    uint8_t rx_data = 0;
    HAL_SPI_TransmitReceive(&hspi1, &tx_data, &rx_data, 1, 1000);
    return rx_data;
}
