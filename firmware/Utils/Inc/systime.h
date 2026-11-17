#ifndef __UTILS_SYSTIME_H
#define __UTILS_SYSTIME_H

#include <stdint.h>

#include "Core/Inc/tim.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"

// Offset corrections up to the threshold are slewed at the given rate
// (100 ppm: 100 us per second); larger ones are applied at once.
#define MCU_UTILS_SYSTIME_SLEW_RATE_PPM 100
#define MCU_UTILS_SYSTIME_STEP_THRESHOLD_US 10000

void mcu_utils_systime_init(void);
void mcu_utils_systime_calib_freq(uint32_t actual_freq_hz);
void mcu_utils_systime_handle_timer_overflow(void);
uint32_t mcu_utils_systime_get_nominal_freq_hz(void);
uint64_t mcu_utils_systime_get_tick(void);
int64_t mcu_utils_systime_get_uptime_us(void);
int64_t mcu_utils_systime_get_uptime_ms(void);
int64_t mcu_utils_systime_get_unix_time_us(void);
int64_t mcu_utils_systime_get_unix_time_ms(void);
void mcu_utils_systime_set_offset(int64_t offset_us);

#endif
