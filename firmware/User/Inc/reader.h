#ifndef __READER_H
#define __READER_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/ads1262/cmds/rdata.h"
#include "User/Inc/ads1262/regs/inpmux.h"
#include "User/Inc/ads1262/utils.h"
#include "User/Inc/array.h"
#include "User/Inc/icm42688/regs/accel_data_x.h"
#include "User/Inc/icm42688/regs/accel_data_y.h"
#include "User/Inc/icm42688/regs/accel_data_z.h"
#include "User/Inc/icm42688/regs/gyro_data_x.h"
#include "User/Inc/icm42688/regs/gyro_data_y.h"
#include "User/Inc/icm42688/regs/gyro_data_z.h"
#include "User/Inc/icm42688/utils.h"
#include "User/Inc/lsm6ds3/regs/outx_xl.h"
#include "User/Inc/lsm6ds3/regs/outy_xl.h"
#include "User/Inc/lsm6ds3/regs/outz_xl.h"
#include "User/Inc/lsm6ds3/utils.h"

void get_adc_readout(ads1262_ctl_pin_t ctl_pin,
                     int32_array_t* channel_buffer,
                     uint8_t channel_samples);
#ifndef USE_LSM6DS3
void get_accel_readout(icm42688_ints_pin_t ints_pin,
                       int32_array_t* channel_buffer,
                       uint8_t channel_samples);
#else
void get_accel_readout(lsm6ds3_ints_pin_t ints_pin,
                       int32_array_t* channel_buffer,
                       uint8_t channel_samples);
#endif

#endif
