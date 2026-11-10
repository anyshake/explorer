#ifndef __LSM6DSR_REGS_CTRL10_C_H
#define __LSM6DSR_REGS_CTRL10_C_H

#include <stdint.h>

#include "User/Inc/lsm6dsr/reg_map.h"
#include "User/Inc/lsm6dsr/utils.h"

// Register reset value
#define __LSM6DSR_REG_CTRL10_C_DEFAULT_VALUE 0x00

#define LSM6DSR_REG_CTRL10_C_TIMESTAMP_EN_DISABLED 0x00
#define LSM6DSR_REG_CTRL10_C_TIMESTAMP_EN_ENABLED 0x01

typedef struct {
    // [5] Enables timestamp counter
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t timestamp_en;
} lsm6dsr_reg_ctrl10_c_t;

lsm6dsr_reg_ctrl10_c_t lsm6dsr_reg_new_ctrl10_c(void);
void lsm6dsr_reg_set_ctrl10_c(lsm6dsr_reg_ctrl10_c_t* ctrl10_c);
void lsm6dsr_reg_get_ctrl10_c(lsm6dsr_reg_ctrl10_c_t* ctrl10_c);

#endif
