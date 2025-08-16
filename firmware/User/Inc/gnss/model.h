#ifndef __GNSS_MODEL_H
#define __GNSS_MODEL_H

#include <stdbool.h>
#include <stdint.h>

#include "Utils/Inc/delay.h"
#include "Utils/Inc/uart2.h"

#define GENERIC 0

#define QUECTEL_LC260Z 1
#define QUECTEL_LC261Z 2
#define QUECTEL_LC760Z 3
#define QUECTEL_LC761Z 4

#define ZHONGKEWEI_ATGM332D 5
#define ZHONGKEWEI_ATGM336H 6
#define QUECTEL_L26K 7
#define QUECTEL_L76K 8

#define ALLYSTAR_TAU812S 9
#define ALLYSTAR_TAU1113 10
#define ALLYSTAR_TAU1114 11

void gnss_model_setup(bool is_rtos);
void gnss_model_factory_reset(bool is_rtos);
int64_t gnss_model_handle_timestamp(int64_t timestamp);

#endif
