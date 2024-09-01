#ifndef __ANYSHAKE_MCU_UTILS_SPI_H
#define __ANYSHAKE_MCU_UTILS_SPI_H

#include <SPI.h>
#include <stdbool.h>
#include <stdint.h>

#include "utils/delay.h"
#include "utils/gpio.h"

static const mcu_utils_gpio_t SPI_SS = {
    .pin = 16,
};

void mcu_utils_spi_init(bool is_rtos);
void mcu_utils_spi_end(void);
uint8_t mcu_utils_spi_transfer(uint8_t tx_data);

#endif
