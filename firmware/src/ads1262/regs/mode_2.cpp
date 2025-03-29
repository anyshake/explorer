#include "ads1262/regs/mode_2.h"

ads1262_reg_mode_2_t ads1262_reg_new_mode_2(void) {
    ads1262_reg_mode_2_t mode_2;
    mode_2.bypass = ADS1262_REG_MODE_2_BYPASS_ENABLED;
    mode_2.gain = ADS1262_REG_MODE_2_GAIN_1;
    mode_2.dr = ADS1262_REG_MODE_2_DR_20;
    return mode_2;
}

void ads1262_reg_set_mode_2(ads1262_reg_mode_2_t* mode_2) {
    uint8_t mode_2_data = __ADS1262_REG_MODE_2_DEFAULT_VALUE;
    mode_2_data &= ~(0x01 << 7);
    mode_2_data |= (mode_2->bypass & 0x01) << 7;
    mode_2_data &= ~(0x07 << 4);
    mode_2_data |= (mode_2->gain & 0x07) << 4;
    mode_2_data &= ~0x0F;
    mode_2_data |= (mode_2->dr & 0x0F);
    ads1262_write_reg(ADS1262_REG_MODE2, &mode_2_data);
}

void ads1262_reg_get_mode_2(ads1262_reg_mode_2_t* mode_2) {
    uint8_t mode_2_data = 0;
    ads1262_read_reg(ADS1262_REG_MODE2, &mode_2_data);
    mode_2->bypass = (mode_2_data >> 7) & 0x01;
    mode_2->gain = (mode_2_data >> 4) & 0x07;
    mode_2->dr = mode_2_data & 0x0F;
}
