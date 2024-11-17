#include "User/Inc/ads1262/regs/mode_1.h"

void ads1262_reg_set_mode_1(ads1262_reg_mode_1_t* mode_1) {
    uint8_t mode_1_data = __ADS1262_MODE_1_DEFAULT_VALUE;
    mode_1_data &= ~(0x07 << 6);
    mode_1_data |= (mode_1->filter & 0x07) << 6;
    mode_1_data &= ~(0x01 << 4);
    mode_1_data |= (mode_1->sb_adc & 0x01) << 4;
    mode_1_data &= ~(0x01 << 3);
    mode_1_data |= (mode_1->sb_pol & 0x01) << 3;
    mode_1_data &= ~0x07;
    mode_1_data |= (mode_1->sb_mag & 0x07);
    ads1262_write_reg(ADS1262_REG_MODE1, &mode_1_data);
}

void ads1262_reg_get_mode_1(ads1262_reg_mode_1_t* mode_1) {
    uint8_t mode_1_data;
    ads1262_read_reg(ADS1262_REG_MODE1, &mode_1_data);
    mode_1->filter = (mode_1_data & 0xC0) >> 6;
    mode_1->sb_adc = (mode_1_data & 0x10) >> 4;
    mode_1->sb_pol = (mode_1_data & 0x08) >> 3;
    mode_1->sb_mag = (mode_1_data & 0x07);
}
