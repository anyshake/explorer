#include "User/Inc/lsm6ds3tr_c/regs/outz_xl.h"

void lsm6ds3tr_c_reg_get_outz_xl(lsm6ds3tr_c_reg_outz_xl_t* outz_xl) {
    uint8_t data[2] = {0};
    lsm6ds3tr_c_read_regs(LSM6DS3TR_C_REG_OUTZ_L_XL, data, sizeof(data));
    outz_xl->outz_l_xl = data[0];
    outz_xl->outz_h_xl = data[1];
}
