#ifndef __LSM6DSR_REGS_OUTX_XL_H
#define __LSM6DSR_REGS_OUTX_XL_H

#include <stdint.h>

#include "User/Inc/lsm6dsr/reg_map.h"
#include "User/Inc/lsm6dsr/utils.h"

typedef struct {
    // X-axis linear acceleration value (LSbyte)
    uint8_t outx_l_xl;
    // X-axis linear acceleration value (MSbyte)
    uint8_t outx_h_xl;
} lsm6dsr_reg_outx_xl_t;

void lsm6dsr_reg_get_outx_xl(lsm6dsr_reg_outx_xl_t* outx_xl);

#endif
