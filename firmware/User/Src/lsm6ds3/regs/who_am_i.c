#include "User/Inc/lsm6ds3/regs/who_am_i.h"

void lsm6ds3_reg_get_who_am_i(lsm6ds3_reg_who_am_i_t* who_am_i) {
    lsm6ds3_read_reg(LSM6DS3_REG_WHO_AM_I, &who_am_i->dev_id);
}
