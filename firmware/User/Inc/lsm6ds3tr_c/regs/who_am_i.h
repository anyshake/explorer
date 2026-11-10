#ifndef __LSM6DS3TR_C_REGS_WHO_AM_I_H
#define __LSM6DS3TR_C_REGS_WHO_AM_I_H

#include <stdint.h>

#include "User/Inc/lsm6ds3tr_c/reg_map.h"
#include "User/Inc/lsm6ds3tr_c/utils.h"

#define LSM6DS3TR_C_WHO_AM_I_VALUE 0x6A

// Register reset value
#define __LSM6DS3TR_C_WHO_AM_I_DEFAULT_VALUE 0x6A

typedef struct {
    // [7:0] Device ID, read-only: 0x6A for LSM6DS3TR-C
    uint8_t dev_id;
} lsm6ds3tr_c_reg_who_am_i_t;

void lsm6ds3tr_c_reg_get_who_am_i(lsm6ds3tr_c_reg_who_am_i_t* who_am_i);

#endif
