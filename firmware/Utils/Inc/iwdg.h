#ifndef __ANYSHAKE_MCU_UTILS_IWDG_H
#define __ANYSHAKE_MCU_UTILS_IWDG_H

#include "Core/Inc/iwdg.h"
#include "Utils/Inc/delay.h"
#include "stm32f1xx_hal_iwdg.h"

void mcu_utils_iwdg_init(void);
void mcu_utils_iwdg_feed(void);

#endif
