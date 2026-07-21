#ifndef __ANYSHAKE_MCU_UTILS_SYSTIME_H
#define __ANYSHAKE_MCU_UTILS_SYSTIME_H

#include <stdint.h>

#include "Core/Inc/tim.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"

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
