#include "Utils/Inc/uptime.h"

volatile static uint64_t userspace_uptime __attribute__((aligned(8)));

void HAL_IncUptime(void) {
    userspace_uptime += MCU_UTILS_UPTIME_TICK_STEP_US;
}

int64_t mcu_utils_uptime_get_us(void) {
    uint64_t a;
    uint64_t b;

    do {
        a = userspace_uptime;
        b = userspace_uptime;
    } while (a != b);

    return (int64_t)a;
}

int64_t mcu_utils_uptime_get_ms(void) {
    return mcu_utils_uptime_get_us() / 1000;
}
