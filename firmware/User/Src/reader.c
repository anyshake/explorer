#include "User/Inc/reader.h"

explorer_adc_calibration_offset_t adc_calibration_offset_new(void) {
    explorer_adc_calibration_offset_t offset_cal;
    for (int i = 0; i < 3; i++) {
        offset_cal.channel_1[i] = 0x00;
        offset_cal.channel_2[i] = 0x00;
        offset_cal.channel_3[i] = 0x00;
    }
    return offset_cal;
}

void get_adc_readout(ads1262_ctl_pin_t ctl_pin, explorer_adc_calibration_offset_t offset_cal, int32_t arr[]) {
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
    ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
    arr[0] = ads1262_cmd_rdata_is_valid(&rdata, ADS1262_REG_INTERFACE_CRC_CRC)
                 ? rdata.data
                 : 0;

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
                 : 0;

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
                 : 0;
}

#if DEVICE_MODEL == E_C111G
void get_accel_readout(uint16_t lsb_per_g, int16_t arr[]) {
    lsm6ds3_reg_outz_xl_t outz_xl;
    lsm6ds3_reg_outx_xl_t outx_xl;
    lsm6ds3_reg_outy_xl_t outy_xl;

    lsm6ds3_wait();

    lsm6ds3_reg_get_outz_xl(&outz_xl);
    arr[0] = (int16_t)(outz_xl.outz_h_xl << 8 | outz_xl.outz_l_xl);
    arr[0] -= lsb_per_g;

    lsm6ds3_reg_get_outx_xl(&outx_xl);
    arr[1] = (int16_t)(outx_xl.outx_h_xl << 8 | outx_xl.outx_l_xl);

    lsm6ds3_reg_get_outy_xl(&outy_xl);
    arr[2] = (int16_t)(outy_xl.outy_h_xl << 8 | outy_xl.outy_l_xl);
}

void get_env_temperature(float* temp) {
    lsm6ds3_reg_out_temp_t out_temp;
    lsm6ds3_reg_get_out_temp(&out_temp);
    *temp = (float)out_temp.out_temp_l / 16.0 + 25.0;
}
#elif DEVICE_MODEL == E_C121G
void get_accel_readout(uint16_t lsb_per_g, int16_t arr[]) {
    icm42688_reg_accel_data_x_t accel_data_x;
    icm42688_reg_accel_data_y_t accel_data_y;
    icm42688_reg_accel_data_z_t accel_data_z;

    icm42688_wait();

    icm42688_reg_get_accel_data_z(&accel_data_z);
    arr[0] = (int16_t)(accel_data_z.accel_data_z_h << 8 | accel_data_z.accel_data_z_l);
    arr[0] -= lsb_per_g;

    icm42688_reg_get_accel_data_x(&accel_data_x);
    arr[1] = (int16_t)(accel_data_x.accel_data_x_h << 8 | accel_data_x.accel_data_x_l);

    icm42688_reg_get_accel_data_y(&accel_data_y);
    arr[2] = (int16_t)(accel_data_y.accel_data_y_h << 8 | accel_data_y.accel_data_y_l);
}

void get_env_temperature(float* temp) {
    icm42688_reg_temp_data_t temp_data;
    icm42688_reg_get_temp_data(&temp_data);
    int16_t temp_raw = temp_data.temp_data_h << 8 | temp_data.temp_data_l;
    *temp = (float)temp_raw / 132.48 + 25;
}
#endif
