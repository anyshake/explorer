#include "User/Inc/icm42688/regs/accel_config_static3.h"

icm42688_reg_accel_config_static3_t icm42688_reg_new_accel_config_static3(void) {
    icm42688_reg_accel_config_static3_t accel_config_static3;
    accel_config_static3.accel_aaf_deltsqr_bit_0_to_7 = 0x40;
    return accel_config_static3;
}

void icm42688_reg_set_accel_config_static3(icm42688_reg_accel_config_static3_t* accel_config_static3) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 2);
    icm42688_write_reg(ICM42688_REG_ACCEL_CONFIG_STATIC3, accel_config_static3->accel_aaf_deltsqr_bit_0_to_7);
}

void icm42688_reg_get_accel_config_static3(icm42688_reg_accel_config_static3_t* accel_config_static3) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 2);
    icm42688_read_reg(ICM42688_REG_ACCEL_CONFIG_STATIC3, &accel_config_static3->accel_aaf_deltsqr_bit_0_to_7);
}
