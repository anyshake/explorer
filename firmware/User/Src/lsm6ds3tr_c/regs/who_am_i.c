#include "User/Inc/lsm6ds3tr_c/regs/who_am_i.h"

void lsm6ds3tr_c_reg_get_who_am_i(lsm6ds3tr_c_reg_who_am_i_t* who_am_i) {
    lsm6ds3tr_c_read_reg(LSM6DS3TR_C_REG_WHO_AM_I, &who_am_i->dev_id);
}
