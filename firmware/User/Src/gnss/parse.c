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

static inline int32_t quick_atoi(char* str) {
    if (str == NULL) {
        return 0;
    }

    int32_t data = 0;
    int32_t sign = 1;

    while (*str == ' ') {
        str++;
    }

    if (*str == '-' || *str == '+') {
        sign = 1 - 2 * (*str++ == '-');
    }

    while (isdigit(*str)) {
        if (data > INT32_MAX / 10 || (data == INT32_MAX / 10 && (str[0] > '7'))) {
            if (sign == -1) {
                return INT32_MIN;
            } else {
                return INT32_MAX;
            }
        }

        data = data * 10 + (*str++ - '0');
    }

    return data * sign;
}

void gnss_parse_rmc(uint8_t* str_buf, gnss_location_t* location, gnss_time_t* time) {
    char* token = strtok((char*)str_buf, ",");

    for (uint8_t i = 0; token != NULL; i++) {
        switch (i) {
            case 1:
                if (time != NULL) {
                    if (*token != GNSS_SENTENCE_PADDING_CHAR) {
                        float result = quick_atof(token);
                        int32_t result_int = (int32_t)result;
                        time->hour = result_int / 10000;
                        time->minute = (result_int / 100) % 100;
                        time->second = result_int % 100;
                        time->milisecond = (result - result_int) * 1000;
                        time->is_valid = true;
                    } else {
                        time->is_valid = false;
                    }
                }
                break;
            case 2:
                if (location != NULL) {
                    if (*token == 'A') {
                        location->is_valid = true;
                    } else {
                        location->is_valid = false;
                    }
                }
                break;
            case 3:
                if (location != NULL) {
                    if (*token != GNSS_SENTENCE_PADDING_CHAR) {
                        float result = quick_atof(token);
                        uint8_t deg = result / 100;
                        float min = result - deg * 100;
                        float lat_deg = deg + min / 60.0;
                        token = strtok(NULL, ",");
                        i++;
                        if (*token == 'S') {
                            lat_deg = -lat_deg;
                        }
                        location->latitude = lat_deg;
                    }
                }
                break;
            case 5:
                if (location != NULL) {
                    if (*token != GNSS_SENTENCE_PADDING_CHAR) {
                        float result = quick_atof(token);
                        uint8_t deg = result / 100;
                        float min = result - deg * 100;
                        float lng_deg = deg + min / 60.0;
                        token = strtok(NULL, ",");
                        i++;
                        if (*token == 'W') {
                            lng_deg = -lng_deg;
                        }
                        location->longitude = lng_deg;
                    }
                }
                break;
            case 9:
                if (time != NULL) {
                    if (*token != GNSS_SENTENCE_PADDING_CHAR) {
                        int32_t result = quick_atoi(token);
                        time->mday = result / 10000;
                        time->month = (result / 100) % 100;
                        time->year = result % 100;
                        time->is_valid = time->year >= 0 && time->year <= 99;
                    } else {
                        time->is_valid = false;
                    }
                }
                break;
        }

        token = strtok(NULL, ",");
    }
}

void gnss_parse_gga(uint8_t* str_buf, gnss_status_t* status, gnss_location_t* location, gnss_time_t* time) {
    char* token = strtok((char*)str_buf, ",");

    for (uint8_t i = 0; token != NULL; i++) {
        switch (i) {
            case 1:
                if (time != NULL) {
                    if (*token != GNSS_SENTENCE_PADDING_CHAR) {
                        float result = quick_atof(token);
                        int32_t result_int = (int32_t)result;
                        time->hour = result_int / 10000;
                        time->minute = (result_int / 100) % 100;
                        time->second = result_int % 100;
                        time->milisecond = (result - result_int) * 1000;
                        time->is_valid = true;
                    } else {
                        time->is_valid = false;
                    }
                }
                break;
            case 2:
                if (location != NULL) {
                    if (*token != GNSS_SENTENCE_PADDING_CHAR) {
                        float result = quick_atof(token);
                        uint8_t deg = result / 100;
                        float min = result - deg * 100;
                        float lat_deg = deg + min / 60.0;
                        token = strtok(NULL, ",");
                        i++;
                        if (*token == 'S') {
                            lat_deg = -lat_deg;
                        }
                        location->latitude = lat_deg;
                        location->is_valid = true;
                    } else {
                        location->is_valid = false;
                    }
                }
                break;
            case 4:
                if (location != NULL) {
                    if (*token != GNSS_SENTENCE_PADDING_CHAR) {
                        float result = quick_atof(token);
                        uint8_t deg = result / 100;
                        float min = result - deg * 100;
                        float lng_deg = deg + min / 60.0;
                        token = strtok(NULL, ",");
                        i++;
                        if (*token == 'W') {
                            lng_deg = -lng_deg;
                        }
                        location->longitude = lng_deg;
                        location->is_valid = true;
                    } else {
                        location->is_valid = false;
                    }
                }
                break;
            case 7:
                if (status != NULL) {
                    if (*token != GNSS_SENTENCE_PADDING_CHAR) {
                        status->satellites = (uint8_t)quick_atoi(token);
                    }
                }
            case 8:
                if (status != NULL) {
                    if (*token != GNSS_SENTENCE_PADDING_CHAR) {
                        status->hdop = quick_atof(token);
                    } else {
                        status->hdop = 0.0;
                    }
                }
            case 9:
                if (location != NULL) {
                    if (*token != GNSS_SENTENCE_PADDING_CHAR) {
                        location->elevation = quick_atof(token);
                        location->is_valid = true;
                    } else {
                        location->is_valid = false;
                    }
                }
                break;
        }

        token = strtok(NULL, ",");
    }
}
