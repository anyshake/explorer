#include "User/Inc/lsm6dsr/lsb_per_g.h"

uint16_t lsm6dsr_get_lsb_per_g(void) {
    const uint16_t lsb_per_g_table[] = {16384, 2048, 4096, 8192};
    lsm6dsr_reg_ctrl1_xl_t lsm6dsr_reg_ctrl1_xl;
    lsm6dsr_reg_get_ctrl1_xl(&lsm6dsr_reg_ctrl1_xl);
    return lsb_per_g_table[lsm6dsr_reg_ctrl1_xl.fs_xl];
}
