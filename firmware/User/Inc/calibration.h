#ifndef __CALIBRATION_H
#define __CALIBRATION_H

#include <stdint.h>

#include "User/Inc/ads1262/cmds/sfocal.h"
#include "User/Inc/ads1262/cmds/start.h"
#include "User/Inc/ads1262/cmds/stop.h"
#include "User/Inc/ads1262/regs/inpmux.h"
#include "User/Inc/ads1262/regs/mode_0.h"
#include "User/Inc/settings.h"
#include "Utils/Inc/gpio.h"

void calibrate_adc_offset(ads1262_ctl_pin_t ctl_pin);

#endif
