#include "User/Inc/lsm6dsr/regs/out_temp.h"

void lsm6dsr_reg_get_out_temp(lsm6dsr_reg_out_temp_t* out_temp) {
    lsm6dsr_read_reg(LSM6DSR_REG_OUT_TEMP_L, &out_temp->out_temp_l);
    lsm6dsr_read_reg(LSM6DSR_REG_OUT_TEMP_H, &out_temp->out_temp_h);
}
