#include "ads1262/regs/inpmux.h"

ads1262_reg_inpmux_t ads1262_reg_new_inpmux(void) {
    ads1262_reg_inpmux_t inpmux;
    inpmux.mux_p = ADS1262_REG_INPMUX_AIN0;
    inpmux.mux_n = ADS1262_REG_INPMUX_AIN1;
    return inpmux;
}

void ads1262_reg_set_inpmux(ads1262_reg_inpmux_t* inpmux) {
    uint8_t inpmux_data = __ADS1262_REG_INPMUX_DEFAULT_VALUE;
    inpmux_data &= ~(0x0F << 4);
    inpmux_data |= (inpmux->mux_n & 0x0F) << 4;
    inpmux_data &= ~0x0F;
    inpmux_data |= (inpmux->mux_p & 0x0F);
    ads1262_write_reg(ADS1262_REG_INPMUX, &inpmux_data);
}

void ads1262_reg_get_inpmux(ads1262_reg_inpmux_t* inpmux) {
    uint8_t inpmux_data = 0;
    ads1262_read_reg(ADS1262_REG_INPMUX, &inpmux_data);
    inpmux->mux_n = (inpmux_data >> 4) & 0x0F;
    inpmux->mux_p = inpmux_data & 0x0F;
}
