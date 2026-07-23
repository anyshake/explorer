#ifndef __MODE_H
#define __MODE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "User/Inc/ads1262/cmds/rdata.h"
#include "User/Inc/ads1262/cmds/start.h"
#include "User/Inc/ads1262/cmds/stop.h"
#include "User/Inc/ads1262/regs/inpmux.h"
#include "User/Inc/ads1262/regs/interface.h"
#include "User/Inc/ads1262/regs/mode_0.h"
#include "User/Inc/ads1262/regs/mode_2.h"
#include "User/Inc/ads1262/regs/ofcal.h"
#include "User/Inc/eeprom/read.h"
#include "User/Inc/eeprom/write.h"
#include "User/Inc/settings.h"

#include "User/Inc/ssd1306/display.h"
#include "User/Inc/ssd1306/font.h"
#include "User/Inc/ssd1306/utils.h"

#include "User/Inc/magic.h"
#include "User/Inc/reader.h"
#include "User/Inc/types.h"

#include "Utils/Inc/delay.h"
#include "Utils/Inc/led.h"
#include "Utils/Inc/uart/uart1.h"
#include "Utils/Inc/uart/uart2.h"

#define EEPROM_OFFSET_CALIB_STATUS 4
#define EEPROM_OFFSET_OFCAL_CHANNEL_1 5
#define EEPROM_OFFSET_OFCAL_CHANNEL_2 11
#define EEPROM_OFFSET_OFCAL_CHANNEL_3 17

typedef struct {
    uint8_t status;
    uint8_t channel_1[3];
    uint8_t channel_2[3];
    uint8_t channel_3[3];
} adc_calibration_data_t;

void mode_entry_gnss_debug(explorer_global_states_t* states);
void mode_entry_leveling(explorer_global_states_t* states);
void mode_entry_adc_calibration(void);

#endif
