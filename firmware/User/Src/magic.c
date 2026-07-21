#include "User/Inc/magic.h"

float quick_sqrt(float x) {
    float xhalf = 0.5f * x;
    uint32_t i;
    memcpy(&i, &x, sizeof(float));
    i = 0x5f375a86 - (i >> 1);
    memcpy(&x, &i, sizeof(uint32_t));
    x = x * (1.5f - xhalf * x * x);
    return 1.0f / x;
}

float quick_atan2(float y, float x) {
    if (y > 0.0f && x == 0.0f) {
        return MAGIC_PI / 2;
    }
    if (y < 0.0f && x == 0.0f) {
        return -MAGIC_PI / 2;
    }
    if (y == 0.0f && x == 0.0f) {
        return 0;
    }
    float ax = (x < 0.0f) ? -x : x;
    float ay = (y < 0.0f) ? -y : y;
    float temp1 = (ax < ay) ? (ax / ay) : (ay / ax);
    float temp2 = temp1 * temp1;
    float result = ((-0.0464964749f * temp2 + 0.15931422f) * temp2 - 0.327622764f) * temp2 * temp1 + temp1;
    if (ay > ax) {
        result = (MAGIC_PI / 2) - result;
    }
    if (x < 0.0f) {
        result = MAGIC_PI - result;
    }
    if (y < 0.0f) {
        result = (-result);
    }
    return result;
}

void format_float_fixed(char* buffer, size_t buffer_size, float value, uint8_t fractional_digits) {
    if (buffer == NULL || buffer_size == 0) {
        return;
    }

    uint32_t scale = 1;
    for (uint8_t i = 0; i < fractional_digits; i++) {
        scale *= 10;
    }

    bool is_negative = value < 0.0f;
    float magnitude = is_negative ? -value : value;
    uint32_t scaled = (uint32_t)(magnitude * (float)scale + 0.5f);

    if (fractional_digits == 0) {
        snprintf(buffer, buffer_size, "%s%lu", is_negative ? "-" : "", scaled);
        return;
    }

    snprintf(buffer,
             buffer_size,
             "%s%lu.%0*lu",
             is_negative ? "-" : "",
             (scaled / scale),
             fractional_digits,
             (scaled % scale));
}
