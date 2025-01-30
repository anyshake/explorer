#ifndef __ICM42688_REGS_INTF_CONFIG1_H
#define __ICM42688_REGS_INTF_CONFIG1_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

#define __ICM42688_REG_INTF_CONFIG1_DEFAULT_VALUE 0x91

#define ICM42688_REG_INTF_CONFIG1_ACCEL_LP_CLK_SEL_WAKEUP_OSC 0x00
#define ICM42688_REG_INTF_CONFIG1_ACCEL_LP_CLK_SEL_RC_OSC 0x01

#define ICM42688_REG_INTF_CONFIG1_RTC_MODE_DISABLED 0x00
#define ICMS42688_REG_INTF_CONFIG1_RTC_MODE_ENABLED 0x01

#define ICM42688_REG_INTF_CONFIG1_CLKSEL_ALWAYS_RC 0x00
#define ICM42688_REG_INTF_CONFIG1_CLKSEL_PLL_OR_RC 0x01
#define ICM42688_REG_INTF_CONFIG1_CLKSEL_DISABLE_ALL 0x03

typedef struct {
    // Selects the clock source for the accelerometer low power mode
    // 0x00: Accelerometer LP mode uses Wake Up oscillator clock (default)
    // 0x01: Accelerometer LP mode uses RC oscillator clock
    uint8_t accel_lp_clk_sel;
    // RTC mode enable
    // 0x00: No input RTC clock is required (default)
    // 0x01: RTC clock input is required
    uint8_t rtc_mode;
    // Clock source selection
    // 0x00: Always select internal RC oscillator
    // 0x01: Select PLL when available, else select RC oscillator (default)
    // 0x03: Disable all clocks
    uint8_t clksel;
} icm42688_reg_intf_config1_t;

icm42688_reg_intf_config1_t icm42688_reg_new_intf_config1(void);
void icm42688_reg_set_intf_config1(icm42688_reg_intf_config1_t* intf_config1);
void icm42688_reg_get_intf_config1(icm42688_reg_intf_config1_t* intf_config1);

#endif
