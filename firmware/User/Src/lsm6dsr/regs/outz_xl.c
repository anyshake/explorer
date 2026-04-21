#include "User/Inc/lsm6dsr/regs/outz_xl.h"

void lsm6dsr_reg_get_outz_xl(lsm6dsr_reg_outz_xl_t* outz_xl) {
    lsm6dsr_read_reg(LSM6DSR_REG_OUTZ_L_XL, &outz_xl->outz_l_xl);
    lsm6dsr_read_reg(LSM6DSR_REG_OUTZ_H_XL, &outz_xl->outz_h_xl);
}
