#include "User/Inc/icm42688/regs/int_status.h"

void icm42688_reg_get_int_status(icm42688_reg_int_status_t* int_status) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    uint8_t int_status_data = 0;
    icm42688_read_reg(ICM42688_REG_INT_STATUS, &int_status_data);
    int_status->ui_fsync_int = (int_status_data >> 6) & 0x01;
    int_status->pll_rdy_int = (int_status_data >> 5) & 0x01;
    int_status->reset_done_int = (int_status_data >> 4) & 0x01;
    int_status->data_rdy_int = (int_status_data >> 3) & 0x01;
    int_status->fifo_ths_int = (int_status_data >> 2) & 0x01;
    int_status->fifo_full_int = (int_status_data >> 1) & 0x01;
    int_status->agc_rdy_int = int_status_data & 0x01;
}
