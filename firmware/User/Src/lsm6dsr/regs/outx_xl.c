#include "User/Inc/lsm6dsr/regs/outx_xl.h"

void lsm6dsr_reg_get_outx_xl(lsm6dsr_reg_outx_xl_t* outx_xl) {
    lsm6dsr_read_reg(LSM6DSR_REG_OUTX_L_XL, &outx_xl->outx_l_xl);
    lsm6dsr_read_reg(LSM6DSR_REG_OUTX_H_XL, &outx_xl->outx_h_xl);
}
