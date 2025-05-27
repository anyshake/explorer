#include "filter.h"

void filter_fir_apply(filter_fir_t* filter, float* input, uint16_t input_size, float* output) {
    float extended_input[(NUM_TAPS - 1) + input_size];

    memcpy(extended_input, filter->state, (NUM_TAPS - 1) * sizeof(float));
    memcpy(&extended_input[NUM_TAPS - 1], input, input_size * sizeof(float));

    for (uint16_t i = 0; i < input_size; i++) {
        output[i] = 0.0;
        for (uint16_t j = 0; j < NUM_TAPS; j++) {
            output[i] += extended_input[i + j] * filter->coeffs[j];
        }
    }

    memcpy(filter->state, &extended_input[input_size], (NUM_TAPS - 1) * sizeof(float));
}

void apply_data_compensation(int32_t* arr, uint16_t len, filter_fir_t* lowpass_filter, filter_fir_t* bandpass_filter, filter_fir_t* highpass_filter) {
    float input[len];
    for (uint16_t i = 0; i < len; i++) {
        input[i] = (float)arr[i];
    }

    float lp_output[len];
    filter_fir_apply(lowpass_filter, input, len, lp_output);

    float bp_output[len];
    filter_fir_apply(bandpass_filter, input, len, bp_output);

    float hp_output[len];
    filter_fir_apply(highpass_filter, input, len, hp_output);

    for (uint16_t i = 0; i < len; i++) {
        float result = (3.0 * lp_output[i] + 8.0 * bp_output[i] + hp_output[i]) * 2;
        arr[i] = (int32_t)result;
    }
}
