#include "User/Inc/reader.h"

void get_adc_readout(ads1262_ctl_pin_t ctl_pin, int32_t arr[]) {
    ads1262_reg_inpmux_t inpmux = ads1262_reg_new_inpmux();

    ads1262_cmd_rdata_t rdata;
    inpmux.mux_p = ADS1262_REG_INPMUX_AIN0;
    inpmux.mux_n = ADS1262_REG_INPMUX_AIN1;
    ads1262_reg_set_inpmux(&inpmux);
    ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
    arr[0] = ads1262_cmd_rdata_is_valid(&rdata, ADS1262_REG_INTERFACE_CRC_CRC)
                 ? rdata.data
                 : 0;

    inpmux.mux_p = ADS1262_REG_INPMUX_AIN2;
    inpmux.mux_n = ADS1262_REG_INPMUX_AIN3;
    ads1262_reg_set_inpmux(&inpmux);
    ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
    arr[1] = ads1262_cmd_rdata_is_valid(&rdata, ADS1262_REG_INTERFACE_CRC_CRC)
                 ? rdata.data
                 : 0;

    inpmux.mux_p = ADS1262_REG_INPMUX_AIN4;
    inpmux.mux_n = ADS1262_REG_INPMUX_AIN5;
    ads1262_reg_set_inpmux(&inpmux);
    ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
    arr[2] = ads1262_cmd_rdata_is_valid(&rdata, ADS1262_REG_INTERFACE_CRC_CRC)
                 ? rdata.data
                 : 0;
}

#ifndef USE_LSM6DS3
void get_accel_readout(int16_t arr[], float* temp) {
    icm42688_reg_accel_data_x_t accel_data_x;
    icm42688_reg_accel_data_y_t accel_data_y;
    icm42688_reg_accel_data_z_t accel_data_z;

    icm42688_wait();

    icm42688_reg_get_accel_data_z(&accel_data_z);
    arr[0] = (int16_t)(accel_data_z.accel_data_z_h << 8 | accel_data_z.accel_data_z_l);

    icm42688_reg_get_accel_data_x(&accel_data_x);
    arr[1] = (int16_t)(accel_data_x.accel_data_x_h << 8 | accel_data_x.accel_data_x_l);

    icm42688_reg_get_accel_data_y(&accel_data_y);
    arr[2] = (int16_t)(accel_data_y.accel_data_y_h << 8 | accel_data_y.accel_data_y_l);

    if (temp != NULL) {
        icm42688_reg_temp_data_t temp_data;
        icm42688_reg_get_temp_data(&temp_data);
        int16_t temp_raw = temp_data.temp_data_h << 8 | temp_data.temp_data_l;
        *temp = (float)temp_raw / 132.48 + 25;
    }
}
#else
void get_accel_readout(int16_t arr[], float* temp) {
    lsm6ds3_reg_outz_xl_t outz_xl;
    lsm6ds3_reg_outx_xl_t outx_xl;
    lsm6ds3_reg_outy_xl_t outy_xl;

    lsm6ds3_wait();

    lsm6ds3_reg_get_outz_xl(&outz_xl);
    arr[0] = (int16_t)(outz_xl.outz_h_xl << 8 | outz_xl.outz_l_xl);

    lsm6ds3_reg_get_outx_xl(&outx_xl);
    arr[1] = (int16_t)(outx_xl.outx_h_xl << 8 | outx_xl.outx_l_xl);

    lsm6ds3_reg_get_outy_xl(&outy_xl);
    arr[2] = (int16_t)(outy_xl.outy_h_xl << 8 | outy_xl.outy_l_xl);

    if (temp != NULL) {
        lsm6ds3_reg_out_temp_t out_temp;
        lsm6ds3_reg_get_out_temp(&out_temp);
        int16_t temp_raw = out_temp.out_temp_h << 8 | out_temp.out_temp_l;
        *temp = (float)temp_raw / 16.0 + 25.0;
    }
}
#endif
