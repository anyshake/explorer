#include "User/Inc/gnss/parse.h"

void gnss_parse_rmc(gnss_location_t* location,
                    gnss_time_t* time,
                    uint8_t* str_buf) {
    uint8_t str_len = strlen((char*)str_buf);
    char buffer[str_len];
    strncpy(buffer, (char*)str_buf, str_len);

    char* token = strtok(buffer, ",");
    for (uint8_t i = 0; token != NULL; i++) {
        switch (i) {
            case 1:
                if (time != NULL) {
                    if (*token != GNSS_SENTENCE_PADDING_CHAR) {
                        double result = atof(token);
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
                        double result = atof(token);
                        uint8_t deg = result / 100;
                        double min = result - deg * 100;
                        double lat_deg = deg + min / 60.0;
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
                        double result = atof(token);
                        uint8_t deg = result / 100;
                        double min = result - deg * 100;
                        double lng_deg = deg + min / 60.0;
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
                        int32_t result = atoi(token);
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

void gnss_parse_gga(gnss_location_t* location, uint8_t* str_buf) {
    uint8_t str_len = strlen((char*)str_buf);
    char buffer[str_len];
    strncpy(buffer, (char*)str_buf, str_len);

    char* token = strtok(buffer, ",");
    for (uint8_t i = 0; token != NULL; i++) {
        switch (i) {
            case 2:
                if (location != NULL) {
                    if (*token != GNSS_SENTENCE_PADDING_CHAR) {
                        double result = atof(token);
                        uint8_t deg = result / 100;
                        double min = result - deg * 100;
                        double lat_deg = deg + min / 60.0;
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
                        double result = atof(token);
                        uint8_t deg = result / 100;
                        double min = result - deg * 100;
                        double lng_deg = deg + min / 60.0;
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
            case 9:
                if (location != NULL) {
                    if (*token != GNSS_SENTENCE_PADDING_CHAR) {
                        location->elevation = atof(token);
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
