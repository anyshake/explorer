#ifndef __LSM6DSR_REGS_OUTY_XL_H
#define __LSM6DSR_REGS_OUTY_XL_H

#include <stdint.h>

#include "User/Inc/lsm6dsr/reg_map.h"
#include "User/Inc/lsm6dsr/utils.h"

typedef struct {
    // [7:0] Y-axis linear acceleration value (LSB)
    uint8_t outy_l_xl;

    // [7:0] Y-axis linear acceleration value (MSB)
    uint8_t outy_h_xl;
} lsm6dsr_reg_outy_xl_t;

void lsm6dsr_reg_get_outy_xl(lsm6dsr_reg_outy_xl_t* outy_xl);

#endif
