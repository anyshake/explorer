#ifndef __ICM42688_REGS_WHO_AM_I_H
#define __ICM42688_REGS_WHO_AM_I_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

typedef struct {
    // Device ID, read-only
    // 0x47: ICM-42688
    uint8_t dev_id;
} icm42688_reg_who_am_i_t;

void icm42688_reg_get_who_am_i(icm42688_reg_who_am_i_t* who_am_i);

#endif
