#include "mcu_utils/delay.hpp"

void mcu_utils_delay_ms(uint32_t ms, uint8_t rtos) {
    if (rtos) {
        vTaskDelay(ms / portTICK_PERIOD_MS);
    } else {
        delay(ms);
    }
}
