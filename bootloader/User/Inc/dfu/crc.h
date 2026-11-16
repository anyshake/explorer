#ifndef __USER_DFU_CRC_H
#define __USER_DFU_CRC_H

#include <stdint.h>

#include "User/Inc/boot.h"

uint32_t dfu_crc_words(uint32_t first, const uint32_t* words, uint32_t count);

#endif
