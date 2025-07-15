#include "User/Inc/gnss/time.h"

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

int64_t gnss_get_time_obj(gnss_time_t* time) {
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
