#ifndef __LSM6DS3TR_C_REGS_OUT_TEMP_H
#define __LSM6DS3TR_C_REGS_OUT_TEMP_H

#include <stdint.h>

#include "User/Inc/lsm6ds3tr_c/reg_map.h"
#include "User/Inc/lsm6ds3tr_c/utils.h"

typedef struct {
    // [7:0] Temperature output (LSB)
    uint8_t out_temp_l;

    // [7:0] Temperature output (MSB)
    uint8_t out_temp_h;
} lsm6ds3tr_c_reg_out_temp_t;

void lsm6ds3tr_c_reg_get_out_temp(lsm6ds3tr_c_reg_out_temp_t* out_temp);

#endif
