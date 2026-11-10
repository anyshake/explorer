#include "User/Inc/lsm6ds3tr_c/regs/out_temp.h"

void lsm6ds3tr_c_reg_get_out_temp(lsm6ds3tr_c_reg_out_temp_t* out_temp) {
    uint8_t data[2] = {0};
    lsm6ds3tr_c_read_regs(LSM6DS3TR_C_REG_OUT_TEMP_L, data, sizeof(data));
    out_temp->out_temp_l = data[0];
    out_temp->out_temp_h = data[1];
}
