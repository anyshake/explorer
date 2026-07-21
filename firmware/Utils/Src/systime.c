#include "Utils/Inc/systime.h"

typedef struct {
    uint64_t tick;
    int64_t time_ns;
    uint64_t tick_remainder;
} systime_base_t;

static volatile systime_base_t systime_base;
static volatile uint64_t timer_freq_hz;
static uint32_t nominal_timer_freq_hz;
static volatile uint64_t timer_overflow_count;
static volatile int64_t unix_offset_us;

static uint32_t mcu_utils_get_timer_clock_hz(void) {
    RCC_ClkInitTypeDef clkconfig;
    uint32_t flash_latency;

    HAL_RCC_GetClockConfig(&clkconfig, &flash_latency);

    uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();
    if (clkconfig.APB2CLKDivider == RCC_HCLK_DIV1) {
        return pclk2;
    }

    return pclk2 * 2;
}

void mcu_utils_systime_init(void) {
    MX_TIM1_Init();
    MX_TIM2_Init();

    systime_base.tick = 0;
    systime_base.tick_remainder = 0;
    systime_base.time_ns = 0;
    nominal_timer_freq_hz = mcu_utils_get_timer_clock_hz();
    timer_freq_hz = nominal_timer_freq_hz;
    timer_overflow_count = 0;
    unix_offset_us = 0;

    __HAL_TIM_SET_COUNTER(&htim1, 0);
    __HAL_TIM_SET_COUNTER(&htim2, 0);

    __HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_UPDATE);
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);

    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_Base_Start_IT(&htim2);
}

void mcu_utils_systime_handle_timer_overflow(void) {
    timer_overflow_count++;
}

uint32_t mcu_utils_systime_get_nominal_freq_hz(void) {
    return nominal_timer_freq_hz;
}

void mcu_utils_systime_calib_freq(uint32_t actual_freq_hz) {
    if (actual_freq_hz == 0) {
        return;
    }
    uint64_t tick = mcu_utils_systime_get_tick();

    __disable_irq();

    uint64_t delta_ticks = tick - systime_base.tick;
    uint64_t quotient = delta_ticks / timer_freq_hz;
    uint64_t rem_ticks = delta_ticks % timer_freq_hz;
    int64_t delta_ns = (int64_t)(quotient * 1000000000ULL);
    uint64_t numerator = (rem_ticks * 1000000000ULL) + systime_base.tick_remainder;

    delta_ns += (int64_t)(numerator / timer_freq_hz);
    uint64_t new_remainder = numerator % timer_freq_hz;

    systime_base.time_ns += delta_ns;
    systime_base.tick_remainder = new_remainder;
    systime_base.tick = tick;

    timer_freq_hz = actual_freq_hz;

    __enable_irq();
}

uint64_t mcu_utils_systime_get_tick(void) {
    uint64_t overflow1, overflow2;
    uint32_t high1, high2;
    uint32_t low;

    while (1) {
        __disable_irq();
        overflow1 = timer_overflow_count;
        high1 = TIM2->CNT;
        low = TIM1->CNT;
        high2 = TIM2->CNT;
        overflow2 = timer_overflow_count;
        __enable_irq();

        if (high1 == high2 && overflow1 == overflow2) {
            break;
        }
    }

    return (overflow1 << 32) | ((uint64_t)high1 << 16) | low;
}

int64_t mcu_utils_systime_get_uptime_us(void) {
    uint64_t tick = mcu_utils_systime_get_tick();

    __disable_irq();
    uint64_t base_tick = systime_base.tick;
    int64_t base_ns = systime_base.time_ns;
    uint64_t base_rem = systime_base.tick_remainder;
    uint32_t freq = timer_freq_hz;
    __enable_irq();

    uint64_t delta_ticks = tick - base_tick;
    uint64_t quotient = delta_ticks / freq;
    uint64_t rem_ticks = delta_ticks % freq;
    int64_t delta_ns = (int64_t)(quotient * 1000000000ULL);
    uint64_t numerator = (rem_ticks * 1000000000ULL) + base_rem;
    delta_ns += (int64_t)(numerator / freq);

    return (base_ns + delta_ns) / 1000;
}

int64_t mcu_utils_systime_get_uptime_ms(void) {
    return mcu_utils_systime_get_uptime_us() / 1000;
}

int64_t mcu_utils_systime_get_unix_time_us(void) {
    int64_t uptime;
    int64_t offset;

    __disable_irq();
    offset = unix_offset_us;
    __enable_irq();

    uptime = mcu_utils_systime_get_uptime_us();
    return uptime + offset;
}

int64_t mcu_utils_systime_get_unix_time_ms(void) {
    return mcu_utils_systime_get_unix_time_us() / 1000;
}

void mcu_utils_systime_set_offset(int64_t offset_us) {
    __disable_irq();
    unix_offset_us = offset_us;
    __enable_irq();
}
