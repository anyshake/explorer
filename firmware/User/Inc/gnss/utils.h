#ifndef __GNSS_UTILS_H
#define __GNSS_UTILS_H

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "User/Inc/gnss/model.h"
#include "Utils/Inc/delay.h"
#include "Utils/Inc/gpio.h"
#include "Utils/Inc/uart2.h"
#include "Utils/Inc/uptime.h"

#define GNSS_SENTENCE_BUFFER_SIZE 96
#define GNSS_SENTENCE_PADDING_CHAR '_'
#define GNSS_SENTENCE_TYPE_RMC "RMC"
#define GNSS_SENTENCE_TYPE_GGA "GGA"
#define GNSS_SENTENCE_TYPE_GSA "GSA"
#define GNSS_SENTENCE_TYPE_GSV "GSV"
#define GNSS_SENTENCE_TYPE_VTG "VTG"
#define GNSS_SENTENCE_TYPE_ZDA "ZDA"
#define GNSS_SENTENCE_TYPE_GLL "GLL"
#define GNSS_SENTENCE_TYPE_TXT "TXT"

typedef struct {
    mcu_utils_gpio_t pps;
    mcu_utils_gpio_t rst;
} gnss_ctl_pin_t;

typedef struct {
    float hdop;
    uint8_t satellites;
} gnss_status_t;

typedef struct {
    float latitude;
    float longitude;
    float elevation;
    bool is_valid;
} gnss_location_t;

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t mday;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t milisecond;
    bool is_valid;
} gnss_time_t;

void gnss_init(gnss_ctl_pin_t pin);
void gnss_reset(gnss_ctl_pin_t pin, bool is_rtos);
uint8_t gnss_get_sentence(uint8_t* str_buf, uint16_t timeout_ms);
uint8_t gnss_padding_sentence(uint8_t* str_buf);
bool gnss_verify_checksum(uint8_t* str_buf);
bool gnss_match_keyword(uint8_t* sentence, const char* keyword);

#endif
