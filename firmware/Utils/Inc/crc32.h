#ifndef __ANYSHAKE_MCU_UTILS_CRC32_H
#define __ANYSHAKE_MCU_UTILS_CRC32_H

#include <stdint.h>

#include "Core/Inc/crc.h"
#include "stm32f1xx_hal.h"

void mcu_utils_crc32_init(void);
uint32_t mcu_utils_crc32_get(uint32_t* data, uint16_t size);

#endif
