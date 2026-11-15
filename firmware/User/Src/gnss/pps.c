#include <string.h>
#include <stddef.h>

#include "User/Inc/gnss/pps.h"

static void gnss_pps_reset_buffer(gnss_pps_state_t* state) {
    state->valid_pps_count = 0;
    state->tick_index = 0;
    state->tick_count = 0;
    state->filtered_freq = 0;
    state->has_freq_filter = false;
    memset(state->tick_buf, 0, sizeof(state->tick_buf));
}

void gnss_pps_init(gnss_pps_state_t* state) {
    memset(state, 0, sizeof(*state));
    state->enabled = true;
}

void gnss_pps_handle_timer_interrupt(gnss_pps_state_t* state,
                                     timestamp_callback_fn_t callback) {
    if (state == NULL || !state->enabled) {
        return;
    }

    int64_t local_tick = (int64_t)mcu_utils_systime_get_tick();
    int64_t monotonic_time_us = mcu_utils_systime_get_uptime_us();
    uint32_t actual_clock_freq = 0;
    bool valid_1pps = false;

    if (state->has_last_pps) {
        // A long PPS gap must not alias across the 32-bit HAL tick wrap.
        int64_t pps_interval_us = monotonic_time_us - state->last_pps_us;
        valid_1pps = pps_interval_us >= 990 * 1000 && pps_interval_us <= 1010 * 1000;
    }

    if (valid_1pps) {
        if (state->valid_pps_count < GNSS_PPS_VALID_VERIFY) {
            state->valid_pps_count++;
        }

        if (state->valid_pps_count >= GNSS_PPS_VALID_VERIFY) {
            if (state->tick_count == GNSS_PPS_FREQ_WINDOW) {
                int64_t oldest_tick = state->tick_buf[state->tick_index];
                uint32_t measured_freq = (uint32_t)
                    ((local_tick - oldest_tick) / GNSS_PPS_FREQ_WINDOW);
                if (!state->has_freq_filter) {
                    state->filtered_freq = measured_freq;
                    state->has_freq_filter = true;
                } else {
                    state->filtered_freq +=
                        ((int64_t)measured_freq - (int64_t)state->filtered_freq) >>
                        GNSS_PPS_FREQ_FILTER;
                }
                actual_clock_freq = (uint32_t)state->filtered_freq;
            }

            state->tick_buf[state->tick_index] = local_tick;
            state->tick_index = (state->tick_index + 1) % GNSS_PPS_FREQ_WINDOW;
            if (state->tick_count < GNSS_PPS_FREQ_WINDOW) {
                state->tick_count++;
            }
        }
    } else {
        gnss_pps_reset_buffer(state);
    }

    state->last_pps_us = monotonic_time_us;
    state->has_last_pps = true;

    if (callback != NULL && actual_clock_freq != 0) {
        callback((uint64_t)monotonic_time_us, actual_clock_freq);
    }
}
