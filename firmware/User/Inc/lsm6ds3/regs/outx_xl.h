#ifndef __LSM6DS3_REGS_OUTX_XL_H
#define __LSM6DS3_REGS_OUTX_XL_H

#include <stdint.h>

#include "User/Inc/lsm6ds3/reg_map.h"
#include "User/Inc/lsm6ds3/utils.h"

typedef struct {
    // [7:0] X-axis linear acceleration value (LSB)
    uint8_t outx_l_xl;

    // [7:0] X-axis linear acceleration value (MSB)
    uint8_t outx_h_xl;
} lsm6ds3_reg_outx_xl_t;

void lsm6ds3_reg_get_outx_xl(lsm6ds3_reg_outx_xl_t* outx_xl);

#endif
