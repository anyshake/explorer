#ifndef __LSM6DS3_REGS_WHO_AM_I_H
#define __LSM6DS3_REGS_WHO_AM_I_H

#include <stdint.h>

#include "User/Inc/lsm6ds3/reg_map.h"
#include "User/Inc/lsm6ds3/utils.h"

#define LSM6DS3_WHO_AM_I_VALUE 0x69

// Register reset value
#define __LSM6DS3_WHO_AM_I_DEFAULT_VALUE 0x69

typedef struct {
    // [7:0] Device ID, read-only
    // 0x69: LSM6DS3
    uint8_t dev_id;
} lsm6ds3_reg_who_am_i_t;

void lsm6ds3_reg_get_who_am_i(lsm6ds3_reg_who_am_i_t* who_am_i);

#endif
