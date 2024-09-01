#ifndef __ANYSHAKE_MCU_UTILS_DELAY_H
#define __ANYSHAKE_MCU_UTILS_DELAY_H

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

void mcu_utils_delay_ms(uint32_t ms, bool is_rtos);

#endif
