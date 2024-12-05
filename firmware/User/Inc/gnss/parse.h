#ifndef __GNSS_PARSE_H
#define __GNSS_PARSE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "User/Inc/gnss/utils.h"

void gnss_parse_rmc(gnss_location_t* location,
                    gnss_time_t* time,
                    uint8_t* str_buf);
void gnss_parse_gga(gnss_status_t* status,
                    gnss_location_t* location,
                    uint8_t* str_buf);

#endif
