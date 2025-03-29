#include "reader.h"

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