#include "User/Inc/lsm6ds3tr_c/regs/outy_xl.h"

void lsm6ds3tr_c_reg_get_outy_xl(lsm6ds3tr_c_reg_outy_xl_t* outy_xl) {
    uint8_t data[2] = {0};
    lsm6ds3tr_c_read_regs(LSM6DS3TR_C_REG_OUTY_L_XL, data, sizeof(data));
    outy_xl->outy_l_xl = data[0];
    outy_xl->outy_h_xl = data[1];
}
