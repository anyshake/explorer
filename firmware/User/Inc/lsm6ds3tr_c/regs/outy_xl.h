#ifndef __LSM6DS3TR_C_REGS_OUTY_XL_H
#define __LSM6DS3TR_C_REGS_OUTY_XL_H

#include <stdint.h>

#include "User/Inc/lsm6ds3tr_c/reg_map.h"
#include "User/Inc/lsm6ds3tr_c/utils.h"

typedef struct {
    // [7:0] Y-axis linear acceleration value (LSB)
    uint8_t outy_l_xl;

    // [7:0] Y-axis linear acceleration value (MSB)
    uint8_t outy_h_xl;
} lsm6ds3tr_c_reg_outy_xl_t;

void lsm6ds3tr_c_reg_get_outy_xl(lsm6ds3tr_c_reg_outy_xl_t* outy_xl);

#endif
