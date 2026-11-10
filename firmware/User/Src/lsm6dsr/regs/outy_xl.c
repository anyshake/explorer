#include "User/Inc/lsm6dsr/regs/outy_xl.h"

void lsm6dsr_reg_get_outy_xl(lsm6dsr_reg_outy_xl_t* outy_xl) {
    uint8_t data[2] = {0};
    lsm6dsr_read_regs(LSM6DSR_REG_OUTY_L_XL, data, sizeof(data));
    outy_xl->outy_l_xl = data[0];
    outy_xl->outy_h_xl = data[1];
}
