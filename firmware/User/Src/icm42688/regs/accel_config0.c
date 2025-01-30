#include "User/Inc/icm42688/regs/accel_config0.h"

icm42688_reg_accel_config0_t icm42688_reg_new_accel_config0(void) {
    icm42688_reg_accel_config0_t accel_config0;
    accel_config0.accel_fs_sel = ICM42688_REG_ACCEL_CONFIG0_ACCEL_FS_SEL_16G;
    accel_config0.accel_odr = ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_1_KHZ;
    return accel_config0;
}

void icm42688_reg_set_accel_config0(icm42688_reg_accel_config0_t* accel_config0) {
    uint8_t accel_config0_data = __ICM42688_REG_ACCEL_CONFIG0_DEFAULT_VALUE;
    accel_config0_data &= ~(0x03 << 5);
    accel_config0_data |= (accel_config0->accel_fs_sel & 0x03) << 5;
    accel_config0_data &= ~0x0F;
    accel_config0_data |= (accel_config0->accel_odr & 0x0F);
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_write_reg(ICM42688_REG_ACCEL_CONFIG0, accel_config0_data);
}

void icm42688_reg_get_accel_config0(icm42688_reg_accel_config0_t* accel_config0) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    uint8_t accel_config0_data = 0;
    icm42688_read_reg(ICM42688_REG_ACCEL_CONFIG0, &accel_config0_data);
    accel_config0->accel_fs_sel = (accel_config0_data >> 5) & 0x03;
    accel_config0->accel_odr = accel_config0_data & 0x0F;
}
