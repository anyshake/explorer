#ifndef __UTILS_H
#define __UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "stm32f103xb.h"
#include "stm32f1xx_hal_rcc.h"

#include "User/Inc/gnss/model.h"
#include "User/Inc/gnss/parse.h"
#include "User/Inc/gnss/time.h"
#include "User/Inc/gnss/utils.h"
#include "User/Inc/settings.h"
#include "User/Inc/ssd1306/display.h"
#include "User/Inc/ssd1306/font.h"
#include "User/Inc/ssd1306/utils.h"
#include "User/Inc/types.h"

#include "Utils/Inc/gpio.h"

uint32_t get_tim3_clk_freq(void);
void display_device_settings(explorer_global_states_t* states);
void read_device_settings(explorer_global_states_t* states);
bool parse_gnss_message(uint8_t* message_buf, gnss_status_t* gnss_status, gnss_location_t* gnss_location, gnss_time_t* gnss_time, int64_t local_timestamp, int64_t* gnss_time_diff);

#endif
