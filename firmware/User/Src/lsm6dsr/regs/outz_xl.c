#include "User/Inc/lsm6dsr/regs/outz_xl.h"

void lsm6dsr_reg_get_outz_xl(lsm6dsr_reg_outz_xl_t* outz_xl) {
    uint8_t data[2] = {0};
    lsm6dsr_read_regs(LSM6DSR_REG_OUTZ_L_XL, data, sizeof(data));
    outz_xl->outz_l_xl = data[0];
    outz_xl->outz_h_xl = data[1];
}
