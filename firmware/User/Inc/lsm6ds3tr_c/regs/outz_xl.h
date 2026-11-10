#ifndef __LSM6DS3TR_C_REGS_OUTZ_XL_H
#define __LSM6DS3TR_C_REGS_OUTZ_XL_H

#include <stdint.h>

#include "User/Inc/lsm6ds3tr_c/reg_map.h"
#include "User/Inc/lsm6ds3tr_c/utils.h"

typedef struct {
    // [7:0] Z-axis linear acceleration value (LSB)
    uint8_t outz_l_xl;

    // [7:0] Z-axis linear acceleration value (MSB)
    uint8_t outz_h_xl;
} lsm6ds3tr_c_reg_outz_xl_t;

void lsm6ds3tr_c_reg_get_outz_xl(lsm6ds3tr_c_reg_outz_xl_t* outz_xl);

#endif
