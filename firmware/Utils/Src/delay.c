#include "Utils/Inc/delay.h"

void mcu_utils_delay_ms(uint32_t ms, bool is_rtos) {
    if (is_rtos) {
        osDelay(ms);
    } else {
        HAL_Delay(ms);
    }
}
