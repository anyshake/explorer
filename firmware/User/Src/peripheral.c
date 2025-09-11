#include "User/Inc/peripheral.h"

void peri_gnss_init(void) {
    gnss_init(GNSS_CTL_PIN);
}

void peri_screen_init(void) {
    ssd1306_init();
    ssd1306_enable();
    ssd1306_clear();
}

void peri_eeprom_init(void) {
    eeprom_init(EEPROM_WP_PIN);
}

uint16_t peri_imu_init(uint8_t sample_rate) {
#if DEVICE_MODEL == E_C111G
    lsm6ds3_reg_ctrl3_c_t lsm6ds3_reg_ctrl3_c = lsm6ds3_reg_new_ctrl3_c();
    lsm6ds3_reg_ctrl3_c.bdu = LSM6DS3_REG_CTRL3_C_BDU_OUTPUT_REGISTERS_NOT_UPDATED;
    lsm6ds3_reg_ctrl3_c.if_inc = LSM6DS3_REG_CTRL3_C_IF_INC_ENABLED;
    lsm6ds3_reg_set_ctrl3_c(&lsm6ds3_reg_ctrl3_c);
    lsm6ds3_reg_ctrl1_xl_t lsm6ds3_reg_ctrl1_xl = lsm6ds3_reg_new_ctrl1_xl();
    lsm6ds3_reg_ctrl1_xl.odr_xl = LSM6DS3_REG_CTRL1_XL_ODR_XL_416HZ;
    lsm6ds3_reg_ctrl1_xl.fs_xl = LSM6DS3_REG_CTRL1_XL_FS_XL_2G;
    lsm6ds3_reg_ctrl1_xl.bw_xl = LSM6DS3_REG_CTRL1_XL_BW0_XL_50HZ;
    lsm6ds3_reg_set_ctrl1_xl(&lsm6ds3_reg_ctrl1_xl);
    lsm6ds3_reg_ctrl4_c_t lsm6ds3_reg_ctrl4_c = lsm6ds3_reg_new_ctrl4_c();
    lsm6ds3_reg_ctrl4_c.xl_bw_scal_odr = LSM6DS3_REG_CTRL4_C_XL_BW_SCAL_ODR_BYPASS;
    lsm6ds3_reg_ctrl4_c.drdy_mask = LSM6DS3_REG_CTRL4_C_DRDY_MASK_ENABLE;
    lsm6ds3_reg_set_ctrl4_c(&lsm6ds3_reg_ctrl4_c);
    lsm6ds3_reg_ctrl10_c_t lsm6ds3_reg_ctrl10_c = lsm6ds3_reg_new_ctrl10_c();
    lsm6ds3_reg_ctrl10_c.func_en = LSM6DS3_REG_CTRL10_C_FUNC_EN_ENABLE;
    lsm6ds3_reg_set_ctrl10_c(&lsm6ds3_reg_ctrl10_c);
    lsm6ds3_reg_ctrl8_xl_t lsm6ds3_reg_ctrl8_xl = lsm6ds3_reg_new_ctrl8_xl();
    lsm6ds3_reg_ctrl8_xl.hpcf_xl = LSM6DS3_REG_CTRL8_XL_HPCF_XL_LPF2_ODR_50;
    lsm6ds3_reg_ctrl8_xl.lpf2_xl_en = LSM6DS3_REG_CTRL8_XL_LPF2_XL_EN_ENABLE;
    lsm6ds3_reg_ctrl8_xl.hp_slope_xl_en = LSM6DS3_REG_CTRL8_XL_HP_SLOPE_XL_EN_DISABLE;
    lsm6ds3_reg_set_ctrl8_xl(&lsm6ds3_reg_ctrl8_xl);
    return lsm6ds3_get_lsb_per_g();
#elif DEVICE_MODEL == E_C121G
    icm42688_reg_accel_config0_t icm42688_reg_accel_config0 = icm42688_reg_new_accel_config0();
    icm42688_reg_accel_config0.accel_fs_sel = ICM42688_REG_ACCEL_CONFIG0_ACCEL_FS_SEL_2G;
    icm42688_reg_accel_config0.accel_odr = ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_500_HZ;
    icm42688_reg_set_accel_config0(&icm42688_reg_accel_config0);
    icm42688_reg_gyro_accel_config0_t icm42688_reg_gyro_accel_config0 = icm42688_reg_new_gyro_accel_config0();
    icm42688_reg_gyro_accel_config0.accel_ui_filt_bw = ICM42688_REG_GYRO_ACCEL_CONFIG0_ACCEL_UI_FILT_BW_MAX_400HZ_ODR_20;
    icm42688_reg_set_gyro_accel_config0(&icm42688_reg_gyro_accel_config0);
    icm42688_reg_accel_config_static3_t icm42688_reg_accel_config_static3 = icm42688_reg_new_accel_config_static3();
    icm42688_reg_accel_config_static3.accel_aaf_deltsqr_bit_0_to_7 = 1;
    icm42688_reg_set_accel_config_static3(&icm42688_reg_accel_config_static3);
    icm42688_reg_accel_config_static4_t icm42688_reg_accel_config_static4 = icm42688_reg_new_accel_config_static4();
    icm42688_reg_accel_config_static4.accel_aaf_deltsqr_bit_8_to_11 = 0;
    icm42688_reg_accel_config_static4.accel_aaf_bitshift = 15;
    icm42688_reg_set_accel_config_static4(&icm42688_reg_accel_config_static4);
    icm42688_reg_accel_config_static2_t icm42688_reg_accel_config_static2 = icm42688_reg_new_accel_config_static2();
    icm42688_reg_accel_config_static2.accel_aaf_delt = 1;
    icm42688_reg_accel_config_static2.accel_aaf_dis = ICM42688_REG_ACCEL_CONFIG_STATIC2_ACCEL_AAF_DIS_ENABLED;
    icm42688_reg_set_accel_config_static2(&icm42688_reg_accel_config_static2);
    icm42688_reg_pwr_mgmt0_t icm42688_reg_pwr_mgmt0 = icm42688_reg_new_pwr_mgmt0();
    icm42688_reg_pwr_mgmt0.temp_dis = ICM42688_REG_PWR_MGMT0_TEMP_DIS_ENABLED;
    icm42688_reg_pwr_mgmt0.gyro_mode = ICM42688_REG_PWR_MGMT0_GYRO_MODE_OFF;
    icm42688_reg_pwr_mgmt0.accel_mode = ICM42688_REG_PWR_MGMT0_ACCEL_MODE_LOW_NOISE;
    icm42688_reg_set_pwr_mgmt0(&icm42688_reg_pwr_mgmt0);
    return icm42688_get_lsb_per_g();
#endif
}

void peri_adc_init(uint8_t control_type, uint8_t sample_rate, bool channel_6d) {
    ads1262_reg_interface_t ads1262_reg_interface = ads1262_reg_new_interface();
    ads1262_reg_interface.status = ADS1262_REG_INTERFACE_STATUS_ENABLED;
    ads1262_reg_interface.crc = ADS1262_REG_INTERFACE_CRC_CRC;
    ads1262_reg_set_interface(&ads1262_reg_interface);
    ads1262_reg_mode_0_t ads1262_reg_mode_0 = ads1262_reg_new_mode_0();
    ads1262_reg_mode_0.run_mode = ADS1262_REG_MODE_0_RUN_MODE_ONESHOT;
    ads1262_reg_set_mode_0(&ads1262_reg_mode_0);
    ads1262_reg_mode_2_t ads1262_reg_mode_2 = ads1262_reg_new_mode_2();
    ads1262_reg_mode_2.dr = ADS1262_REG_MODE_2_DR_4800;
    ads1262_reg_set_mode_2(&ads1262_reg_mode_2);
}
