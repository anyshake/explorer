#ifndef __USER_DFU_FLASH_H
#define __USER_DFU_FLASH_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/boot.h"

bool dfu_flash_erase_app(uint32_t size);
bool dfu_flash_program_word(uint32_t addr, uint32_t word);

#endif
