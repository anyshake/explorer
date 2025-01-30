#include "User/Inc/icm42688/regs/gyro_accel_config0.h"

icm42688_reg_gyro_accel_config0_t icm42688_reg_new_gyro_accel_config0(void) {
    icm42688_reg_gyro_accel_config0_t gyro_accel_config0;
    gyro_accel_config0.accel_ui_filt_bw = ICM42688_REG_GYRO_ACCEL_CONFIG0_ACCEL_UI_FILT_BW_MAX_400HZ_ODR_4;
    gyro_accel_config0.gyro_ui_filt_bw = ICM42688_REG_GYRO_ACCEL_CONFIG0_GYRO_UI_FILT_BW_MAX_400HZ_ODR_4;
    return gyro_accel_config0;
}

void icm42688_reg_set_gyro_accel_config0(icm42688_reg_gyro_accel_config0_t* gyro_accel_config0) {
    uint8_t gyro_accel_config0_data = __ICM42688_REG_GYRO_ACCEL_CONFIG0_DEFAULT_VALUE;
    gyro_accel_config0_data &= ~(0x0F << 4);
    gyro_accel_config0_data |= (gyro_accel_config0->accel_ui_filt_bw & 0x0F) << 4;
    gyro_accel_config0_data &= ~0x0F;
    gyro_accel_config0_data |= (gyro_accel_config0->gyro_ui_filt_bw & 0x0F);
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_write_reg(ICM42688_REG_GYRO_ACCEL_CONFIG0, gyro_accel_config0_data);
}

void icm42688_reg_get_gyro_accel_config0(icm42688_reg_gyro_accel_config0_t* gyro_accel_config0) {
    icm42688_write_reg(ICM42688_REG_GYRO_ACCEL_CONFIG0, 0);
    uint8_t gyro_accel_config0_data = 0;
    icm42688_read_reg(ICM42688_REG_GYRO_ACCEL_CONFIG0, &gyro_accel_config0_data);
    gyro_accel_config0->accel_ui_filt_bw = (gyro_accel_config0_data >> 4) & 0x0F;
    gyro_accel_config0->gyro_ui_filt_bw = gyro_accel_config0_data & 0x0F;
}
