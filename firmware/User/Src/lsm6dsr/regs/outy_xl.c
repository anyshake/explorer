#include "User/Inc/lsm6dsr/regs/outy_xl.h"

void lsm6dsr_reg_get_outy_xl(lsm6dsr_reg_outy_xl_t* outy_xl) {
    lsm6dsr_read_reg(LSM6DSR_REG_OUTY_L_XL, &outy_xl->outy_l_xl);
    lsm6dsr_read_reg(LSM6DSR_REG_OUTY_H_XL, &outy_xl->outy_h_xl);
}