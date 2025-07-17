#ifndef __GNSS_TIME_H
#define __GNSS_TIME_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/gnss/utils.h"
#include "Utils/Inc/uptime.h"

int64_t gnss_get_timestamp(gnss_time_t* time);

#endif
