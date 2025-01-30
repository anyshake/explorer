#include "User/Inc/icm42688/regs/intf_config1.h"

icm42688_reg_intf_config1_t icm42688_reg_new_intf_config1(void) {
    icm42688_reg_intf_config1_t intf_config1;
    intf_config1.accel_lp_clk_sel = ICM42688_REG_INTF_CONFIG1_ACCEL_LP_CLK_SEL_WAKEUP_OSC;
    intf_config1.rtc_mode = ICM42688_REG_INTF_CONFIG1_RTC_MODE_DISABLED;
    intf_config1.clksel = ICM42688_REG_INTF_CONFIG1_CLKSEL_PLL_OR_RC;
    return intf_config1;
}

void icm42688_reg_set_intf_config1(icm42688_reg_intf_config1_t* intf_config1) {
    uint8_t intf_config1_data = __ICM42688_REG_INTF_CONFIG1_DEFAULT_VALUE;
    intf_config1_data &= ~(0x01 << 3);
    intf_config1_data |= (intf_config1->accel_lp_clk_sel & 0x01) << 3;
    intf_config1_data &= ~(0x01 << 2);
    intf_config1_data |= (intf_config1->rtc_mode & 0x01) << 2;
    intf_config1_data &= ~0x03;
    intf_config1_data |= (intf_config1->clksel & 0x03);
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_write_reg(ICM42688_REG_INTF_CONFIG1, intf_config1_data);
}

void icm42688_reg_get_intf_config1(icm42688_reg_intf_config1_t* intf_config1) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    uint8_t intf_config1_data = 0;
    icm42688_read_reg(ICM42688_REG_INTF_CONFIG1, &intf_config1_data);
    intf_config1->accel_lp_clk_sel = (intf_config1_data >> 3) & 0x01;
    intf_config1->rtc_mode = (intf_config1_data >> 2) & 0x01;
    intf_config1->clksel = intf_config1_data & 0x03;
}
