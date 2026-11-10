#include "User/Inc/lsm6ds3/regs/outz_xl.h"

void lsm6ds3_reg_get_outz_xl(lsm6ds3_reg_outz_xl_t* outz_xl) {
    uint8_t data[2] = {0};
    lsm6ds3_read_regs(LSM6DS3_REG_OUTZ_L_XL, data, sizeof(data));
    outz_xl->outz_l_xl = data[0];
    outz_xl->outz_h_xl = data[1];
}
