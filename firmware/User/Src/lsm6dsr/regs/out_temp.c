#include "User/Inc/lsm6dsr/regs/out_temp.h"

void lsm6dsr_reg_get_out_temp(lsm6dsr_reg_out_temp_t* out_temp) {
    uint8_t data[2] = {0};
    lsm6dsr_read_regs(LSM6DSR_REG_OUT_TEMP_L, data, sizeof(data));
    out_temp->out_temp_l = data[0];
    out_temp->out_temp_h = data[1];
}
