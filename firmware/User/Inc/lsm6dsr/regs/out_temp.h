#ifndef __LSM6DSR_REGS_OUT_TEMP_H
#define __LSM6DSR_REGS_OUT_TEMP_H

#include <stdint.h>

#include "User/Inc/lsm6dsr/reg_map.h"
#include "User/Inc/lsm6dsr/utils.h"

typedef struct {
    // Temperature output (LSbyte)
    uint8_t out_temp_l;
    // Temperature output (MSbyte)
    uint8_t out_temp_h;
} lsm6dsr_reg_out_temp_t;

void lsm6dsr_reg_get_out_temp(lsm6dsr_reg_out_temp_t* out_temp);

#endif
