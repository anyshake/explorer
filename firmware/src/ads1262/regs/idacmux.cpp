#include "ads1262/regs/idacmux.h"

ads1262_reg_idacmux_t ads1262_reg_new_idacmux(void) {
    ads1262_reg_idacmux_t idacmux;
    idacmux.mux_2 = ADS1262_REG_IDACMUX_NOCONN;
    idacmux.mux_1 = ADS1262_REG_IDACMUX_NOCONN;
    return idacmux;
}

void ads1262_reg_set_idacmux(ads1262_reg_idacmux_t* idacmux) {
    uint8_t idacmux_data = __ADS1262_REG_IDACMUX_DEFAULT_VALUE;
    idacmux_data &= ~(0x0F << 4);
    idacmux_data |= (idacmux->mux_2 & 0x0F) << 4;
    idacmux_data &= ~0x0F;
    idacmux_data |= (idacmux->mux_1 & 0x0F);
    ads1262_write_reg(ADS1262_REG_IDACMUX, &idacmux_data);
}

void ads1262_reg_get_idacmux(ads1262_reg_idacmux_t* idacmux) {
    uint8_t idacmux_data = 0;
    ads1262_read_reg(ADS1262_REG_IDACMUX, &idacmux_data);
    idacmux->mux_2 = (idacmux_data >> 4) & 0x0F;
    idacmux->mux_1 = idacmux_data & 0x0F;
}
