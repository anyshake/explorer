#ifndef __GNSS_PARSE__
#define __GNSS_PARSE__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "gnss/types.hpp"

void gnss_parse_rmc(gnss_location_t* location,
                    gnss_time_t* time,
                    uint8_t* str_buf);
void gnss_parse_gga(gnss_location_t* location,
                    gnss_time_t* time,
                    uint8_t* str_buf);

#endif
