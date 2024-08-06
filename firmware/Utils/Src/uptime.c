#include "Utils/Inc/uptime.h"

volatile uint64_t uptime_ms = 0;

void HAL_IncTick(void) {
    uwTick += uwTickFreq;
    uptime_ms += uwTickFreq;
}

int64_t mcu_utils_uptime_ms(void) {
    return uptime_ms;
}
