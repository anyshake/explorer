#ifndef __ICM42688_UTILS_H
#define __ICM42688_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/icm42688/regs/device_config.h"
#include "User/Inc/icm42688/regs/int_status.h"
#include "Utils/Inc/delay.h"
#include "Utils/Inc/gpio.h"
#include "Utils/Inc/i2c.h"

#define ICM42688_I2C_ADDRES 0x69

typedef struct {
    mcu_utils_gpio_t int_1;
    mcu_utils_gpio_t int_2;
} icm42688_ints_pin_t;

void icm42688_wait(void);
void icm42688_init(icm42688_ints_pin_t pin);
void icm42688_reset(bool is_rtos);
void icm42688_read_reg(uint8_t reg, uint8_t* rx_data);
void icm42688_write_reg(uint8_t reg, uint8_t tx_data);

#endif
