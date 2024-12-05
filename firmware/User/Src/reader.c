#include "User/Inc/reader.h"

void get_adc_readout(ads1262_ctl_pin_t ctl_pin,
                     int32_array_t* channel_buffer,
                     uint8_t channel_samples) {
    ads1262_reg_inpmux_t inpmux;
    ads1262_cmd_rdata_t rdata;

    for (uint8_t i = 0; i < channel_samples; i++) {
        // Read Z-axis geophone data (AIN0, AIN1)
        inpmux.mux_p = ADS1262_INPMUX_AIN1;
        inpmux.mux_n = ADS1262_INPMUX_AIN0;
        ads1262_reg_set_inpmux(&inpmux);
        ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
        channel_buffer->data[i] = rdata.data;

        // Read E-axis geophone data (AIN2, AIN3)
        inpmux.mux_p = ADS1262_INPMUX_AIN3;
        inpmux.mux_n = ADS1262_INPMUX_AIN2;
        ads1262_reg_set_inpmux(&inpmux);
        ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
        channel_buffer->data[i + channel_samples] = rdata.data;

        // Read N-axis geophone data (AIN4, AIN5)
        inpmux.mux_p = ADS1262_INPMUX_AIN5;
        inpmux.mux_n = ADS1262_INPMUX_AIN4;
        ads1262_reg_set_inpmux(&inpmux);
        ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
        channel_buffer->data[i + 2 * channel_samples] = rdata.data;
    }
}

void get_acc_readout(lsm6ds3_ints_pin_t ints_pin,
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
        channel_buffer->data[i] =
            (int16_t)(outz_xl.outz_h_xl << 8 | outz_xl.outz_l_xl);

        // Read E-axis accelerometer data
        lsm6ds3_reg_get_outx_xl(&outx_xl);
        channel_buffer->data[i + channel_samples] =
            (int16_t)(outx_xl.outx_h_xl << 8 | outx_xl.outx_l_xl);

        // Read N-axis accelerometer data
        lsm6ds3_reg_get_outy_xl(&outy_xl);
        channel_buffer->data[i + 2 * channel_samples] =
            (int16_t)(outy_xl.outy_h_xl << 8 | outy_xl.outy_l_xl);
    }
}
