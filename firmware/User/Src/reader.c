#include "User/Inc/reader.h"

explorer_adc_calibration_offset_t adc_calibration_offset_new(void) {
    explorer_adc_calibration_offset_t offset_cal = {0};
    return offset_cal;
}

int64_t get_adc_readout(ads1262_ctl_pin_t ctl_pin, explorer_adc_calibration_offset_t offset_cal, int32_t arr[]) {
    ads1262_reg_ofcal_t ofcal = ads1262_reg_new_ofcal();
    ads1262_reg_inpmux_t inpmux = ads1262_reg_new_inpmux();
    ads1262_cmd_rdata_t rdata;

    ofcal.ofcal_0 = offset_cal.channel_1[0];
    ofcal.ofcal_1 = offset_cal.channel_1[1];
    ofcal.ofcal_2 = offset_cal.channel_1[2];
    ads1262_reg_set_ofcal(&ofcal);
    inpmux.mux_n = ADS1262_REG_INPMUX_AIN0;
    inpmux.mux_p = ADS1262_REG_INPMUX_AIN1;
    ads1262_reg_set_inpmux(&inpmux);
    int64_t started_at_us = mcu_utils_systime_get_unix_time_us();
    ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
    int64_t timestamp_us = started_at_us + (mcu_utils_systime_get_unix_time_us() - started_at_us) / 2;
    arr[0] = ads1262_cmd_rdata_is_valid(&rdata, ADS1262_REG_INTERFACE_CRC_CRC)
                 ? rdata.data
                 : UINT32_MAX;

    ofcal.ofcal_0 = offset_cal.channel_2[0];
    ofcal.ofcal_1 = offset_cal.channel_2[1];
    ofcal.ofcal_2 = offset_cal.channel_2[2];
    ads1262_reg_set_ofcal(&ofcal);
    inpmux.mux_n = ADS1262_REG_INPMUX_AIN2;
    inpmux.mux_p = ADS1262_REG_INPMUX_AIN3;
    ads1262_reg_set_inpmux(&inpmux);
    ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
    arr[1] = ads1262_cmd_rdata_is_valid(&rdata, ADS1262_REG_INTERFACE_CRC_CRC)
                 ? rdata.data
                 : UINT32_MAX;

    ofcal.ofcal_0 = offset_cal.channel_3[0];
    ofcal.ofcal_1 = offset_cal.channel_3[1];
    ofcal.ofcal_2 = offset_cal.channel_3[2];
    ads1262_reg_set_ofcal(&ofcal);
    inpmux.mux_n = ADS1262_REG_INPMUX_AIN4;
    inpmux.mux_p = ADS1262_REG_INPMUX_AIN5;
    ads1262_reg_set_inpmux(&inpmux);
    ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
    arr[2] = ads1262_cmd_rdata_is_valid(&rdata, ADS1262_REG_INTERFACE_CRC_CRC)
                 ? rdata.data
                 : UINT32_MAX;

    return timestamp_us;
}

static void get_lsm6dsr_accel_readout(uint16_t lsb_per_g, int16_t arr[]) {
    uint8_t data[6] = {0};

    lsm6dsr_wait();

    // One burst for all three axes; output order follows the board orientation.
    lsm6dsr_read_regs(LSM6DSR_REG_OUTX_L_XL, data, sizeof(data));
    arr[0] = (int16_t)((uint16_t)data[5] << 8 | data[4]);
    arr[0] -= lsb_per_g;
    arr[1] = (int16_t)((uint16_t)data[1] << 8 | data[0]);
    arr[2] = (int16_t)((uint16_t)data[3] << 8 | data[2]);
}

static void get_lsm6dsr_temperature(float* temp) {
    lsm6dsr_reg_out_temp_t out_temp;
    lsm6dsr_reg_get_out_temp(&out_temp);
    int16_t temp_raw = (int16_t)((uint16_t)out_temp.out_temp_h << 8 | out_temp.out_temp_l);
    *temp = (float)temp_raw / 256.0f + 25.0f;
}

static void get_lsm6ds3_accel_readout(uint16_t lsb_per_g, int16_t arr[]) {
    uint8_t data[6] = {0};

    lsm6ds3_wait();

    // One burst for all three axes; output order follows the board orientation.
    lsm6ds3_read_regs(LSM6DS3_REG_OUTX_L_XL, data, sizeof(data));
    arr[0] = (int16_t)((uint16_t)data[5] << 8 | data[4]);
    arr[0] -= lsb_per_g;
    arr[1] = (int16_t)((uint16_t)data[1] << 8 | data[0]);
    arr[2] = (int16_t)((uint16_t)data[3] << 8 | data[2]);
}

static void get_lsm6ds3_temperature(float* temp) {
    lsm6ds3_reg_out_temp_t out_temp;
    lsm6ds3_reg_get_out_temp(&out_temp);
    int16_t temp_raw = (int16_t)((uint16_t)out_temp.out_temp_h << 8 | out_temp.out_temp_l);
    *temp = (float)temp_raw / 16.0f + 25.0f;
}

static void get_lsm6ds3tr_c_accel_readout(uint16_t lsb_per_g, int16_t arr[]) {
    uint8_t data[6] = {0};

    lsm6ds3tr_c_wait();

    // One burst for all three axes; output order follows the board orientation.
    lsm6ds3tr_c_read_regs(LSM6DS3TR_C_REG_OUTX_L_XL, data, sizeof(data));
    arr[0] = (int16_t)((uint16_t)data[5] << 8 | data[4]);
    arr[0] -= lsb_per_g;
    arr[1] = (int16_t)((uint16_t)data[1] << 8 | data[0]);
    arr[2] = (int16_t)((uint16_t)data[3] << 8 | data[2]);
}

static void get_lsm6ds3tr_c_temperature(float* temp) {
    lsm6ds3tr_c_reg_out_temp_t out_temp;
    lsm6ds3tr_c_reg_get_out_temp(&out_temp);
    int16_t temp_raw = (int16_t)((uint16_t)out_temp.out_temp_h << 8 | out_temp.out_temp_l);
    *temp = (float)temp_raw / 256.0f + 25.0f;
}

static void get_icm42688_accel_readout(uint16_t lsb_per_g, int16_t arr[]) {
    uint8_t data[6] = {0};

    icm42688_wait();

    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0x00);
    // One burst for all three axes; output order follows the board orientation.
    icm42688_read_regs(ICM42688_REG_ACCEL_DATA_X1, data, sizeof(data));
    arr[0] = (int16_t)((uint16_t)data[4] << 8 | data[5]);
    arr[0] -= lsb_per_g;
    arr[1] = (int16_t)((uint16_t)data[0] << 8 | data[1]);
    arr[2] = (int16_t)((uint16_t)data[2] << 8 | data[3]);
}

static void get_icm42688_temperature(float* temp) {
    icm42688_reg_temp_data_t temp_data;
    icm42688_reg_get_temp_data(&temp_data);
    int16_t temp_raw = (int16_t)((uint16_t)temp_data.temp_data_h << 8 | temp_data.temp_data_l);
    *temp = (float)temp_raw / 132.48f + 25.0f;
}

void get_accel_readout(explorer_imu_type_t imu_type, uint16_t lsb_per_g, int16_t arr[]) {
    switch (imu_type) {
        case EXPLORER_IMU_LSM6DS3:
            get_lsm6ds3_accel_readout(lsb_per_g, arr);
            break;
        case EXPLORER_IMU_LSM6DS3TR_C:
            get_lsm6ds3tr_c_accel_readout(lsb_per_g, arr);
            break;
        case EXPLORER_IMU_LSM6DSR:
            get_lsm6dsr_accel_readout(lsb_per_g, arr);
            break;
        case EXPLORER_IMU_ICM42688:
            get_icm42688_accel_readout(lsb_per_g, arr);
            break;
        default:
            arr[0] = arr[1] = arr[2] = 0;
            break;
    }
}

void get_env_temperature(explorer_imu_type_t imu_type, float* temp) {
    switch (imu_type) {
        case EXPLORER_IMU_LSM6DS3:
            get_lsm6ds3_temperature(temp);
            break;
        case EXPLORER_IMU_LSM6DS3TR_C:
            get_lsm6ds3tr_c_temperature(temp);
            break;
        case EXPLORER_IMU_LSM6DSR:
            get_lsm6dsr_temperature(temp);
            break;
        case EXPLORER_IMU_ICM42688:
            get_icm42688_temperature(temp);
            break;
        default:
            *temp = 0.0f;
            break;
    }
}
