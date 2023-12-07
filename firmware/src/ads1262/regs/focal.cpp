#include "ads1262/regs/ofcal.hpp"

void adc_reg_set_ofcal(adc_reg_ofcal_t* ofcal) {
    uint8_t ofcal_0_data = (__ADC_OFCAL_DEFAULT_VALUE >> 16) & 0xFF;
    ofcal_0_data &= ~(0x01 << 7);
    ofcal_0_data |= (ofcal->ofcal_0 & 0x01) << 7;
    adc_write_reg(ADC_REG_OFCAL0, &ofcal_0_data);
    uint8_t ofcal_1_data = (__ADC_OFCAL_DEFAULT_VALUE >> 8) & 0xFF;
    ofcal_1_data &= ~(0x01 << 7);
    ofcal_1_data |= (ofcal->ofcal_1 & 0x01) << 7;
    adc_write_reg(ADC_REG_OFCAL1, &ofcal_1_data);
    uint8_t ofcal_2_data = __ADC_OFCAL_DEFAULT_VALUE & 0xFF;
    ofcal_2_data &= ~(0x01 << 7);
    ofcal_2_data |= (ofcal->ofcal_2 & 0x01) << 7;
    adc_write_reg(ADC_REG_OFCAL2, &ofcal_2_data);
}

void adc_reg_get_ofcal(adc_reg_ofcal_t* ofcal) {
    uint8_t ofcal_0_data = 0;
    adc_read_reg(ADC_REG_OFCAL0, &ofcal_0_data);
    ofcal->ofcal_0 = ofcal_0_data;
    uint8_t ofcal_1_data = 0;
    adc_read_reg(ADC_REG_OFCAL1, &ofcal_1_data);
    ofcal->ofcal_1 = ofcal_1_data;
    uint8_t ofcal_2_data = 0;
    adc_read_reg(ADC_REG_OFCAL2, &ofcal_2_data);
    ofcal->ofcal_2 = ofcal_2_data;
}
