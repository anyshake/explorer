#include "User/Inc/icm42688/regs/who_am_i.h"

void icm42688_reg_get_who_am_i(icm42688_reg_who_am_i_t* who_am_i) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_read_reg(ICM42688_REG_WHO_AM_I, &who_am_i->dev_id);
}
