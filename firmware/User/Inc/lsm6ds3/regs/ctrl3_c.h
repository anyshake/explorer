#ifndef __LSM6DS3_REGS_CTRL3_C_H
#define __LSM6DS3_REGS_CTRL3_C_H

#include <stdint.h>

#include "User/Inc/lsm6ds3/reg_map.h"
#include "User/Inc/lsm6ds3/utils.h"

#define __LSM6DS3_CTRL3_C_DEFAULT_VALUE 0x04

#define LSM6DS3_CTRL3_C_BOOT_NORMAL_MODE 0x00
#define LSM6DS3_CTRL3_C_BOOT_REBOOT_MEMORY_CONTENT 0x01

#define LSM6DS3_CTRL3_C_BDU_CONTINUOUS_UPDATE 0x00
#define LSM6DS3_CTRL3_C_BDU_OUTPUT_REGISTERS_NOT_UPDATED 0x01

#define LSM6DS3_CTRL3_C_H_LACTIVE_ACTIVE_HIGH 0x00
#define LSM6DS3_CTRL3_C_H_LACTIVE_ACTIVE_LOW 0x01

#define LSM6DS3_CTRL3_C_PP_OD_PUSH_PULL_MODE 0x00
#define LSM6DS3_CTRL3_C_PP_OD_OPEN_DRAIN_MODE 0x01

#define LSM6DS3_CTRL3_C_SIM_4_WIRE_INTERFACE 0x00
#define LSM6DS3_CTRL3_C_SIM_3_WIRE_INTERFACE 0x01

#define LSM6DS3_CTRL3_C_IF_INC_DISABLED 0x00
#define LSM6DS3_CTRL3_C_IF_INC_ENABLED 0x01

#define LSM6DS3_CTRL3_C_BLE_DATA_LSB 0x00
#define LSM6DS3_CTRL3_C_BLE_DATA_MSB 0x01

#define LSM6DS3_CTRL3_C_SW_RESET_NORMAL_MODE 0x00
#define LSM6DS3_CTRL3_C_SW_RESET_RESET_DEVICE 0x01

typedef struct {
    // Reboot memory content
    // 0x00: normal mode (default)
    // 0x01: reboot memory content
    uint8_t boot;
    // Block Data Update
    // 0x00: continuous update (default)
    // 0x01: output registers not updated until MSB and LSB have been read
    uint8_t bdu;
    // Interrupt activation level
    // 0x00: interrupt output pads active high (default)
    // 0x01: interrupt output pads active low
    uint8_t h_lactive;
    // Push-pull/open-drain selection on INT1 and INT2 pads
    // 0x00: push-pull mode (default)
    // 0x01: open-drain mode
    uint8_t pp_od;
    // SPI Serial Interface Mode selection
    // 0x00: 4-wire interface (default)
    // 0x01: 3-wire interface
    uint8_t sim;
    // Register address automatically incremented during a multiple byte access
    // with a serial interface (I2C or SPI)
    // 0x00: disabled (default)
    // 0x01: enabled
    uint8_t if_inc;
    // Big/Little Endian Data selection
    // 0x00: data LSB @ lower address (default)
    // 0x01: data MSB @ lower address
    uint8_t ble;
    // Software reset
    // Note: This bit is automatically cleared after the reset operation
    // 0x00: normal mode (default)
    // 0x01: reset device
    uint8_t sw_reset;
} lsm6ds3_reg_ctrl3_c_t;

void lsm6ds3_reg_set_ctrl3_c(lsm6ds3_reg_ctrl3_c_t* ctrl3_c);
void lsm6ds3_reg_get_ctrl3_c(lsm6ds3_reg_ctrl3_c_t* ctrl3_c);

#endif
