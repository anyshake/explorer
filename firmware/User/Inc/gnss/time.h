#ifndef __GNSS_TIME_H
#define __GNSS_TIME_H

#include <stdbool.h>
#include <stdint.h>

#include "Utils/Inc/uptime.h"
#include "User/Inc/gnss/utils.h"

int64_t gnss_get_timestamp(gnss_time_t* time);
int64_t gnss_get_current_timestamp(int64_t base_time, int64_t ref_time);

#endif
