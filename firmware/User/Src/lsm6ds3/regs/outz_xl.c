#include "User/Inc/lsm6ds3/regs/outz_xl.h"

void lsm6ds3_reg_get_outz_xl(lsm6ds3_reg_outz_xl_t* outz_xl) {
    lsm6ds3_read_reg(LSM6DS3_REG_OUTZ_L_XL, &outz_xl->outz_l_xl);
    lsm6ds3_read_reg(LSM6DS3_REG_OUTZ_H_XL, &outz_xl->outz_h_xl);
}
