#include "User/Inc/lsm6dsr/regs/master_config.h"

lsm6dsr_reg_master_config_t lsm6dsr_reg_new_master_config(void) {
    lsm6dsr_reg_master_config_t master_config;
    master_config.rst_master_regs = LSM6DSR_REG_MASTER_CONFIG_RST_MASTER_REGS_NOP;
    master_config.write_once = LSM6DSR_REG_MASTER_CONFIG_WRITE_ONCE_EACH_CYCLE;
    master_config.start_config = LSM6DSR_REG_MASTER_CONFIG_START_CONFIG_SENSOR_DRDY;
    master_config.pass_through_mode = LSM6DSR_REG_MASTER_CONFIG_PASS_THROUGH_MODE_DISABLED;
    master_config.shub_pu_en = LSM6DSR_REG_MASTER_CONFIG_SHUB_PU_EN_DISABLED;
    master_config.master_on = LSM6DSR_REG_MASTER_CONFIG_MASTER_ON_DISABLED;
    master_config.aux_sens_on = LSM6DSR_REG_MASTER_CONFIG_AUX_SENS_ON_1_SENSOR;
    return master_config;
}

void lsm6dsr_reg_set_master_config(lsm6dsr_reg_master_config_t* master_config) {
    uint8_t master_config_data = __LSM6DSR_REG_MASTER_CONFIG_DEFAULT_VALUE;
    master_config_data &= ~(0x01 << 7);
    master_config_data |= (master_config->rst_master_regs & 0x01) << 7;
    master_config_data &= ~(0x01 << 6);
    master_config_data |= (master_config->write_once & 0x01) << 6;
    master_config_data &= ~(0x01 << 5);
    master_config_data |= (master_config->start_config & 0x01) << 5;
    master_config_data &= ~(0x01 << 4);
    master_config_data |= (master_config->pass_through_mode & 0x01) << 4;
    master_config_data &= ~(0x01 << 3);
    master_config_data |= (master_config->shub_pu_en & 0x01) << 3;
    master_config_data &= ~(0x01 << 2);
    master_config_data |= (master_config->master_on & 0x01) << 2;
    master_config_data &= ~(0x03 << 1);
    master_config_data |= (master_config->aux_sens_on & 0x03) << 1;
    lsm6dsr_write_reg(LSM6DSR_REG_MASTER_CONFIG, master_config_data);
}

void lsm6dsr_reg_get_master_config(lsm6dsr_reg_master_config_t* master_config) {
    uint8_t master_config_data = 0;
    lsm6dsr_read_reg(LSM6DSR_REG_MASTER_CONFIG, &master_config_data);
    master_config->rst_master_regs = (master_config_data >> 7) & 0x01;
    master_config->write_once = (master_config_data >> 6) & 0x01;
    master_config->start_config = (master_config_data >> 5) & 0x01;
    master_config->pass_through_mode = (master_config_data >> 4) & 0x01;
    master_config->shub_pu_en = (master_config_data >> 3) & 0x01;
    master_config->master_on = (master_config_data >> 2) & 0x01;
    master_config->aux_sens_on = (master_config_data >> 1) & 0x03;
}
