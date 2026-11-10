#include "User/Inc/icm42688/regs/temp_data.h"

void icm42688_reg_get_temp_data(icm42688_reg_temp_data_t* temp_data) {
    uint8_t data[2] = {0};
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0x00);
    icm42688_read_regs(ICM42688_REG_TEMP_DATA1, data, sizeof(data));
    temp_data->temp_data_l = data[1];
    temp_data->temp_data_h = data[0];
}
