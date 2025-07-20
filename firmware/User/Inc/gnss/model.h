#ifndef __GNSS_MODEL_H
#define __GNSS_MODEL_H

#include <stdbool.h>
#include <stdint.h>

#include "Utils/Inc/delay.h"
#include "Utils/Inc/uart2.h"

#define GENERIC 0
#define QUECTEL_LC260Z 1
#define ZHONGKEWEI_ATGM332D 2

#if GNSS_MODEL == GENERIC

#define GNSS_ROUND_TIMESTAMP false
#define GNSS_REQUIRED_HDOP 2.0f

#elif GNSS_MODEL == QUECTEL_LC260Z

#define GNSS_ROUND_TIMESTAMP true
#define GNSS_REQUIRED_HDOP 2.0f

#elif GNSS_MODEL == ZHONGKEWEI_ATGM332D

#define GNSS_ROUND_TIMESTAMP false
#define GNSS_REQUIRED_HDOP 2.0f

#endif

void gnss_model_reset(bool is_rtos);

#endif
