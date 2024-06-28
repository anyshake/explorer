#ifndef __MCU_UTILS_DELAY__
#define __MCU_UTILS_DELAY__

#include <Arduino.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

void mcu_utils_delay_ms(uint32_t ms, uint8_t rtos);

#endif
