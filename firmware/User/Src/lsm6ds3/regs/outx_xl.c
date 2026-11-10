#include "User/Inc/lsm6ds3/regs/outx_xl.h"

void lsm6ds3_reg_get_outx_xl(lsm6ds3_reg_outx_xl_t* outx_xl) {
    uint8_t data[2] = {0};
    lsm6ds3_read_regs(LSM6DS3_REG_OUTX_L_XL, data, sizeof(data));
    outx_xl->outx_l_xl = data[0];
    outx_xl->outx_h_xl = data[1];
}
