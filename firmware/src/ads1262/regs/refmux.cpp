#include "ads1262/regs/refmux.h"

ads1262_reg_refmux_t ads1262_reg_new_refmux(void) {
    ads1262_reg_refmux_t refmux;
    refmux.refmux_p = ADS1262_REG_REFMUX_INTREF;
    refmux.refmux_n = ADS1262_REG_REFMUX_INTREF;
    return refmux;
}

void ads1262_reg_set_refmux(ads1262_reg_refmux_t* refmux) {
    uint8_t refmux_data = __ADS1262_REG_REFMUX_DEFAULT_VALUE;
    refmux_data &= ~(0x07 << 3);
    refmux_data |= (refmux->refmux_p & 0x07) << 3;
    refmux_data &= ~0x07;
    refmux_data |= (refmux->refmux_n & 0x07);
    ads1262_write_reg(ADS1262_REG_REFMUX, &refmux_data);
}

void ads1262_reg_get_refmux(ads1262_reg_refmux_t* refmux) {
    uint8_t refmux_data = 0;
    ads1262_read_reg(ADS1262_REG_REFMUX, &refmux_data);
    refmux->refmux_p = (refmux_data >> 3) & 0x07;
    refmux->refmux_n = refmux_data & 0x07;
}
