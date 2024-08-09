#include "ads1262/regs/mode_0.h"

void ads1262_reg_set_mode_0(ads1262_reg_mode_0_t* mode_0) {
    uint8_t mode_0_data = __ADS1262_MODE_0_DEFAULT_VALUE;
    mode_0_data &= ~(0x01 << 7);
    mode_0_data |= (mode_0->ref_rev & 0x01) << 7;
    mode_0_data &= ~(0x0F << 5);
    mode_0_data |= (mode_0->delay & 0x0F) << 5;
    mode_0_data &= ~(0x03 << 2);
    mode_0_data |= (mode_0->chop & 0x03) << 2;
    mode_0_data &= ~(0x01 << 1);
    mode_0_data |= (mode_0->run_mode & 0x01) << 1;
    ads1262_write_reg(ADS1262_REG_MODE0, &mode_0_data);
}

void ads1262_reg_get_mode_0(ads1262_reg_mode_0_t* mode_0) {
    uint8_t mode_0_data = 0;
    ads1262_read_reg(ADS1262_REG_MODE0, &mode_0_data);
    mode_0->ref_rev = (mode_0_data & 0x80) >> 7;
    mode_0->delay = (mode_0_data & 0xF0) >> 5;
    mode_0->chop = (mode_0_data & 0x0C) >> 2;
    mode_0->run_mode = (mode_0_data & 0x02) >> 1;
}
