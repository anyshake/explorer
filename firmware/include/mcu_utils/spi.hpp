#ifndef __MCU_UTILS_SPI_HPP__
#define __MCU_UTILS_SPI_HPP__

#include <SPI.h>
#include <stdint.h>

#define MCU_UTILS_SPI_CLKDIV SPI_CLOCK_DIV2
#define MCU_UTILS_SPI_ORDER MSBFIRST
#define MCU_UTILS_SPI_MODE SPI_MODE2
#define MCU_UTILS_SPI_SS PIN_SPI_SS

void mcu_utils_spi_init();
uint8_t mcu_utils_spi_write(uint8_t tx_data);

#endif
