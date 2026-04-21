#ifndef __LSM6DSR_UTILS_H
#define __LSM6DSR_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/lsm6dsr/regs/ctrl3_c.h"
#include "User/Inc/lsm6dsr/regs/int1_ctrl.h"
#include "User/Inc/lsm6dsr/regs/status_reg.h"
#include "Utils/Inc/delay.h"
#include "Utils/Inc/gpio.h"
#include "Utils/Inc/i2c.h"

#define LSM6DSR_I2C_ADDRES 0x6A

typedef struct {
    mcu_utils_gpio_t int_1;
    mcu_utils_gpio_t int_2;
} lsm6dsr_ints_pin_t;

void lsm6dsr_wait(void);
void lsm6dsr_init(lsm6dsr_ints_pin_t pin);
void lsm6dsr_reset(bool is_rtos);
void lsm6dsr_read_reg(uint8_t reg, uint8_t* rx_data);
void lsm6dsr_write_reg(uint8_t reg, uint8_t tx_data);

#endif
