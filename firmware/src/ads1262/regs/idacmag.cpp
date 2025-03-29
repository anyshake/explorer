#include "ads1262/regs/idacmag.h"

ads1262_reg_idacmag_t ads1262_reg_new_idacmag(void) {
    ads1262_reg_idacmag_t idacmag;
    idacmag.mag_2 = ADS1262_REG_IDACMAG_DISABLED;
    idacmag.mag_1 = ADS1262_REG_IDACMAG_DISABLED;
    return idacmag;
}

void ads1262_reg_set_idacmag(ads1262_reg_idacmag_t* idacmag) {
    uint8_t idacmag_data = __ADS1262_REG_IDACMAG_DEFAULT_VALUE;
    idacmag_data &= ~(0x0F << 4);
    idacmag_data |= (idacmag->mag_2 & 0x0F) << 4;
    idacmag_data &= ~0x0F;
    idacmag_data |= (idacmag->mag_1 & 0x0F);
    ads1262_write_reg(ADS1262_REG_IDACMAG, &idacmag_data);
}

void ads1262_reg_get_idacmag(ads1262_reg_idacmag_t* idacmag) {
    uint8_t idacmag_data = 0;
    ads1262_read_reg(ADS1262_REG_IDACMAG, &idacmag_data);
    idacmag->mag_2 = (idacmag_data >> 4) & 0x0F;
    idacmag->mag_1 = idacmag_data & 0x0F;
}
