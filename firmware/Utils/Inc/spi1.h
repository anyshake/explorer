#ifndef __ANYSHAKE_MCU_UTILS_SPI1_H
#define __ANYSHAKE_MCU_UTILS_SPI1_H

#include <stdbool.h>
#include <stdint.h>

#include "Core/Inc/spi.h"
#include "Utils/Inc/delay.h"
#include "stm32f1xx_hal_spi.h"

void mcu_utils_spi1_init(bool is_rtos);
void mcu_utils_spi1_end(void);
uint8_t mcu_utils_spi1_transfer(uint8_t tx_data);

#endif
