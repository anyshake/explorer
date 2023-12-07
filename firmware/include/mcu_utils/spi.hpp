#ifndef __MCU_UTILS_SPI_HPP__
#define __MCU_UTILS_SPI_HPP__

#include <SPI.h>
#include <stdint.h>

#include "settings.hpp"

void spi_init();
uint8_t spi_write(uint8_t tx_data);

#endif
