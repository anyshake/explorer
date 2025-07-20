#ifndef __GNSS_PARSE_H
#define __GNSS_PARSE_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "User/Inc/gnss/utils.h"

void gnss_parse_rmc(uint8_t* str_buf, gnss_location_t* location, gnss_time_t* time);
void gnss_parse_gga(uint8_t* str_buf, gnss_status_t* status, gnss_location_t* location);

#endif
