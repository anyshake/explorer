#include "ads1262/regs/power.h"

ads1262_reg_power_t ads1262_reg_new_power(void) {
    ads1262_reg_power_t power;
    power.reset = ADS1262_REG_POWER_RESET_OCCURRED;
    power.vbias = ADS1262_REG_POWER_VBIAS_DISABLED;
    power.intref = ADS1262_REG_POWER_INTREF_ENABLED;
    return power;
}

void ads1262_reg_set_power(ads1262_reg_power_t* power) {
    uint8_t power_data = __ADS1262_REG_POWER_DEFAULT_VALUE;
    power_data &= ~(0x01 << 4);
    power_data |= (power->reset & 0x01) << 4;
    power_data &= ~(0x01 << 1);
    power_data |= (power->vbias & 0x01) << 1;
    power_data &= ~0x01;
    power_data |= (power->intref & 0x01);
    ads1262_write_reg(ADS1262_REG_POWER, &power_data);
}

void ads1262_reg_get_power(ads1262_reg_power_t* power) {
    uint8_t power_data = 0;
    ads1262_read_reg(ADS1262_REG_POWER, &power_data);
    power->reset = (power_data >> 4) & 0x01;
    power->vbias = (power_data >> 1) & 0x01;
    power->intref = power_data & 0x01;
}
