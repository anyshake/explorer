#ifndef __LSM6DS3_REGS_STATUS_REG_H
#define __LSM6DS3_REGS_STATUS_REG_H

#include <stdint.h>

#include "User/Inc/lsm6ds3/reg_map.h"
#include "User/Inc/lsm6ds3/utils.h"

#define __LSM6DS3_STATUS_REG_DEFAULT_VALUE 0x00

typedef struct {
    // Temperature new data available
    // 0x00: no set of data is available at temperature sensor output (default)
    // 0x01: new data is available at temperature sensor output
    uint8_t tda;
    // Gyroscope new data available
    // 0x00: no set of data is available at gyroscope output (default)
    // 0x01: new data is available at gyroscope output
    uint8_t gda;
    // Accelerometer new data available
    // 0x00: no set of data is available at accelerometer output (default)
    // 0x01: new data is available at accelerometer output
    uint8_t xlda;
} lsm6ds3_reg_status_reg_t;

void lsm6ds3_reg_get_status_reg(lsm6ds3_reg_status_reg_t* status_reg);

#endif
