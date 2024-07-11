#ifndef __MCU_UTILS_DELAY__
#define __MCU_UTILS_DELAY__

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

void mcu_utils_delay_ms(uint32_t ms, bool rtos);

#endif
