#ifndef __READER_H
#define __READER_H

#include <stdbool.h>
#include <stdint.h>

#include "ads1262/cmds/rdata.h"
#include "ads1262/regs/inpmux.h"
#include "array.h"
#include "filter.h"
#include "settings.h"

void get_adc_readout(ads1262_ctl_pin_t ctl_pin,
                     int32_t adc_readout_z_axis[],
                     int32_t adc_readout_e_axis[],
                     int32_t adc_readout_n_axis[],
                     filter_state_t* filter_state_z_axis,
                     filter_state_t* filter_state_e_axis,
                     filter_state_t* filter_state_n_axis,
                     uint8_t channel_samples);

#endif
