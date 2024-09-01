#include "utils/delay.h"

void mcu_utils_delay_ms(uint32_t ms, bool is_rtos) {
    delay(ms);
}
