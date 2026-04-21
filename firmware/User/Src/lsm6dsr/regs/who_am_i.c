#include "User/Inc/lsm6dsr/regs/who_am_i.h"

void lsm6dsr_reg_get_who_am_i(lsm6dsr_reg_who_am_i_t* who_am_i) {
    lsm6dsr_read_reg(LSM6DSR_REG_WHO_AM_I, &who_am_i->dev_id);
}
