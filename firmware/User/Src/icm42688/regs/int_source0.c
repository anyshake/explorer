#include "User/Inc/icm42688/regs/int_source0.h"

icm42688_reg_int_source0_t icm42688_reg_new_int_source0(void) {
    icm42688_reg_int_source0_t int_source0;
    int_source0.ui_fsync_int1_en = ICM42688_REG_INT_SOURCE0_UI_FSYNC_INT1_EN_DISABLED;
    int_source0.pll_rdy_int1_en = ICM42688_REG_INT_SOURCE0_PLL_RDY_INT1_EN_DISABLED;
    int_source0.reset_done_int1_en = ICM42688_REG_INT_SOURCE0_RESET_DONE_INT1_EN_ENABLED;
    int_source0.ui_drdy_int1_en = ICM42688_REG_INT_SOURCE0_UI_DRDY_INT1_EN_DISABLED;
    int_source0.fifo_ths_int1_en = ICM42688_REG_INT_SOURCE0_FIFO_THS_INT1_EN_DISABLED;
    int_source0.fifo_full_int1_en = ICM42688_REG_INT_SOURCE0_FIFO_FULL_INT1_EN_DISABLED;
    int_source0.ui_agc_rdy_int1_en = ICM42688_REG_INT_SOURCE0_UI_AGC_RDY_INT1_EN_DISABLED;
    return int_source0;
}

void icm42688_reg_set_int_source0(icm42688_reg_int_source0_t* int_source0) {
    uint8_t int_source0_data = __ICM42688_REG_INT_SOURCE0_DEFAULT_VALUE;
    int_source0_data &= ~(0x01 << 6);
    int_source0_data |= (int_source0->ui_fsync_int1_en & 0x01) << 6;
    int_source0_data &= ~(0x01 << 5);
    int_source0_data |= (int_source0->pll_rdy_int1_en & 0x01) << 5;
    int_source0_data &= ~(0x01 << 4);
    int_source0_data |= (int_source0->reset_done_int1_en & 0x01) << 4;
    int_source0_data &= ~(0x01 << 3);
    int_source0_data |= (int_source0->ui_drdy_int1_en & 0x01) << 3;
    int_source0_data &= ~(0x01 << 2);
    int_source0_data |= (int_source0->fifo_ths_int1_en & 0x01) << 2;
    int_source0_data &= ~(0x01 << 1);
    int_source0_data |= (int_source0->fifo_full_int1_en & 0x01) << 1;
    int_source0_data &= ~0x01;
    int_source0_data |= (int_source0->ui_agc_rdy_int1_en & 0x01);
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    icm42688_write_reg(ICM42688_REG_INT_SOURCE0, int_source0_data);
}

void icm42688_reg_get_int_source0(icm42688_reg_int_source0_t* int_source0) {
    icm42688_write_reg(ICM42688_REG_REG_BANK_SEL, 0);
    uint8_t int_source0_data = 0;
    icm42688_read_reg(ICM42688_REG_INT_SOURCE0, &int_source0_data);
    int_source0->ui_fsync_int1_en = (int_source0_data >> 6) & 0x01;
    int_source0->pll_rdy_int1_en = (int_source0_data >> 5) & 0x01;
    int_source0->reset_done_int1_en = (int_source0_data >> 4) & 0x01;
    int_source0->ui_drdy_int1_en = (int_source0_data >> 3) & 0x01;
    int_source0->fifo_ths_int1_en = (int_source0_data >> 2) & 0x01;
    int_source0->fifo_full_int1_en = (int_source0_data >> 1) & 0x01;
    int_source0->ui_agc_rdy_int1_en = int_source0_data & 0x01;
}
