#ifndef __ANYSHAKE_MCU_UTILS_DELAY_H
#define __ANYSHAKE_MCU_UTILS_DELAY_H

#include <stdbool.h>
#include <stdint.h>

#include "cmsis_os.h"
#include "stm32f1xx_hal.h"

void mcu_utils_delay_ms(uint32_t ms, bool is_rtos);

#endif
