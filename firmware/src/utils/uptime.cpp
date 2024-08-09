#include "utils/uptime.h"

int64_t mcu_utils_uptime_ms(void) {
    return millis();
}
