#ifndef APP_SPI_H
#define APP_SPI_H

#include <stdint.h>
#include "gd32f30x_spi.h"

#include "delay.h"

#define SPI_SCK GPIO_PIN_5
#define SPI_MISO GPIO_PIN_6
#define SPI_MOSI GPIO_PIN_7

void spi_begin(void);
void spi_end(void);
uint8_t spi_transfer(uint8_t dat);

#endif
