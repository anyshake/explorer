#include "utils/uptime.h"

int64_t mcu_utils_uptime_ms(void) {
    static uint32_t low32 = 0;
    static uint32_t high32 = 0;

    uint32_t new_low32 = millis();
    if (new_low32 < low32) {
        high32++;
    }
    low32 = new_low32;

    return (uint64_t)high32 << 32 | low32;
}
