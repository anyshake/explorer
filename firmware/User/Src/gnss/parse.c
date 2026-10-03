#include "User/Inc/gnss/parse.h"

static inline float quick_atof(const char* str) {
    if (str == NULL) {
        return 0.0f;
    }

    float result = 0.0f;
    float fraction = 0.1f;
    bool negative = false;
    bool after_dot = false;

    while (*str == ' ') {
        str++;
    }

    if (*str == '-') {
        negative = true;
        str++;
    } else if (*str == '+') {
        str++;
    }

    while ((*str >= '0' && *str <= '9') || *str == '.') {
        if (*str == '.') {
            after_dot = true;
        } else {
            uint8_t digit = *str - '0';
            if (!after_dot) {
                result = result * 10.0f + digit;
            } else {
                result += digit * fraction;
                fraction *= 0.1f;
            }
        }
        str++;
    }

    return negative ? -result : result;
}

static bool is_leap_year(uint16_t year) {
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            return year % 400 == 0;
        }
        return true;
    }

    return false;
}

static bool parse_time(const char* token, gnss_time_t* time) {
    size_t len = strlen(token);
    if (len < 6) {
        return false;
    }

    for (size_t i = 0; i < 6; i++) {
        if (!isdigit((unsigned char)token[i])) {
            return false;
        }
    }

    if (len > 6) {
        if (token[6] != '.' || len == 7) {
            return false;
        }
        for (size_t i = 7; i < len; i++) {
            if (!isdigit((unsigned char)token[i])) {
                return false;
            }
        }
    }

    uint8_t hour = (uint8_t)((token[0] - '0') * 10 + token[1] - '0');
    uint8_t minute = (uint8_t)((token[2] - '0') * 10 + token[3] - '0');
    uint8_t second = (uint8_t)((token[4] - '0') * 10 + token[5] - '0');
    if (hour > 23 || minute > 59 || second > 59) {
        return false;
    }

    uint16_t millisecond = 0;
    size_t fractional_digits = len > 7 ? len - 7 : 0;
    size_t used_digits = fractional_digits < 3 ? fractional_digits : 3;
    for (size_t i = 0; i < used_digits; i++) {
        millisecond = (uint16_t)(millisecond * 10 + token[7 + i] - '0');
    }
    for (size_t i = used_digits; i < 3; i++) {
        millisecond *= 10;
    }

    time->hour = hour;
    time->minute = minute;
    time->second = second;
    time->milisecond = millisecond;
    return true;
}

static bool parse_date(const char* token, gnss_time_t* time) {
    if (strlen(token) != 6) {
        return false;
    }
    for (size_t i = 0; i < 6; i++) {
        if (!isdigit((unsigned char)token[i])) {
            return false;
        }
    }

    uint8_t day = (uint8_t)((token[0] - '0') * 10 + token[1] - '0');
    uint8_t month = (uint8_t)((token[2] - '0') * 10 + token[3] - '0');
    uint8_t year = (uint8_t)((token[4] - '0') * 10 + token[5] - '0');
    if (month == 0 || month > 12) {
        return false;
    }

    static const uint8_t days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint8_t max_day = days_in_month[month - 1];
    if (month == 2 && is_leap_year((uint16_t)year + 2000U)) {
        max_day++;
    }
    if (day == 0 || day > max_day) {
        return false;
    }

    time->mday = day;
    time->month = month;
    time->year = year;
    return true;
}

static bool parse_uint8(const char* token, uint8_t* value) {
    if (token == NULL || *token == '\0') {
        return false;
    }

    uint16_t result = 0;
    for (const char* ch = token; *ch != '\0'; ch++) {
        if (!isdigit((unsigned char)*ch)) {
            return false;
        }
        result = (uint16_t)(result * 10 + (*ch - '0'));
        if (result > UINT8_MAX) {
            return false;
        }
    }

    *value = (uint8_t)result;
    return true;
}

static bool parse_decimal(const char* token, float* value) {
    if (token == NULL || *token == '\0') {
        return false;
    }

    const char* ch = token;
    if (*ch == '-' || *ch == '+') {
        ch++;
    }

    bool has_digit = false;
    bool has_dot = false;
    for (; *ch != '\0'; ch++) {
        if (isdigit((unsigned char)*ch)) {
            has_digit = true;
        } else if (*ch == '.' && !has_dot) {
            has_dot = true;
        } else {
            return false;
        }
    }

    if (!has_digit) {
        return false;
    }

    *value = quick_atof(token);
    return true;
}

static bool parse_coordinate(const char* value_token,
                             const char* direction_token,
                             bool latitude,
                             float* coordinate) {
    float raw_value;
    if (!parse_decimal(value_token, &raw_value) || raw_value < 0.0f ||
        direction_token == NULL || direction_token[1] != '\0') {
        return false;
    }

    char direction = direction_token[0];
    if ((latitude && direction != 'N' && direction != 'S') ||
        (!latitude && direction != 'E' && direction != 'W')) {
        return false;
    }

    float max_value = latitude ? 9000.0f : 18000.0f;
    if (raw_value > max_value) {
        return false;
    }

    uint16_t degrees = (uint16_t)(raw_value / 100.0f);
    float minutes = raw_value - (float)degrees * 100.0f;
    uint16_t max_degrees = latitude ? 90 : 180;
    if (minutes >= 60.0f || degrees > max_degrees ||
        (degrees == max_degrees && minutes > 0.0f)) {
        return false;
    }

    float result = (float)degrees + minutes / 60.0f;
    if (direction == 'S' || direction == 'W') {
        result = -result;
    }
    *coordinate = result;
    return true;
}

static uint8_t split_sentence(uint8_t* str_buf, char* fields[], uint8_t max_fields) {
    uint8_t field_count = 0;
    char* token = strtok((char*)str_buf, ",");
    while (token != NULL) {
        if (field_count >= max_fields) {
            return 0;
        }
        fields[field_count++] = token;
        token = strtok(NULL, ",");
    }
    return field_count;
}

void gnss_parse_rmc(uint8_t* str_buf, gnss_location_t* location, gnss_time_t* time) {
    if (time != NULL) {
        time->is_valid = false;
    }
    if (location != NULL) {
        location->is_valid = false;
    }
    if (str_buf == NULL || !gnss_match_keyword(str_buf, GNSS_SENTENCE_TYPE_RMC)) {
        return;
    }

    char* fields[GNSS_MAX_FIELDS] = {0};
    uint8_t field_count = split_sentence(str_buf, fields, GNSS_MAX_FIELDS);
    if (field_count <= 9) {
        return;
    }

    if (time != NULL) {
        bool has_valid_time = parse_time(fields[1], time);
        bool has_valid_date = parse_date(fields[9], time);
        time->is_valid = has_valid_time && has_valid_date;
    }

    if (location != NULL) {
        bool has_valid_status = fields[2][0] == 'A' && fields[2][1] == '\0';
        bool has_valid_latitude = parse_coordinate(fields[3], fields[4], true, &location->latitude);
        bool has_valid_longitude = parse_coordinate(fields[5], fields[6], false, &location->longitude);
        location->is_valid = has_valid_status && has_valid_latitude && has_valid_longitude;
    }
}

void gnss_parse_gga(uint8_t* str_buf, gnss_status_t* status, gnss_location_t* location, gnss_time_t* time) {
    if (time != NULL) {
        time->is_valid = false;
    }
    if (status != NULL) {
        status->satellites = 0;
        status->hdop = GNSS_INVALID_HDOP;
    }
    if (location != NULL) {
        location->is_valid = false;
    }
    if (str_buf == NULL || !gnss_match_keyword(str_buf, GNSS_SENTENCE_TYPE_GGA)) {
        return;
    }

    char* fields[GNSS_MAX_FIELDS] = {0};
    uint8_t field_count = split_sentence(str_buf, fields, GNSS_MAX_FIELDS);
    if (field_count <= 9) {
        return;
    }

    if (time != NULL) {
        time->is_valid = parse_time(fields[1], time);
    }

    uint8_t fix_quality = 0;
    bool has_valid_fix = parse_uint8(fields[6], &fix_quality) && fix_quality > 0;

    uint8_t satellites = 0;
    bool has_valid_satellites = parse_uint8(fields[7], &satellites);
    float hdop = GNSS_INVALID_HDOP;
    bool has_valid_hdop = parse_decimal(fields[8], &hdop) && hdop >= 0.0f;
    if (status != NULL) {
        status->satellites = has_valid_satellites ? satellites : 0;
        status->hdop = has_valid_hdop ? hdop : GNSS_INVALID_HDOP;
    }

    if (location != NULL) {
        bool has_valid_latitude = parse_coordinate(fields[2], fields[3], true, &location->latitude);
        bool has_valid_longitude = parse_coordinate(fields[4], fields[5], false, &location->longitude);
        bool has_valid_elevation = parse_decimal(fields[9], &location->elevation);
        location->is_valid = has_valid_fix && has_valid_hdop &&
                             has_valid_latitude && has_valid_longitude && has_valid_elevation;
    }
}
