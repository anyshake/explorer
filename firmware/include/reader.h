#ifndef __READER_H
#define __READER_H

#include <stdbool.h>
#include <stdint.h>

#include "ads1262/cmds/rdata.h"
#include "ads1262/regs/inpmux.h"
#include "array.h"
#include "settings.h"

void get_adc_readout(ads1262_ctl_pin_t ctl_pin, int32_t arr[]);

#endif
