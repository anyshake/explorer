#include "User/Inc/icm42688/regs/accel_config1.h"

icm42688_reg_accel_config1_t icm42688_reg_new_accel_config1(void) {
    icm42688_reg_accel_config1_t accel_config1;
    accel_config1.accel_ui_filt_ord = ICM42688_REG_ACCEL_CONFIG1_ACCEL_UI_FILT_ORD_2;
    accel_config1.accel_dec2_m2_ord = ICM42688_REG_ACCEL_CONFIG1_ACCEL_DEC2_M2_ORD_3;
    return accel_config1;
}

void icm42688_reg_set_accel_config1(icm42688_reg_accel_config1_t* accel_config1) {
    uint8_t accel_config1_data = __ICM42688_REG_ACCEL_CONFIG1_DEFAULT_VALUE;
    accel_config1_data &= ~(0x03 << 3);
    accel_config1_data |= (accel_config1->accel_ui_filt_ord & 0x03) << 3;
    accel_config1_data &= ~(0x03 << 1);
    accel_config1_data |= (accel_config1->accel_dec2_m2_ord & 0x03) << 1;
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_write_reg(ICM42688_REG_ACCEL_CONFIG1, accel_config1_data);
}

void icm42688_reg_get_accel_config1(icm42688_reg_accel_config1_t* accel_config1) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    uint8_t accel_config1_data = 0;
    icm42688_read_reg(ICM42688_REG_ACCEL_CONFIG1, &accel_config1_data);
    accel_config1->accel_ui_filt_ord = (accel_config1_data >> 3) & 0x03;
    accel_config1->accel_dec2_m2_ord = (accel_config1_data >> 1) & 0x03;
}
