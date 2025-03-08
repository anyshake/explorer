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
#ifdef USE_ICM42688
    icm42688_reg_accel_config0_t icm42688_reg_accel_config0 = icm42688_reg_new_accel_config0();
    icm42688_reg_accel_config0.accel_fs_sel = ICM42688_REG_ACCEL_CONFIG0_ACCEL_FS_SEL_2G;
    icm42688_reg_accel_config0.accel_odr = ICM42688_REG_ACCEL_CONFIG0_ACCEL_ODR_1_KHZ;
    icm42688_reg_set_accel_config0(&icm42688_reg_accel_config0);
    icm42688_reg_gyro_accel_config0_t icm42688_reg_gyro_accel_config0 = icm42688_reg_new_gyro_accel_config0();
    icm42688_reg_gyro_accel_config0.accel_ui_filt_bw = ICM42688_REG_GYRO_ACCEL_CONFIG0_ACCEL_UI_FILT_BW_LOW_LATENCY_ODR_DEC2;
    icm42688_reg_set_gyro_accel_config0(&icm42688_reg_gyro_accel_config0);
    icm42688_reg_accel_config_static2_t icm42688_reg_accel_config_static2 = icm42688_reg_new_accel_config_static2();
    icm42688_reg_accel_config_static2.accel_aaf_delt = 100;
    icm42688_reg_accel_config_static2.accel_aaf_dis = ICM42688_REG_ACCEL_CONFIG_STATIC2_ACCEL_AAF_DIS_ENABLED;
    icm42688_reg_set_accel_config_static2(&icm42688_reg_accel_config_static2);
    icm42688_reg_pwr_mgmt0_t icm42688_reg_pwr_mgmt0 = icm42688_reg_new_pwr_mgmt0();
    icm42688_reg_pwr_mgmt0.temp_dis = ICM42688_REG_PWR_MGMT0_TEMP_DIS_ENABLED;
    icm42688_reg_pwr_mgmt0.gyro_mode = ICM42688_REG_PWR_MGMT0_GYRO_MODE_OFF;
    icm42688_reg_pwr_mgmt0.accel_mode = ICM42688_REG_PWR_MGMT0_ACCEL_MODE_LOW_NOISE;
    icm42688_reg_set_pwr_mgmt0(&icm42688_reg_pwr_mgmt0);
    return icm42688_get_lsb_per_g();
#else
    lsm6ds3_reg_int1_ctrl_t lsm6ds3_reg_int1_ctrl = lsm6ds3_reg_new_int1_ctrl();
    lsm6ds3_reg_int1_ctrl.drdy_xl = LSM6DS3_REG_INT1_CTRL_DRDY_XL_ENABLED;
    lsm6ds3_reg_set_int1_ctrl(&lsm6ds3_reg_int1_ctrl);
    lsm6ds3_reg_ctrl3_c_t lsm6ds3_reg_ctrl3_c = lsm6ds3_reg_new_ctrl3_c();
    lsm6ds3_reg_ctrl3_c.bdu = LSM6DS3_REG_CTRL3_C_BDU_OUTPUT_REGISTERS_NOT_UPDATED;
    lsm6ds3_reg_ctrl3_c.if_inc = LSM6DS3_REG_CTRL3_C_IF_INC_ENABLED;
    lsm6ds3_reg_set_ctrl3_c(&lsm6ds3_reg_ctrl3_c);
    lsm6ds3_reg_ctrl1_xl_t lsm6ds3_reg_ctrl1_xl = lsm6ds3_reg_new_ctrl1_xl();
    lsm6ds3_reg_ctrl1_xl.odr_xl = LSM6DS3_REG_CTRL1_XL_ODR_XL_833HZ;
    lsm6ds3_reg_ctrl1_xl.fs_xl = LSM6DS3_REG_CTRL1_XL_FS_XL_2G;
    lsm6ds3_reg_ctrl1_xl.bw_xl = LSM6DS3_REG_CTRL1_XL_BW0_XL_100HZ;
    lsm6ds3_reg_set_ctrl1_xl(&lsm6ds3_reg_ctrl1_xl);
    return lsm6ds3_get_lsb_per_g();
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
    switch (sample_rate) {
        case 50:
            ads1262_reg_mode_2.dr = ADS1262_REG_MODE_2_DR_400;
            break;
        case 100:
            ads1262_reg_mode_2.dr = channel_6d ? ADS1262_REG_MODE_2_DR_1200 : ADS1262_REG_MODE_2_DR_400;
            break;
        case 200:
            ads1262_reg_mode_2.dr = channel_6d ? ADS1262_REG_MODE_2_DR_2400 : ADS1262_REG_MODE_2_DR_1200;
            break;
        case 250:
            ads1262_reg_mode_2.dr = channel_6d ? ADS1262_REG_MODE_2_DR_4800 : ADS1262_REG_MODE_2_DR_2400;
            break;
    }
    ads1262_reg_set_mode_2(&ads1262_reg_mode_2);
}
