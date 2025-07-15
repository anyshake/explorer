#include "Utils/Inc/uptime.h"

volatile static mcu_uptime_t userspace_uptime = {0};

static inline void mcu_uptime_inc(volatile mcu_uptime_t* ut, uint16_t step_us) {
    uint32_t prev_us = ut->tick_us;
    ut->tick_us += step_us;
    if (ut->tick_us < prev_us) {
        ut->overflow += 1;
    }
}

void HAL_IncUptime(void) {
    mcu_uptime_inc(&userspace_uptime, MCU_UTILS_UPTIME_TICK_STEP_US);
}

static inline uint64_t mcu_uptime_get_tick_us(volatile mcu_uptime_t* ut) {
    uint32_t lo, hi;
    do {
        hi = ut->overflow;
        lo = ut->tick_us;
    } while (hi != ut->overflow);
    return ((uint64_t)hi << 32) + lo;
}

int64_t mcu_utils_uptime_get_us(void) {
    return mcu_uptime_get_tick_us(&userspace_uptime);
}

int64_t mcu_utils_uptime_get_ms(void) {
    return (int64_t)mcu_uptime_get_tick_us(&userspace_uptime) / 1000;
}
