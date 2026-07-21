#include "User/Inc/gnss/pps.h"

static gnss_pps_state_t gnss_pps_state;

static void gnss_pps_reset_buffer(void) {
    gnss_pps_state.valid_pps_count = 0;
    gnss_pps_state.tick_index = 0;
    gnss_pps_state.tick_count = 0;
    gnss_pps_state.filtered_freq = 0;
    gnss_pps_state.has_freq_filter = false;

    for (uint8_t i = 0; i < GNSS_PPS_FREQ_WINDOW; i++) {
        gnss_pps_state.tick_buf[i] = 0;
    }
}

void gnss_pps_init(void) {
    gnss_pps_state.enabled = true;
    gnss_pps_state.has_last_hal_tick = false;
    gnss_pps_state.last_hal_tick = 0;
    gnss_pps_state.has_last_tick = false;
    gnss_pps_state.last_tick = 0;
    gnss_pps_reset_buffer();
}

void gnss_pps_end(void) {
    gnss_pps_state.enabled = false;
    gnss_pps_state.has_last_hal_tick = false;
    gnss_pps_state.last_hal_tick = 0;
    gnss_pps_state.has_last_tick = false;
    gnss_pps_state.last_tick = 0;
    gnss_pps_reset_buffer();
}

void gnss_pps_handle_timer_interrupt(timestamp_callback_fn_t cb) {
    if (!gnss_pps_state.enabled) {
        return;
    }

    int64_t local_tick = mcu_utils_systime_get_tick();
    int64_t monotonic_time_us = mcu_utils_systime_get_uptime_us();
    uint32_t hal_tick = HAL_GetTick();

    uint32_t actual_clock_freq = 0;
    bool valid_1pps = false;

    if (gnss_pps_state.has_last_tick && gnss_pps_state.has_last_hal_tick) {
        uint32_t hal_diff = hal_tick - gnss_pps_state.last_hal_tick;
        if (hal_diff >= 990 && hal_diff <= 1010) {
            valid_1pps = true;
        }
    }

    if (valid_1pps) {
        if (gnss_pps_state.valid_pps_count < GNSS_PPS_VALID_VERIFY) {
            gnss_pps_state.valid_pps_count++;
        }

        if (gnss_pps_state.valid_pps_count >= GNSS_PPS_VALID_VERIFY) {
            if (gnss_pps_state.tick_count == GNSS_PPS_FREQ_WINDOW) {
                int64_t oldest = gnss_pps_state.tick_buf[gnss_pps_state.tick_index];
                int64_t delta_tick = local_tick - oldest;
                uint32_t measured_freq = (uint32_t)(delta_tick / GNSS_PPS_FREQ_WINDOW);
                if (!gnss_pps_state.has_freq_filter) {
                    gnss_pps_state.filtered_freq = measured_freq;
                    gnss_pps_state.has_freq_filter = true;
                } else {
                    gnss_pps_state.filtered_freq += ((int64_t)measured_freq - (int64_t)gnss_pps_state.filtered_freq) >> GNSS_PPS_FREQ_FILTER;
                }
                actual_clock_freq = (uint32_t)gnss_pps_state.filtered_freq;
            }

            gnss_pps_state.tick_buf[gnss_pps_state.tick_index] = local_tick;
            gnss_pps_state.tick_index++;

            if (gnss_pps_state.tick_index >= GNSS_PPS_FREQ_WINDOW) {
                gnss_pps_state.tick_index = 0;
            }

            if (gnss_pps_state.tick_count < GNSS_PPS_FREQ_WINDOW) {
                gnss_pps_state.tick_count++;
            }
        }
    } else {
        gnss_pps_reset_buffer();
        gnss_pps_state.last_tick = local_tick;
        gnss_pps_state.last_hal_tick = hal_tick;
        gnss_pps_state.has_last_tick = true;
        gnss_pps_state.has_last_hal_tick = true;
    }

    gnss_pps_state.last_tick = local_tick;
    gnss_pps_state.last_hal_tick = hal_tick;

    gnss_pps_state.has_last_tick = true;
    gnss_pps_state.has_last_hal_tick = true;

    if (cb && actual_clock_freq != 0) {
        cb(monotonic_time_us, actual_clock_freq);
    }
}
