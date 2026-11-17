#ifndef __USER_GNSS_PPS_H
#define __USER_GNSS_PPS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "Utils/Inc/systime.h"

#define GNSS_PPS_VALID_VERIFY 10
#define GNSS_PPS_FREQ_WINDOW 5
#define GNSS_PPS_FREQ_FILTER 5
#define GNSS_PPS_FREQ_FRAC_BITS 8

typedef struct {
    bool enabled;
    volatile bool has_last_pps;
    volatile int64_t last_pps_us;
    uint8_t valid_pps_count;
    int64_t tick_buf[GNSS_PPS_FREQ_WINDOW];
    uint8_t tick_index;
    uint8_t tick_count;
    volatile bool has_freq_filter;
    // Hz << GNSS_PPS_FREQ_FRAC_BITS
    int64_t filtered_freq;
} gnss_pps_state_t;

typedef void (*timestamp_callback_fn_t)(uint64_t monotonic_time_us,
                                        uint32_t actual_freq_hz);

void gnss_pps_init(gnss_pps_state_t* state);
void gnss_pps_handle_timer_interrupt(gnss_pps_state_t* state,
                                     timestamp_callback_fn_t callback);

#endif
