#ifndef __LSM6DSR_REGS_CTRL3_C_H
#define __LSM6DSR_REGS_CTRL3_C_H

#include <stdint.h>

#include "User/Inc/lsm6dsr/reg_map.h"
#include "User/Inc/lsm6dsr/utils.h"

// Register reset value
#define __LSM6DSR_REG_CTRL3_C_DEFAULT_VALUE 0x04

#define LSM6DSR_REG_CTRL3_C_BOOT_NORMAL_MODE 0x00
#define LSM6DSR_REG_CTRL3_C_BOOT_REBOOT_MEMORY_CONTENT 0x01

#define LSM6DSR_REG_CTRL3_C_BDU_CONTINUOUS_UPDATE 0x00
#define LSM6DSR_REG_CTRL3_C_BDU_OUTPUT_REGISTERS_NOT_UPDATED 0x01

#define LSM6DSR_REG_CTRL3_C_H_LACTIVE_ACTIVE_HIGH 0x00
#define LSM6DSR_REG_CTRL3_C_H_LACTIVE_ACTIVE_LOW 0x01

#define LSM6DSR_REG_CTRL3_C_PP_OD_PUSH_PULL_MODE 0x00
#define LSM6DSR_REG_CTRL3_C_PP_OD_OPEN_DRAIN_MODE 0x01

#define LSM6DSR_REG_CTRL3_C_SIM_4_WIRE_INTERFACE 0x00
#define LSM6DSR_REG_CTRL3_C_SIM_3_WIRE_INTERFACE 0x01

#define LSM6DSR_REG_CTRL3_C_IF_INC_DISABLED 0x00
#define LSM6DSR_REG_CTRL3_C_IF_INC_ENABLED 0x01

#define LSM6DSR_REG_CTRL3_C_SW_RESET_NORMAL_MODE 0x00
#define LSM6DSR_REG_CTRL3_C_SW_RESET_RESET_DEVICE 0x01

typedef struct {
    // [7] Reboot memory content
    // 0x00: Normal mode (default)
    // 0x01: Reboot memory content
    uint8_t boot;

    // [6] Block Data Update
    // 0x00: Continuous update (default)
    // 0x01: Output registers not updated until MSB and LSB have been read
    uint8_t bdu;

    // [5] Interrupt activation level
    // 0x00: Interrupt output pads active high (default)
    // 0x01: Interrupt output pads active low
    uint8_t h_lactive;

    // [4] Push-pull/open-drain selection on INT1 and INT2 pads
    // 0x00: Push-pull mode (default)
    // 0x01: Open-drain mode
    uint8_t pp_od;

    // [3] SPI Serial Interface Mode selection
    // 0x00: 4-wire interface (default)
    // 0x01: 3-wire interface
    uint8_t sim;

    // [2] Register address automatically incremented during a multiple byte access
    // with a serial interface (I2C or SPI)
    // 0x00: Disabled
    // 0x01: Enabled (default)
    uint8_t if_inc;

    // [0] Software reset
    // Note: This bit is automatically cleared after the reset operation
    // 0x00: Normal mode (default)
    // 0x01: Reset device
    uint8_t sw_reset;
} lsm6dsr_reg_ctrl3_c_t;

lsm6dsr_reg_ctrl3_c_t lsm6dsr_reg_new_ctrl3_c(void);
void lsm6dsr_reg_set_ctrl3_c(lsm6dsr_reg_ctrl3_c_t* ctrl3_c);
void lsm6dsr_reg_get_ctrl3_c(lsm6dsr_reg_ctrl3_c_t* ctrl3_c);

#endif
