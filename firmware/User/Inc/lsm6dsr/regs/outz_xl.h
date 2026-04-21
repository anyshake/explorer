#ifndef __LSM6DSR_REGS_OUTZ_XL_H
#define __LSM6DSR_REGS_OUTZ_XL_H

#include <stdint.h>

#include "User/Inc/lsm6dsr/reg_map.h"
#include "User/Inc/lsm6dsr/utils.h"

typedef struct {
    // Z-axis linear acceleration value (LSbyte)
    uint8_t outz_l_xl;
    // Z-axis linear acceleration value (MSbyte)
    uint8_t outz_h_xl;
} lsm6dsr_reg_outz_xl_t;

void lsm6dsr_reg_get_outz_xl(lsm6dsr_reg_outz_xl_t* outz_xl);

#endif
