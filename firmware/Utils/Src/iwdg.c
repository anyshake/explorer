#include "Utils/Inc/iwdg.h"

void mcu_utils_iwdg_init(bool is_rtos) {
    MX_IWDG_Init();
    mcu_utils_delay_ms(100, false);
}

void mcu_utils_iwdg_feed(void) {
    HAL_IWDG_Refresh(&hiwdg);
}
