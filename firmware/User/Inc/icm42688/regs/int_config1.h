#ifndef __ICM42688_REGS_INT_CONFIG1_H
#define __ICM42688_REGS_INT_CONFIG1_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

// Register reset value
#define __ICM42688_REG_INT_CONFIG1_DEFAULT_VALUE 0x10

#define ICM42688_REG_INT_CONFIG1_INT_TPULSE_DURATION_100US 0x00
#define ICM42688_REG_INT_CONFIG1_INT_TPULSE_DURATION_8US 0x01

#define ICM42688_REG_INT_CONFIG1_INT_TDEASSERT_DISABLE_MINIMUM 0x00
#define ICM42688_REG_INT_CONFIG1_INT_TDEASSERT_DISABLE_DISABLED 0x01

#define ICM42688_REG_INT_CONFIG1_INT_ASYNC_RESET_NORMAL 0x00
#define ICM42688_REG_INT_CONFIG1_INT_ASYNC_RESET_RESET_VALUE 0x01

typedef struct {
    // [6] Interrupt pulse duration
    // 0x00: Interrupt pulse duration is 100 us. Use only if ODR < 4 kHz (default)
    // 0x01: Interrupt pulse duration is 8 us. Required if ODR >= 4 kHz, optional for ODR < 4 kHz
    uint8_t int_tpulse_duration;

    // [5] Interrupt de-assertion duration
    // 0x00: The interrupt de-assertion duration is set to a minimum of 100 us. Use only if ODR < 4 kHz (default)
    // 0x01: Disables de-assert duration. Required if ODR >= 4 kHz, optional for ODR < 4 kHz.
    uint8_t int_tdeassert_disable;

    // [4] User should change setting to 0 from default setting of 1, for proper INT1 and INT2 pin operation
    // 0x00: Required for proper INT1/INT2 operation
    // 0x01: Reset value; change to 0x00 before using INT1/INT2
    uint8_t int_async_reset;
} icm42688_reg_int_config1_t;

icm42688_reg_int_config1_t icm42688_reg_new_int_config1(void);
void icm42688_reg_set_int_config1(icm42688_reg_int_config1_t* int_config1);
void icm42688_reg_get_int_config1(icm42688_reg_int_config1_t* int_config1);

#endif
