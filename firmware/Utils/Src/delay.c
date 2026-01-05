#include "Utils/Inc/delay.h"

void HAL_Delay(uint32_t ms) {
    for (uint32_t i = HAL_GetTick(); HAL_GetTick() - i < ms;) {
        ;
    }
}

void mcu_utils_delay_ms(uint32_t ms, bool is_rtos) {
    if (is_rtos) {
        osDelay(ms);
    } else {
        HAL_Delay(ms);
    }
}
