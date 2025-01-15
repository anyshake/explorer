#include "filter.h"

void apply_compensation_filter(int32_t* arr, uint8_t len, filter_state_t* state) {
    double y_lowpass[len];
    y_lowpass[0] = state->prev_lowpass[0];
    y_lowpass[1] = state->prev_lowpass[1];

    double y_bandpass[len];
    y_bandpass[0] = state->prev_bandpass[0];
    y_bandpass[1] = state->prev_bandpass[1];

    double y_highpass[len];
    y_highpass[0] = state->prev_highpass[0];
    y_highpass[1] = state->prev_highpass[1];

    for (uint8_t i = 2; i < len; i++) {
        double y_next_lowpass = -LOWPASS_A[0] * y_lowpass[i - 1] - LOWPASS_A[1] * y_lowpass[i - 2] +
                                LOWPASS_B[0] * (double)arr[i] + LOWPASS_B[1] * (double)arr[i - 1] + LOWPASS_B[2] * (double)arr[i - 2];
        y_lowpass[i] = y_next_lowpass;

        double y_next_bandpass = -BANDPASS_A[0] * y_bandpass[i - 1] - BANDPASS_A[1] * y_bandpass[i - 2] +
                                 BANDPASS_B[0] * (double)arr[i] + BANDPASS_B[1] * (double)arr[i - 1] + BANDPASS_B[2] * (double)arr[i - 2];
        y_bandpass[i] = y_next_bandpass;

        double y_next_highpass = -HIGHPASS_A[0] * y_highpass[i - 1] - HIGHPASS_A[1] * y_highpass[i - 2] +
                                 HIGHPASS_B[0] * (double)arr[i] + HIGHPASS_B[1] * (double)arr[i - 1] + HIGHPASS_B[2] * (double)arr[i - 2];
        y_highpass[i] = y_next_highpass;
    }

    for (uint8_t i = 0; i < len; i++) {
        arr[i] = (int32_t)(y_lowpass[i] + y_bandpass[i] + y_highpass[i]);
    }

    state->prev_lowpass[0] = y_lowpass[len - 2];
    state->prev_lowpass[1] = y_lowpass[len - 1];

    state->prev_bandpass[0] = y_bandpass[len - 2];
    state->prev_bandpass[1] = y_bandpass[len - 1];

    state->prev_highpass[0] = y_highpass[len - 2];
    state->prev_highpass[1] = y_highpass[len - 1];
}
