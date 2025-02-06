#include "User/Inc/lsm6ds3/regs/out_temp.h"

void lsm6ds3_reg_get_out_temp(lsm6ds3_reg_out_temp_t* out_temp) {
    lsm6ds3_read_reg(LSM6DS3_REG_OUT_TEMP_L, &out_temp->out_temp_l);
    lsm6ds3_read_reg(LSM6DS3_REG_OUT_TEMP_H, &out_temp->out_temp_h);
}
