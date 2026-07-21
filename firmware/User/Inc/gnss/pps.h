#ifndef __GNSS_PPS_H
#define __GNSS_PPS_H

#include <stdbool.h>
#include <stdint.h>

#include "Utils/Inc/systime.h"
#include "stm32f1xx_hal.h"

#define GNSS_PPS_VALID_VERIFY 10
#define GNSS_PPS_FREQ_WINDOW 5
#define GNSS_PPS_FREQ_FILTER 5

typedef struct {
    bool enabled;

    // to prevent timestamp from leaping
    bool has_last_hal_tick;
    uint32_t last_hal_tick;
    uint8_t valid_pps_count;

    // to calculate actual clock frequency
    bool has_last_tick;
    int64_t last_tick;

    // frequency measurement buffer
    int64_t tick_buf[GNSS_PPS_FREQ_WINDOW];
    uint8_t tick_index;
    uint8_t tick_count;
    bool has_freq_filter;
    uint64_t filtered_freq;
} gnss_pps_state_t;

typedef void (*timestamp_callback_fn_t)(uint64_t monotonic_time_us, uint32_t actual_freq_hz);

void gnss_pps_init(void);
void gnss_pps_end(void);
void gnss_pps_handle_timer_interrupt(timestamp_callback_fn_t cb);

#endif
