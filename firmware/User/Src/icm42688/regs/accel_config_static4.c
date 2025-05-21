#include "User/Inc/icm42688/regs/accel_config_static4.h"

icm42688_reg_accel_config_static4_t icm42688_reg_new_accel_config_static4(void) {
    icm42688_reg_accel_config_static4_t accel_config_static4;
    accel_config_static4.accel_aaf_bitshift = 0x06;
    accel_config_static4.accel_aaf_deltsqr_bit_8_to_11 = 0x02;
    return accel_config_static4;
}

void icm42688_reg_set_accel_config_static4(icm42688_reg_accel_config_static4_t* accel_config_static4) {
    uint8_t accel_config_static4_data = __ICM42688_REG_ACCEL_CONFIG_STATIC4_DEFAULT_VALUE;
    accel_config_static4_data &= ~(0x0F << 4);
    accel_config_static4_data |= (accel_config_static4->accel_aaf_bitshift & 0x0F) << 4;
    accel_config_static4_data &= ~0x0F;
    accel_config_static4_data |= (accel_config_static4->accel_aaf_deltsqr_bit_8_to_11 & 0x0F);
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 2);
    icm42688_write_reg(ICM42688_REG_ACCEL_CONFIG_STATIC4, accel_config_static4_data);
}

void icm42688_reg_get_accel_config_static4(icm42688_reg_accel_config_static4_t* accel_config_static4) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 2);
    uint8_t accel_config_static4_data = 0;
    icm42688_read_reg(ICM42688_REG_ACCEL_CONFIG_STATIC4, &accel_config_static4_data);
    accel_config_static4->accel_aaf_bitshift = (accel_config_static4_data >> 4) & 0x0F;
    accel_config_static4->accel_aaf_deltsqr_bit_8_to_11 = accel_config_static4_data & 0x0F;
}
