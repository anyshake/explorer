#include "User/Inc/dfu/crc.h"

static uint32_t crc_word(uint32_t crc, uint32_t word) {
    crc ^= word;
    for (uint32_t i = 0; i < 32U; i++) {
        crc = (crc & 0x80000000U) ? (crc << 1) ^ 0x04C11DB7U : crc << 1;
    }
    return crc;
}

uint32_t dfu_crc_words(uint32_t first, const uint32_t* words, uint32_t count) {
    uint32_t crc = crc_word(0xFFFFFFFFU, first);
    while (count--) {
        crc = crc_word(crc, *words++);
        if ((count & 0x3FFU) == 0) {
            boot_feed_watchdog();
        }
    }
    return crc;
}
