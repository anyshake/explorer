#ifndef __LSM6DS3_REGS_OUT_TEMP_H
#define __LSM6DS3_REGS_OUT_TEMP_H

#include <stdint.h>

#include "User/Inc/lsm6ds3/reg_map.h"
#include "User/Inc/lsm6ds3/utils.h"

typedef struct {
    // Temperature output (LSbyte)
    uint8_t out_temp_l;
    // Temperature output (MSbyte)
    uint8_t out_temp_h;
} lsm6ds3_reg_out_temp_t;

void lsm6ds3_reg_get_out_temp(lsm6ds3_reg_out_temp_t* out_temp);

#endif
