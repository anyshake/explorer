#ifndef __LSM6DSR_REGS_WHO_AM_I_H
#define __LSM6DSR_REGS_WHO_AM_I_H

#include <stdint.h>

#include "User/Inc/lsm6dsr/reg_map.h"
#include "User/Inc/lsm6dsr/utils.h"

#define LSM6DSR_WHO_AM_I_VALUE 0x6B

// Register reset value
#define __LSM6DSR_WHO_AM_I_DEFAULT_VALUE 0x6B

typedef struct {
    // [7:0] Device ID, read-only
    // 0x6B: LSM6DSR
    uint8_t dev_id;
} lsm6dsr_reg_who_am_i_t;

void lsm6dsr_reg_get_who_am_i(lsm6dsr_reg_who_am_i_t* who_am_i);

#endif
