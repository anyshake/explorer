#include "utils/crc32.h"

void mcu_utils_crc32_init(void) {
    ;
}

uint32_t mcu_utils_crc32_get(uint32_t* data, uint16_t size) {
    uint32_t crc = 0xFFFFFFFF;

    for (uint16_t i = 0; i < size; i++) {
        uint32_t v = data[i];
        for (uint8_t j = 0; j < 4; j++) {
            uint8_t byte = (v >> (24 - j * 8)) & 0xFF;
            crc ^= (uint32_t)byte << 24;
            for (uint8_t k = 0; k < 8; k++) {
                if (crc & 0x80000000) {
                    crc = (crc << 1) ^ 0x04C11DB7;
                } else {
                    crc <<= 1;
                }
            }
        }
    }

    return crc;
}
