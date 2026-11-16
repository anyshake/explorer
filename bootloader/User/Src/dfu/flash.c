#include "User/Inc/dfu/flash.h"

static bool flash_wait(void) {
    while (READ_BIT(FLASH->SR, FLASH_SR_BSY)) {
        boot_feed_watchdog();
    }
    uint32_t errors = READ_BIT(FLASH->SR, FLASH_SR_PGERR | FLASH_SR_WRPRTERR);
    WRITE_REG(FLASH->SR, FLASH_SR_EOP | FLASH_SR_PGERR | FLASH_SR_WRPRTERR);
    return errors == 0;
}

static void flash_unlock(void) {
    if (READ_BIT(FLASH->CR, FLASH_CR_LOCK)) {
        WRITE_REG(FLASH->KEYR, FLASH_KEY1);
        WRITE_REG(FLASH->KEYR, FLASH_KEY2);
    }
}

static void flash_lock(void) {
    SET_BIT(FLASH->CR, FLASH_CR_LOCK);
}

bool dfu_flash_program_word(uint32_t addr, uint32_t word) {
    volatile uint16_t* half = (volatile uint16_t*)addr;
    bool ok = true;
    flash_unlock();
    SET_BIT(FLASH->CR, FLASH_CR_PG);
    for (uint32_t i = 0; i < 2U && ok; i++) {
        uint16_t value = (uint16_t)(word >> (16U * i));
        if (half[i] != value) {
            half[i] = value;
            ok = flash_wait();
        }
    }
    CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
    flash_lock();
    return ok && *(volatile uint32_t*)addr == word;
}

bool dfu_flash_erase_app(uint32_t size) {
    bool ok = true;
    flash_unlock();
    SET_BIT(FLASH->CR, FLASH_CR_PER);
    // The vector page is erased first, so an interrupted update can never
    // leave a half-old, half-new image that looks bootable.
    for (uint32_t addr = BOOT_APP_BASE; addr < BOOT_APP_BASE + size && ok; addr += BOOT_FLASH_PAGE_SIZE) {
        WRITE_REG(FLASH->AR, addr);
        SET_BIT(FLASH->CR, FLASH_CR_STRT);
        ok = flash_wait();
    }
    CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
    flash_lock();
    return ok;
}
