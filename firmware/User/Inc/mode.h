#ifndef __MODE_H
#define __MODE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "User/Inc/magic.h"
#include "User/Inc/reader.h"
#include "User/Inc/ssd1306/display.h"
#include "User/Inc/ssd1306/font.h"
#include "User/Inc/ssd1306/utils.h"
#include "User/Inc/types.h"

#include "Utils/Inc/delay.h"
#include "Utils/Inc/uart.h"
#include "Utils/Inc/uart2.h"

void mode_entry_gnss_debug(explorer_global_states_t* states);
void mode_entry_leveling(explorer_global_states_t* states);

#endif
