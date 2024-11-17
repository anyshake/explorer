#ifndef __ANYSHAKE_MCU_UTILS_IWDG_H
#define __ANYSHAKE_MCU_UTILS_IWDG_H

#include <stdbool.h>

#include "Core/Inc/iwdg.h"
#include "Utils/Inc/delay.h"
#include "stm32f1xx_hal.h"

void mcu_utils_iwdg_init(bool is_rtos);
void mcu_utils_iwdg_feed(void);

#endif
