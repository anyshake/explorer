#include "User/Inc/icm42688/regs/temp_data.h"

void icm42688_reg_get_temp_data(icm42688_reg_temp_data_t* temp_data) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_read_reg(ICM42688_REG_TEMP_DATA0, &temp_data->temp_data_l);
    icm42688_read_reg(ICM42688_REG_TEMP_DATA1, &temp_data->temp_data_h);
}
