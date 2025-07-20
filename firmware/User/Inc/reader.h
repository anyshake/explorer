#ifndef __READER_H
#define __READER_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/ads1262/cmds/rdata.h"
#include "User/Inc/ads1262/regs/inpmux.h"
#include "User/Inc/ads1262/regs/ofcal.h"
#include "User/Inc/ads1262/utils.h"
#include "User/Inc/array.h"
#include "User/Inc/icm42688/regs/accel_data_x.h"
#include "User/Inc/icm42688/regs/accel_data_y.h"
#include "User/Inc/icm42688/regs/accel_data_z.h"
#include "User/Inc/icm42688/regs/gyro_data_x.h"
#include "User/Inc/icm42688/regs/gyro_data_y.h"
#include "User/Inc/icm42688/regs/gyro_data_z.h"
#include "User/Inc/icm42688/regs/temp_data.h"
#include "User/Inc/icm42688/utils.h"
#include "User/Inc/lsm6ds3/regs/out_temp.h"
#include "User/Inc/lsm6ds3/regs/outx_xl.h"
#include "User/Inc/lsm6ds3/regs/outy_xl.h"
#include "User/Inc/lsm6ds3/regs/outz_xl.h"
#include "User/Inc/lsm6ds3/utils.h"
#include "User/Inc/types.h"

explorer_adc_calibration_offset_t adc_calibration_offset_new(void);
void get_adc_readout(ads1262_ctl_pin_t ctl_pin, explorer_adc_calibration_offset_t offset_cal, int32_t arr[]);
void get_accel_readout(uint16_t lsb_per_g, int16_t arr[]);
void get_env_temperature(float* temp);

#endif
