#ifndef __UTILS_BOOTLOADER_H
#define __UTILS_BOOTLOADER_H

#include <stdint.h>

#include "stm32f1xx_hal.h"

#define MCU_UTILS_BOOTLOADER_MAILBOX_ADDR 0x20004FF0U
#define MCU_UTILS_BOOTLOADER_DFU_REQUEST 0x21554644U
#define MCU_UTILS_BOOTLOADER_DFU_FRAME { \
    0xA5,                                \
    0x06,                                \
    0x00,                                \
    0x00,                                \
    0xB8,                                \
    0x5F,                                \
    0x38,                                \
    0xC4,                                \
}

void mcu_utils_bootloader_feed(uint8_t ch);
void mcu_utils_bootloader_enter_dfu(void);

#endif
