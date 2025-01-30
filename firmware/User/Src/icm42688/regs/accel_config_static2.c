#include "User/Inc/icm42688/regs/accel_config_static2.h"

icm42688_reg_accel_config_static2_t icm42688_reg_new_accel_config_static2(void) {
    icm42688_reg_accel_config_static2_t accel_config_static2;
    accel_config_static2.accel_aaf_delt = 0x18;
    accel_config_static2.accel_aaf_dis = ICM42688_REG_ACCEL_CONFIG_STATIC2_ACCEL_AAF_DIS_ENABLED;
    return accel_config_static2;
}

void icm42688_reg_set_accel_config_static2(icm42688_reg_accel_config_static2_t* accel_config_static2) {
    uint8_t accel_config_static2_data = __ICM42688_REG_ACCEL_CONFIG_STATIC2_DEFAULT_VALUE;
    accel_config_static2_data &= ~(0x3F << 1);
    accel_config_static2_data |= (accel_config_static2->accel_aaf_delt & 0x3F) << 1;
    accel_config_static2_data &= ~0x01;
    accel_config_static2_data |= (accel_config_static2->accel_aaf_dis & 0x01);
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 2);
    icm42688_write_reg(ICM42688_REG_ACCEL_CONFIG_STATIC2, accel_config_static2_data);
}

void icm42688_reg_get_accel_config_static2(icm42688_reg_accel_config_static2_t* accel_config_static2) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 2);
    uint8_t accel_config_static2_data = 0;
    icm42688_read_reg(ICM42688_REG_ACCEL_CONFIG_STATIC2, &accel_config_static2_data);
    accel_config_static2->accel_aaf_delt = (accel_config_static2_data >> 1) & 0x3F;
    accel_config_static2->accel_aaf_dis = accel_config_static2_data & 0x01;
}
