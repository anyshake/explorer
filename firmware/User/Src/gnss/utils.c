#include "User/Inc/gnss/utils.h"

void gnss_init(gnss_ctl_pin_t pin) {
    mcu_utils_gpio_mode(pin.rst, MCU_UTILS_GPIO_MODE_OUTPUT);
    mcu_utils_gpio_high(pin.rst);
}

void gnss_reset(gnss_ctl_pin_t pin, bool is_rtos) {
    mcu_utils_gpio_low(pin.rst);
    mcu_utils_delay_ms(100, is_rtos);
    mcu_utils_gpio_high(pin.rst);
    mcu_utils_delay_ms(100, is_rtos);
}

bool gnss_get_sentence(uint8_t* str_buf, uint16_t timeout_ms, bool is_rtos) {
    if (str_buf == NULL) {
        return false;
    }

    uint8_t line_buf[GNSS_SENTENCE_BUFFER_SIZE];
    int16_t line_len = 0;

    for (uint32_t start_time = HAL_GetTick(); HAL_GetTick() - start_time < timeout_ms;) {
        line_len = mcu_utils_uart2_read_line(line_buf, GNSS_SENTENCE_BUFFER_SIZE);
        if (line_len > 0 && (uint16_t)line_len < GNSS_SENTENCE_BUFFER_SIZE) {
            line_buf[line_len] = '\0';
            if (gnss_verify_checksum(line_buf)) {
                memcpy(str_buf, line_buf, (size_t)line_len + 1);
                return true;
            }
        } else if (line_len == -1) {
            mcu_utils_uart2_flush();
            line_len = 0;
        }

        mcu_utils_delay_ms(100, is_rtos);
    }

    return false;
}

uint8_t gnss_padding_sentence(uint8_t* str_buf) {
    if (str_buf == NULL) {
        return 0;
    }

    uint8_t str_len = 0;
    while (str_len < GNSS_SENTENCE_BUFFER_SIZE && str_buf[str_len] != '\0') {
        str_len++;
    }
    if (str_len == GNSS_SENTENCE_BUFFER_SIZE) {
        str_buf[0] = '\0';
        return 0;
    }

    uint8_t new_str[GNSS_SENTENCE_BUFFER_SIZE];
    uint8_t new_len = 0;

    for (uint8_t i = 0; i < str_len; i++) {
        if (new_len + 1 >= GNSS_SENTENCE_BUFFER_SIZE) {
            str_buf[0] = '\0';
            return 0;
        }
        new_str[new_len++] = str_buf[i];
        if (str_buf[i] == ',' && i < str_len - 1 && str_buf[i + 1] == ',') {
            if (new_len + 1 >= GNSS_SENTENCE_BUFFER_SIZE) {
                str_buf[0] = '\0';
                return 0;
            }
            new_str[new_len++] = GNSS_SENTENCE_PADDING_CHAR;
        }
    }

    memcpy(str_buf, new_str, new_len);
    str_buf[new_len] = '\0';

    return new_len;
}

bool gnss_verify_checksum(uint8_t* str_buf) {
    if (str_buf == NULL) {
        return false;
    }

    const char* start = strchr((char*)str_buf, '$');
    if (start == NULL) {
        return false;
    }

    const char* end = strchr(start, '*');

    if (end != NULL && end[1] != '\0' && end[2] != '\0') {
        uint8_t checksum = 0;
        for (const char* ch = start + 1; ch < end; ch++) {
            checksum ^= *ch;
        }

        uint8_t msg_checksum = 0;
        for (uint8_t i = 1; i <= 2; i++) {
            char ch = *(end + i);
            if (isdigit((unsigned char)ch)) {
                msg_checksum = (msg_checksum << 4) + (ch - '0');
            } else if (isxdigit((unsigned char)ch)) {
                msg_checksum = (msg_checksum << 4) + (toupper((unsigned char)ch) - 'A' + 10);
            } else {
                return false;
            }
        }

        return checksum == msg_checksum;
    }

    return false;
}

bool gnss_match_keyword(uint8_t* sentence, const char* keyword) {
    if (!sentence || !keyword) {
        return false;
    }
    if (sentence[0] != '$') {
        return false;
    }
    const char* separator = strchr((char*)sentence, ',');
    size_t keyword_len = strlen(keyword);
    if (separator == NULL || (size_t)(separator - (char*)sentence) < keyword_len) {
        return false;
    }

    return memcmp(separator - keyword_len, keyword, keyword_len) == 0;
}

static bool is_leap_year(uint16_t year) {
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            return year % 400 == 0;
        }
        return true;
    }

    return false;
};

static uint32_t days_before_year(uint16_t year) {
    uint32_t days = 0;
    for (uint16_t y = 1970; y < year; y++) {
        days += is_leap_year(y) ? 366 : 365;
    }

    return days;
};

static bool is_valid_datetime(const gnss_time_t* time) {
    if (time->month == 0 || time->month > 12 ||
        time->hour > 23 || time->minute > 59 || time->second > 59 || time->milisecond > 999) {
        return false;
    }

    static const uint8_t days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint8_t max_day = days_in_month[time->month - 1];
    if (time->month == 2 && is_leap_year((uint16_t)time->year + 2000U)) {
        max_day++;
    }

    return time->mday > 0 && time->mday <= max_day;
}

int64_t gnss_get_timestamp(gnss_time_t* time) {
    if (time == NULL || !time->is_valid || !is_valid_datetime(time)) {
        return 0;
    }

    const uint8_t days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    uint16_t year = time->year + 2000;
    uint16_t month = time->month;
    uint16_t day = time->mday;
    uint16_t hour = time->hour;
    uint16_t minute = time->minute;
    uint16_t second = time->second;
    uint16_t milisecond = time->milisecond;
    uint32_t total_days = days_before_year(year);

    for (uint16_t m = 0; m < month - 1; m++) {
        total_days += days_in_month[m];
        if (m == 1 && is_leap_year(year)) {
            total_days += 1;
        }
    }

    total_days += day - 1;

    int64_t total_miliseconds = (int64_t)total_days * 24 * 3600 * 1000;
    total_miliseconds += (int64_t)hour * 3600 * 1000;
    total_miliseconds += (int64_t)minute * 60 * 1000;
    total_miliseconds += (int64_t)second * 1000;
    total_miliseconds += milisecond;

    return total_miliseconds;
}
