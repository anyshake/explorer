#include "filter.h"

void apply_compensation_filter(int32_t* arr, size_t len) {
    double yLow[len];
    double yBand[len];
    double yHigh[len];

    for (size_t i = 0; i < len; i++) {
        yLow[i] = 0.0;
        yBand[i] = 0.0;
        yHigh[i] = 0.0;
    }

    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < sizeof(LOWPASS_A) / sizeof(LOWPASS_A[0]); j++) {
            if (i - j >= 0) {
                yLow[i] += LOWPASS_A[j] * (double)arr[i - j];
            }
        }
        yLow[i] *= 1000.0;
    }

    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < sizeof(BANDPASS_A) / sizeof(BANDPASS_A[0]); j++) {
            if (i - j >= 0) {
                yBand[i] += BANDPASS_A[j] * (double)arr[i - j];
            }
        }
        yBand[i] *= 0.01;
    }

    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < sizeof(HIGHPASS_A) / sizeof(HIGHPASS_A[0]); j++) {
            if (i - j >= 0) {
                yHigh[i] += HIGHPASS_A[j] * (double)arr[i - j];
            }
        }
        yHigh[i] *= 0.001;
    }

    for (size_t i = 0; i < len; i++) {
        double val = yLow[i] + yBand[i] + yHigh[i];
        if (val > INT32_MAX) {
            val = INT32_MAX;
        } else if (val < INT32_MIN) {
            val = INT32_MIN;
        }
        arr[i] = (int32_t)val;
    }
}
