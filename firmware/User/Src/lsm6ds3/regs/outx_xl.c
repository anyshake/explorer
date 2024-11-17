#include "User/Inc/lsm6ds3/regs/outx_xl.h"

void lsm6ds3_reg_get_outx_xl(lsm6ds3_reg_outx_xl_t* outx_xl) {
    lsm6ds3_read_reg(LSM6DS3_REG_OUTX_L_XL, &outx_xl->outx_l_xl);
    lsm6ds3_read_reg(LSM6DS3_REG_OUTX_H_XL, &outx_xl->outx_h_xl);
}
