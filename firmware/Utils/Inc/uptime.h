#ifndef __ANYSHAKE_MCU_UTILS_UPTIME_H
#define __ANYSHAKE_MCU_UTILS_UPTIME_H

#include <stdint.h>

#include "Core/Inc/main.h"
#include "Core/Inc/tim.h"
#include "cmsis_os2.h"

#define MCU_UTILS_UPTIME_TICK_STEP_US 50

int64_t mcu_utils_uptime_get_us(void);
int64_t mcu_utils_uptime_get_ms(void);

#endif
