#include "User/Inc/reader.h"

void get_adc_readout(ads1262_ctl_pin_t ctl_pin,
                     int32_array_t* channel_buffer,
                     uint8_t channel_samples) {
    ads1262_reg_inpmux_t inpmux = ads1262_reg_new_inpmux();

    ads1262_cmd_rdata_t rdata;
    for (uint8_t i = 0; i < channel_samples; i++) {
        // Read Z-axis geophone data (AIN0, AIN1)
        inpmux.mux_p = ADS1262_REG_INPMUX_AIN0;
        inpmux.mux_n = ADS1262_REG_INPMUX_AIN1;
        ads1262_reg_set_inpmux(&inpmux);
        ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
        channel_buffer->data[i] = ads1262_cmd_rdata_is_valid(&rdata, ADS1262_REG_INTERFACE_CRC_CRC)
                                      ? rdata.data
                                      : 0;

        // Read E-axis geophone data (AIN2, AIN3)
        inpmux.mux_p = ADS1262_REG_INPMUX_AIN2;
        inpmux.mux_n = ADS1262_REG_INPMUX_AIN3;
        ads1262_reg_set_inpmux(&inpmux);
        ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
        channel_buffer->data[i + channel_samples] = ads1262_cmd_rdata_is_valid(&rdata, ADS1262_REG_INTERFACE_CRC_CRC)
                                                        ? rdata.data
                                                        : 0;

        // Read N-axis geophone data (AIN4, AIN5)
        inpmux.mux_p = ADS1262_REG_INPMUX_AIN4;
        inpmux.mux_n = ADS1262_REG_INPMUX_AIN5;
        ads1262_reg_set_inpmux(&inpmux);
        ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
        channel_buffer->data[i + 2 * channel_samples] = ads1262_cmd_rdata_is_valid(&rdata, ADS1262_REG_INTERFACE_CRC_CRC)
                                                            ? rdata.data
                                                            : 0;
    }
}

#ifndef USE_LSM6DS3
void get_accel_readout(icm42688_ints_pin_t ints_pin,
                       int32_array_t* channel_buffer,
                       uint8_t channel_samples) {
    icm42688_reg_accel_data_x_t accel_data_x;
    icm42688_reg_accel_data_y_t accel_data_y;
    icm42688_reg_accel_data_z_t accel_data_z;

    for (uint8_t i = 0; i < channel_samples; i++) {
        // Wait for ICM-42688 data ready
        icm42688_wait(ints_pin);

        // Read Z-axis accelerometer data
        icm42688_reg_get_accel_data_z(&accel_data_z);
        channel_buffer->data[i] = (int16_t)(accel_data_z.accel_data_z_h << 8 | accel_data_z.accel_data_z_l);

        // Read E-axis accelerometer data
        icm42688_reg_get_accel_data_x(&accel_data_x);
        channel_buffer->data[i + channel_samples] = (int16_t)(accel_data_x.accel_data_x_h << 8 | accel_data_x.accel_data_x_l);

        // Read N-axis accelerometer data
        icm42688_reg_get_accel_data_y(&accel_data_y);
        channel_buffer->data[i + 2 * channel_samples] = (int16_t)(accel_data_y.accel_data_y_h << 8 | accel_data_y.accel_data_y_l);
    }
}

void get_gyro_readout(icm42688_ints_pin_t ints_pin, int16_t gyro_data[3]) {
    icm42688_reg_gyro_data_x_t gyro_data_x;
    icm42688_reg_gyro_data_y_t gyro_data_y;
    icm42688_reg_gyro_data_z_t gyro_data_z;

    // Wait for ICM-42688 data ready
    icm42688_wait(ints_pin);

    // Read Z-axis gyroscope data
    icm42688_reg_get_gyro_data_z(&gyro_data_z);
    gyro_data[0] = (int16_t)(gyro_data_z.gyro_data_z_h << 8 | gyro_data_z.gyro_data_z_l);

    // Read E-axis gyroscope data
    icm42688_reg_get_gyro_data_x(&gyro_data_x);
    gyro_data[1] = (int16_t)(gyro_data_x.gyro_data_x_h << 8 | gyro_data_x.gyro_data_x_l);

    // Read N-axis gyroscope data
    icm42688_reg_get_gyro_data_y(&gyro_data_y);
    gyro_data[2] = (int16_t)(gyro_data_y.gyro_data_y_h << 8 | gyro_data_y.gyro_data_y_l);
}
#else
void get_accel_readout(lsm6ds3_ints_pin_t ints_pin,
                       int32_array_t* channel_buffer,
                       uint8_t channel_samples) {
    lsm6ds3_reg_outz_xl_t outz_xl;
    lsm6ds3_reg_outx_xl_t outx_xl;
    lsm6ds3_reg_outy_xl_t outy_xl;

    for (uint8_t i = 0; i < channel_samples; i++) {
        // Wait for LSM6DS3 data ready
        lsm6ds3_wait(ints_pin);

        // Read Z-axis accelerometer data
        lsm6ds3_reg_get_outz_xl(&outz_xl);
        channel_buffer->data[i] = (int16_t)(outz_xl.outz_h_xl << 8 | outz_xl.outz_l_xl);

        // Read E-axis accelerometer data
        lsm6ds3_reg_get_outx_xl(&outx_xl);
        channel_buffer->data[i + channel_samples] = (int16_t)(outx_xl.outx_h_xl << 8 | outx_xl.outx_l_xl);

        // Read N-axis accelerometer data
        lsm6ds3_reg_get_outy_xl(&outy_xl);
        channel_buffer->data[i + 2 * channel_samples] = (int16_t)(outy_xl.outy_h_xl << 8 | outy_xl.outy_l_xl);
    }
}

void get_gyro_readout(lsm6ds3_ints_pin_t ints_pin, int16_t gyro_data[3]) {
    ;
}
#endif
