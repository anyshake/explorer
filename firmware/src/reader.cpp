#include "reader.h"

void get_adc_readout(ads1262_ctl_pin_t ctl_pin,
                     int32_t adc_readout_z_axis[],
                     int32_t adc_readout_e_axis[],
                     int32_t adc_readout_n_axis[],
                     filter_state_t* filter_state_z_axis,
                     filter_state_t* filter_state_e_axis,
                     filter_state_t* filter_state_n_axis,
                     uint8_t channel_samples) {
    ads1262_reg_inpmux_t inpmux;
    ads1262_cmd_rdata_t rdata;

    int32_t z_readout[channel_samples];
    int32_t e_readout[channel_samples];
    int32_t n_readout[channel_samples];

    for (uint8_t i = 0; i < channel_samples; i++) {
        inpmux.mux_p = ADS1262_INPMUX_AIN0;
        inpmux.mux_n = ADS1262_INPMUX_AIN1;
        ads1262_reg_set_inpmux(&inpmux);
        ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
        z_readout[i] = ads1262_cmd_rdata_is_valid(
                           &rdata, ADS1262_INTERFACE_CRC_CRC)
                           ? rdata.data
                           : 0;

        inpmux.mux_p = ADS1262_INPMUX_AIN2;
        inpmux.mux_n = ADS1262_INPMUX_AIN3;
        ads1262_reg_set_inpmux(&inpmux);
        ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
        e_readout[i] = ads1262_cmd_rdata_is_valid(
                           &rdata, ADS1262_INTERFACE_CRC_CRC)
                           ? rdata.data
                           : 0;

        inpmux.mux_p = ADS1262_INPMUX_AIN4;
        inpmux.mux_n = ADS1262_INPMUX_AIN5;
        ads1262_reg_set_inpmux(&inpmux);
        ads1262_cmd_rdata(ctl_pin, &rdata, ADS1262_INIT_CONTROL_TYPE_HARD);
        n_readout[i] = ads1262_cmd_rdata_is_valid(
                           &rdata, ADS1262_INTERFACE_CRC_CRC)
                           ? rdata.data
                           : 0;
    }

#if ENABLE_COMPENSATION == true
    apply_compensation_filter(z_readout, channel_samples, filter_state_z_axis);
    apply_compensation_filter(e_readout, channel_samples, filter_state_e_axis);
    apply_compensation_filter(n_readout, channel_samples, filter_state_n_axis);
#endif

    for (uint8_t i = 0; i < channel_samples; i++) {
        adc_readout_z_axis[i] = z_readout[i];
        adc_readout_e_axis[i] = e_readout[i];
        adc_readout_n_axis[i] = n_readout[i];
    }
}
