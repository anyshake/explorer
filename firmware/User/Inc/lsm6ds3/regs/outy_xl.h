#ifndef __LSM6DS3_REGS_OUTY_XL_H
#define __LSM6DS3_REGS_OUTY_XL_H

#include <stdint.h>

#include "User/Inc/lsm6ds3/reg_map.h"
#include "User/Inc/lsm6ds3/utils.h"

typedef struct {
    // Y-axis linear acceleration value (LSbyte)
    uint8_t outy_l_xl;
    // Y-axis linear acceleration value (MSbyte)
    uint8_t outy_h_xl;
} lsm6ds3_reg_outy_xl_t;

void lsm6ds3_reg_get_outy_xl(lsm6ds3_reg_outy_xl_t* outy_xl);

#endif
