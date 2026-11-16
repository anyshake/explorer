#if defined(USE_BOOTLOADER)

#include "Utils/Inc/bootloader.h"

static const uint8_t dfu_frame[] = MCU_UTILS_BOOTLOADER_DFU_FRAME;
static uint8_t dfu_frame_matched = 0;

void mcu_utils_bootloader_feed(uint8_t ch) {
    if (ch == dfu_frame[dfu_frame_matched]) {
        dfu_frame_matched++;
    } else {
        // The sync byte 0xA5 occurs only at the start of the frame, so
        // restarting from the current byte is enough (no KMP table needed).
        dfu_frame_matched = ch == dfu_frame[0] ? 1 : 0;
    }

    if (dfu_frame_matched == sizeof(dfu_frame)) {
        mcu_utils_bootloader_enter_dfu();
    }
}

void mcu_utils_bootloader_enter_dfu(void) {
    volatile uint32_t* mailbox = (volatile uint32_t*)MCU_UTILS_BOOTLOADER_MAILBOX_ADDR;

    __disable_irq();
    mailbox[0] = MCU_UTILS_BOOTLOADER_DFU_REQUEST;
    mailbox[1] = ~MCU_UTILS_BOOTLOADER_DFU_REQUEST;
    __DSB();
    NVIC_SystemReset();
}

#endif
