#ifndef __GNSS_MODEL_H
#define __GNSS_MODEL_H

#include <stdbool.h>

#define GENERIC 0
#define QUECTEL_LC260Z 1
#define ZHONGKEWEI_ATGM332D 2

#if GNSS_MODEL == GENERIC

#define GNSS_ROUND_TIMESTAMP false
#define GNSS_INIT_COMMAND false
#define GNSS_INIT_DELAY_MS 0

#elif GNSS_MODEL == QUECTEL_LC260Z

#define GNSS_ROUND_TIMESTAMP true
#define GNSS_INIT_COMMAND true
#define GNSS_INIT_DELAY_MS 1000

// Set NMEA message update interval to 0.2s
// reference: https://www.quectel.com.cn/download/quectel_lc26xz00lc76xz00_gnss_%E5%8D%8F%E8%AE%AE%E8%A7%84%E8%8C%83_v1-3
static const char gnss_init_cmd[] = {0xF1, 0xD9, 0x06, 0x42, 0x14, 0x00, 0x00, 0x01, 0x35, 0x32, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8C, 0xD5};

#elif GNSS_MODEL == ZHONGKEWEI_ATGM332D

#define GNSS_ROUND_TIMESTAMP false
#define GNSS_INIT_COMMAND true
#define GNSS_INIT_DELAY_MS 5000

// Warm start command for AT6558R based GNSS
// reference: https://wiki.millerjs.org/atgm336h
static const char gnss_init_cmd[] = "$PCAS10,0*1C\r\n";

#endif

#endif
