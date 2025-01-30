#ifndef __ICM42688_REGS_INT_SOURCE0_H
#define __ICM42688_REGS_INT_SOURCE0_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

#define __ICM42688_REG_INT_SOURCE0_DEFAULT_VALUE 0x10

#define ICM42688_REG_INT_SOURCE0_UI_FSYNC_INT1_EN_DISABLED 0x00
#define ICM42688_REG_INT_SOURCE0_UI_FSYNC_INT1_EN_ENABLED 0x01

#define ICM42688_REG_INT_SOURCE0_PLL_RDY_INT1_EN_DISABLED 0x00
#define ICM42688_REG_INT_SOURCE0_PLL_RDY_INT1_EN_ENABLED 0x01

#define ICM42688_REG_INT_SOURCE0_RESET_DONE_INT1_EN_DISABLED 0x00
#define ICM42688_REG_INT_SOURCE0_RESET_DONE_INT1_EN_ENABLED 0x01

#define ICM42688_REG_INT_SOURCE0_UI_DRDY_INT1_EN_DISABLED 0x00
#define ICM42688_REG_INT_SOURCE0_UI_DRDY_INT1_EN_ENABLED 0x01

#define ICM42688_REG_INT_SOURCE0_FIFO_THS_INT1_EN_DISABLED 0x00
#define ICM42688_REG_INT_SOURCE0_FIFO_THS_INT1_EN_ENABLED 0x01

#define ICM42688_REG_INT_SOURCE0_FIFO_FULL_INT1_EN_DISABLED 0x00
#define ICM42688_REG_INT_SOURCE0_FIFO_FULL_INT1_EN_ENABLED 0x01

#define ICM42688_REG_INT_SOURCE0_UI_AGC_RDY_INT1_EN_DISABLED 0x00
#define ICM42688_REG_INT_SOURCE0_UI_AGC_RDY_INT1_EN_ENABLED 0x01

typedef struct {
    // Routing of the UI FSYNC interrupt to INT1
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t ui_fsync_int1_en;
    // Routing of the PLL ready interrupt to INT1
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t pll_rdy_int1_en;
    // Routing of the reset done interrupt to INT1
    // 0x00: Disabled
    // 0x01: Enabled (default)
    uint8_t reset_done_int1_en;
    // Routing of the UI DRDY interrupt to INT1
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t ui_drdy_int1_en;
    // Routing of the FIFO threshold interrupt to INT1
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t fifo_ths_int1_en;
    // Routing of the FIFO full interrupt to INT1
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t fifo_full_int1_en;
    // Routing of the UI AGC ready interrupt to INT1
    // 0x00: Disabled (default)
    // 0x01: Enabled
    uint8_t ui_agc_rdy_int1_en;
} icm42688_reg_int_source0_t;

icm42688_reg_int_source0_t icm42688_reg_new_int_source0(void);
void icm42688_reg_set_int_source0(icm42688_reg_int_source0_t* int_source0);
void icm42688_reg_get_int_source0(icm42688_reg_int_source0_t* int_source0);

#endif
