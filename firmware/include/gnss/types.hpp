#ifndef __GNSS_TYPES__
#define __GNSS_TYPES__

#include <stdint.h>

#define GNSS_SENTENCE_BUFFER_SIZE 100
#define GNSS_SENTENCE_PADDING_CHAR '_'
#define GNSS_SENTENCE_TYPE_RMC "RMC"
#define GNSS_SENTENCE_TYPE_GGA "GGA"
#define GNSS_SENTENCE_TYPE_GSA "GSA"
#define GNSS_SENTENCE_TYPE_GSV "GSV"
#define GNSS_SENTENCE_TYPE_VTG "VTG"
#define GNSS_SENTENCE_TYPE_ZDA "ZDA"
#define GNSS_SENTENCE_TYPE_GLL "GLL"
#define GNSS_SENTENCE_TYPE_TXT "TXT"
#define GNSS_SENTENCE_READ_ATTEMPTS 50000U

typedef struct {
    double latitude = 0;
    double longitude = 0;
    double altitude = 0;
    uint8_t is_valid = 0;
} gnss_location_t;

typedef struct {
    uint8_t year = 0;
    uint8_t month = 0;
    uint8_t mday = 0;
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t second = 0;
    uint16_t milisecond = 0;
    uint8_t is_valid = 0;
} gnss_time_t;

#endif
