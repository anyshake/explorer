#include "ads1262/regs/power.hpp"

void adc_reg_set_power(adc_reg_power_t* power) {
    uint8_t power_data = __ADC_POWER_DEFAULT_VALUE;
    power_data &= ~(0x01 << 4);
    power_data |= (power->reset & 0x01) << 4;
    power_data &= ~(0x01 << 1);
    power_data |= (power->vbias & 0x01) << 1;
    power_data &= ~0x01;
    power_data |= power->intref & 0x01;
    adc_write_reg(ADC_REG_POWER, &power_data);
}

void adc_reg_get_power(adc_reg_power_t* power) {
    uint8_t power_data = 0;
    adc_read_reg(ADC_REG_POWER, &power_data);
    power->reset = (power_data & 0x10) >> 4;
    power->vbias = (power_data & 0x02) >> 1;
    power->intref = power_data & 0x01;
}
