#include "User/Inc/boot.h"

volatile uint32_t boot_ms;

static volatile uint32_t* const mailbox = (volatile uint32_t*)BOOT_MAILBOX_ADDR;

static void mailbox_write(uint32_t magic) {
    mailbox[0] = magic;
    mailbox[1] = ~magic;
}

static bool mailbox_is(uint32_t magic) {
    return mailbox[0] == magic && mailbox[1] == ~magic;
}

void boot_feed_watchdog(void) {
    // The IWDG may have been started by the firmware (or by the hardware
    // watchdog option bit). Reloading a stopped IWDG is harmless.
    // Same as LL_IWDG_ReloadCounter(); the bootloader does not enable the
    // IWDG in CubeMX (that would also start it), so stm32f1xx_ll_iwdg.h is
    // not part of its driver set.
    WRITE_REG(IWDG->KR, 0xAAAAU);
}

bool boot_vectors_are_valid(uint32_t sp, uint32_t image_size) {
    if (sp <= BOOT_RAM_BASE || sp > BOOT_MAILBOX_ADDR || (sp & 3U) != 0 ||
        image_size < BOOT_VECTOR_WORDS * 4U || image_size > BOOT_APP_MAX_SIZE) {
        return false;
    }
    const volatile uint32_t* vectors = (const volatile uint32_t*)BOOT_APP_BASE;
    for (uint32_t i = 1; i < BOOT_VECTOR_WORDS; i++) {
        uint32_t pc = vectors[i];
        if (pc == 0 && i != 1) {  // unused/reserved vectors
            continue;
        }
        uint32_t addr = pc & ~1U;
        if ((pc & 1U) == 0 || addr < BOOT_APP_BASE + BOOT_VECTOR_WORDS * 4U ||
            addr >= BOOT_APP_BASE + image_size) {
            return false;
        }
    }
    return true;
}

bool boot_app_is_valid(void) {
    const volatile uint32_t* vectors = (const volatile uint32_t*)BOOT_APP_BASE;
    // The initial SP word is the last word written by the DFU procedure,
    // so an interrupted update always leaves it erased (0xFFFFFFFF).
    return boot_vectors_are_valid(vectors[0], BOOT_APP_MAX_SIZE);
}

boot_reason_t boot_take_request(void) {
    bool requested = mailbox_is(BOOT_MAILBOX_DFU_REQUEST);
    mailbox_write(0);
    return requested ? BOOT_REASON_REQUESTED : BOOT_REASON_NONE;
}

void boot_jump_if_requested(void) {
    if (!mailbox_is(BOOT_MAILBOX_RUN_APP)) {
        return;
    }
    mailbox_write(0);
    if (!boot_app_is_valid()) {
        return;
    }

    // Called first thing in main(): clocks, peripherals and NVIC are still
    // in their reset state, so the application starts as after a power-on.
    const volatile uint32_t* vectors = (const volatile uint32_t*)BOOT_APP_BASE;
    // No LL API for VTOR; CMSIS core register.
    WRITE_REG(SCB->VTOR, BOOT_APP_BASE);
    __DSB();
    __ISB();
    __set_MSP(vectors[0]);
    // The reset vector is read from flash after the stack switch, so nothing
    // kept on the bootloader's stack is needed any more. Reset_Handler never
    // returns.
    ((void (*)(void))vectors[1])();
    while (1) {
        ;
    }
}

void boot_start_app(void) {
    // Go through a reset instead of jumping directly, so that nothing the
    // bootloader configured (PLL, SysTick, USART) leaks into the application.
    mailbox_write(BOOT_MAILBOX_RUN_APP);
    NVIC_SystemReset();
}
