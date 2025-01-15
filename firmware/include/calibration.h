#ifndef __CALIBRATION_H
#define __CALIBRATION_H

#include <stdint.h>

#include "ads1262/cmds/sfocal.h"
#include "ads1262/cmds/start.h"
#include "ads1262/cmds/stop.h"
#include "ads1262/regs/inpmux.h"
#include "ads1262/regs/mode_0.h"
#include "settings.h"

void calibrate_adc_offset(ads1262_ctl_pin_t ctl_pin);

#endif
