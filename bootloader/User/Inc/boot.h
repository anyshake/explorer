#ifndef __USER_BOOT_H
#define __USER_BOOT_H

#include <stdbool.h>
#include <stdint.h>

#include "stm32f1xx.h"

// Flash layout (STM32F103C8, 64 KiB, 1 KiB pages).
// Must agree with bootloader/STM32F103C8Tx_FLASH_BOOT.ld and
// firmware/STM32F103C8Tx_FLASH_APP.ld.
#define BOOT_FLASH_PAGE_SIZE 0x400U
#define BOOT_APP_BASE 0x08001000U
#define BOOT_APP_END 0x08010000U
#define BOOT_APP_MAX_SIZE (BOOT_APP_END - BOOT_APP_BASE)
// Core vectors plus IRQs through USBWakeUp; excludes BootRAM signature.
#define BOOT_VECTOR_WORDS 59U

// RAM layout: the last 16 bytes of SRAM are excluded from both linker scripts
// and survive a system reset. The firmware writes BOOT_MAILBOX_DFU_REQUEST
// there to ask the bootloader to stay in DFU mode.
#define BOOT_RAM_BASE 0x20000000U
#define BOOT_RAM_END 0x20005000U
#define BOOT_MAILBOX_ADDR 0x20004FF0U
#define BOOT_MAILBOX_DFU_REQUEST 0x21554644U  // "DFU!"
#define BOOT_MAILBOX_RUN_APP 0x21505041U      // "APP!"

typedef enum {
    BOOT_REASON_NONE = 0,
    BOOT_REASON_REQUESTED = 1,  // firmware asked for DFU over UART1
    BOOT_REASON_NO_APP = 2,     // no valid application image
    BOOT_REASON_HOST = 3,       // host caught the post-reset listen window
} boot_reason_t;

// Milliseconds since start-up, incremented by SysTick_Handler.
extern volatile uint32_t boot_ms;

void boot_feed_watchdog(void);
bool boot_app_is_valid(void);
bool boot_vectors_are_valid(uint32_t sp, uint32_t image_size);
boot_reason_t boot_take_request(void);
void boot_jump_if_requested(void);
void boot_start_app(void);

#endif
