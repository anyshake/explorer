#include "User/Inc/lsm6ds3/regs/outy_xl.h"

void lsm6ds3_reg_get_outy_xl(lsm6ds3_reg_outy_xl_t* outy_xl) {
    lsm6ds3_read_reg(LSM6DS3_REG_OUTY_L_XL, &outy_xl->outy_l_xl);
    lsm6ds3_read_reg(LSM6DS3_REG_OUTY_H_XL, &outy_xl->outy_h_xl);
}