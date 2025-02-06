#include "Utils/Inc/iwdg.h"

void mcu_utils_iwdg_init(void) {
    MX_IWDG_Init();
}

void mcu_utils_iwdg_feed(void) {
    HAL_IWDG_Refresh(&hiwdg);
}
