#ifndef __LSM6DSR_REGS_INT1_CTRL_H
#define __LSM6DSR_REGS_INT1_CTRL_H

#include <stdint.h>

#include "User/Inc/lsm6dsr/reg_map.h"
#include "User/Inc/lsm6dsr/utils.h"

#define __LSM6DSR_REG_INT1_CTRL_DEFAULT_VALUE 0x00

#define LSM6DSR_REG_INT1_CTRL_DEN_DRDY_FLAG_DISABLED 0x00
#define LSM6DSR_REG_INT1_CTRL_DEN_DRDY_FLAG_ENABLED 0x01

#define LSM6DSR_REG_INT1_CTRL_INT1_CNT_BDR_DISABLED 0x00
#define LSM6DSR_REG_INT1_CTRL_INT1_CNT_BDR_ENABLED 0x01

#define LSM6DSR_REG_INT1_CTRL_INT1_FIFO_FULL_DISABLED 0x00
#define LSM6DSR_REG_INT1_CTRL_INT1_FIFO_FULL_ENABLED 0x01

#define LSM6DSR_REG_INT1_CTRL_INT1_FIFO_OVR_DISABLED 0x00
#define LSM6DSR_REG_INT1_CTRL_INT1_FIFO_OVR_ENABLED 0x01

#define LSM6DSR_REG_INT1_CTRL_INT1_FIFO_TH_DISABLED 0x00
#define LSM6DSR_REG_INT1_CTRL_INT1_FIFO_TH_ENABLED 0x01

#define LSM6DSR_REG_INT1_CTRL_INT1_BOOT_DISABLED 0x00
#define LSM6DSR_REG_INT1_CTRL_INT1_BOOT_ENABLED 0x01

#define LSM6DSR_REG_INT1_CTRL_INT1_DRDY_G_DISABLED 0x00
#define LSM6DSR_REG_INT1_CTRL_INT1_DRDY_G_ENABLED 0x01

#define LSM6DSR_REG_INT1_CTRL_INT1_DRDY_XL_DISABLED 0x00
#define LSM6DSR_REG_INT1_CTRL_INT1_DRDY_XL_ENABLED 0x01

typedef struct {
    // Sends DEN_DRDY (DEN stamped on Sensor Data flag) to INT1 pin
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t den_drdy_flag;
    // Enables COUNTER_BDR_IA interrupt on INT1
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t int1_cnt_bdr;
    // Enables FIFO full flag interrupt on INT1 pin
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t int1_fifo_full;
    // Enables FIFO overrun interrupt on INT1 pin
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t int1_fifo_ovr;
    // Enables FIFO threshold interrupt on INT1 pin
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t int1_fifo_th;
    // Enables boot status on INT1 pin
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t int1_boot;
    // Enables gyroscope data-ready interrupt on INT1 pin
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t int1_drdy_g;
    // Enables accelerometer data-ready interrupt on INT1 pin
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t int1_drdy_xl;
} lsm6dsr_reg_int1_ctrl_t;

lsm6dsr_reg_int1_ctrl_t lsm6dsr_reg_new_int1_ctrl(void);
void lsm6dsr_reg_set_int1_ctrl(lsm6dsr_reg_int1_ctrl_t* int1_ctrl);
void lsm6dsr_reg_get_int1_ctrl(lsm6dsr_reg_int1_ctrl_t* int1_ctrl);

#endif
