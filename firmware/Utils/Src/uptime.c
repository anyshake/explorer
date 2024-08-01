#include "Utils/Inc/uptime.h"

static uint32_t overflow_count = 0;
static uint32_t last_tick = 0;

int64_t mcu_utils_uptime_ms(void) {
    uint32_t current_tick = HAL_GetTick();

    if (current_tick < last_tick) {
        overflow_count++;
    }
    last_tick = current_tick;

    int64_t total_uptime = overflow_count;
    total_uptime = (total_uptime << 32) | current_tick;
    return total_uptime;
}