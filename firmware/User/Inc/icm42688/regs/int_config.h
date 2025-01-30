#ifndef __ICM42688_REGS_INT_CONFIG_H
#define __ICM42688_REGS_INT_CONFIG_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

#define __ICM42688_REG_INT_CONFIG_DEFAULT_VALUE 0x00

#define ICM42688_REG_INT_CONFIG_INT2_MODE_PULSED 0x00
#define ICM42688_REG_INT_CONFIG_INT2_MODE_LATCHED 0x01

#define ICM42688_REG_INT_CONFIG_INT2_DRIVE_CIRCUIT_OPEN_DRAIN 0x00
#define ICM42688_REG_INT_CONFIG_INT2_DRIVE_CIRCUIT_PUSH_PULL 0x01

#define ICM42688_REG_INT_CONFIG_INT2_POLARITY_ACTIVE_LOW 0x00
#define ICM42688_REG_INT_CONFIG_INT2_POLARITY_ACTIVE_HIGH 0x01

#define ICM42688_REG_INT_CONFIG_INT1_MODE_PULSED 0x00
#define ICM42688_REG_INT_CONFIG_INT1_MODE_LATCHED 0x02

#define ICM42688_REG_INT_CONFIG_INT1_DRIVE_CIRCUIT_OPEN_DRAIN 0x00
#define ICM42688_REG_INT_CONFIG_INT1_DRIVE_CIRCUIT_PUSH_PULL 0x02

#define ICM42688_REG_INT_CONFIG_INT1_POLARITY_ACTIVE_LOW 0x00
#define ICM42688_REG_INT_CONFIG_INT1_POLARITY_ACTIVE_HIGH 0x02

typedef struct {
    // INT2 interrupt mode
    // 0x00: Pulsed mode (default)
    // 0x01: Latched mode
    uint8_t int2_mode;
    // INT2 drive circuit
    // 0x00: Open-drain (default)
    // 0x01: Push-pull
    uint8_t int2_drive_circuit;
    // INT2 interrupt polarity
    // 0x00: Active-low (default)
    // 0x01: Active-high
    uint8_t int2_polarity;
    // INT1 interrupt mode
    // 0x00: Pulsed mode (default)
    // 0x02: Latched mode
    uint8_t int1_mode;
    // INT1 drive circuit
    // 0x00: Open-drain (default)
    // 0x02: Push-pull
    uint8_t int1_drive_circuit;
    // INT1 interrupt polarity
    // 0x00: Active-low (default)
    // 0x02: Active-high
    uint8_t int1_polarity;
} icm42688_reg_int_config_t;

icm42688_reg_int_config_t icm42688_reg_new_int_config(void);
void icm42688_reg_set_int_config(icm42688_reg_int_config_t* int_config);
void icm42688_reg_get_int_config(icm42688_reg_int_config_t* int_config);

#endif
