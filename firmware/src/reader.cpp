#include "reader.h"

void get_adc_readout(ads1262_ctl_pin_t ctl_pin,
                     int32_array_t* channel_buffer,
                     uint8_t channel_samples,
                     bool is_24bit) {
    ads1262_reg_inpmux_t inpmux;
    ads1262_cmd_rdata_t rdata;

    for (uint8_t i = 0; i < channel_samples; i++) {
        // Read Z-axis geophone data (AIN0, AIN1)
        inpmux.mux_p = ADS1262_INPMUX_AIN0;
        inpmux.mux_n = ADS1262_INPMUX_AIN1;
        ads1262_reg_set_inpmux(&inpmux);
        ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
        if (is_24bit) {
            int32_t temp = (int32_t)((rdata.data >> 8) & 0xFFFFFF);
            channel_buffer->data[i] =
                temp & 0x800000 ? temp | 0xFF000000 : temp;
        } else {
            channel_buffer->data[i] = rdata.data;
        }

        // Read E-axis geophone data (AIN2, AIN3)
        inpmux.mux_p = ADS1262_INPMUX_AIN2;
        inpmux.mux_n = ADS1262_INPMUX_AIN3;
        ads1262_reg_set_inpmux(&inpmux);
        ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
        if (is_24bit) {
            int32_t temp = (int32_t)((rdata.data >> 8) & 0xFFFFFF);
            channel_buffer->data[i + channel_samples] =
                temp & 0x800000 ? temp | 0xFF000000 : temp;
        } else {
            channel_buffer->data[i + channel_samples] = rdata.data;
        }

        // Read N-axis geophone data (AIN4, AIN5)
        inpmux.mux_p = ADS1262_INPMUX_AIN4;
        inpmux.mux_n = ADS1262_INPMUX_AIN5;
        ads1262_reg_set_inpmux(&inpmux);
        ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
        if (is_24bit) {
            int32_t temp = (int32_t)((rdata.data >> 8) & 0xFFFFFF);
            channel_buffer->data[i + 2 * channel_samples] =
                temp & 0x800000 ? temp | 0xFF000000 : temp;
        } else {
            channel_buffer->data[i + 2 * channel_samples] = rdata.data;
        }
    }
}
