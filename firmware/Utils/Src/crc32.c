#include "Utils/Inc/crc32.h"

void mcu_utils_crc32_init(void) {
    MX_CRC_Init();
}

uint32_t mcu_utils_crc32_get(uint32_t* data, uint16_t size) {
    return HAL_CRC_Calculate(&hcrc, data, size);
}
