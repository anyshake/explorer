#ifndef __LSM6DS3_REGS_OUTZ_XL_H
#define __LSM6DS3_REGS_OUTZ_XL_H

#include <stdint.h>

#include "User/Inc/lsm6ds3/reg_map.h"
#include "User/Inc/lsm6ds3/utils.h"

typedef struct {
    // Z-axis linear acceleration value (LSbyte)
    uint8_t outz_l_xl;
    // Z-axis linear acceleration value (MSbyte)
    uint8_t outz_h_xl;
} lsm6ds3_reg_outz_xl_t;

void lsm6ds3_reg_get_outz_xl(lsm6ds3_reg_outz_xl_t* outz_xl);

#endif
