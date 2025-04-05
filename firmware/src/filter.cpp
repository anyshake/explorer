#include "filter.h"

void filter_fir_lowpass_get_coeffs(float cutoff_freq, float sample_rate, uint16_t num_taps, float* coeffs) {
    float normalized_cutoff = 2 * M_PI * cutoff_freq / sample_rate;
    float middle = (float)(num_taps - 1) / 2.0;
    for (uint16_t i = 0; i < num_taps; i++) {
        float n = (float)(i)-middle;
        if (n == 0) {
            coeffs[i] = normalized_cutoff / M_PI;
        } else {
            coeffs[i] = sin(normalized_cutoff * n) / (M_PI * n);
        }
    }

    for (uint16_t i = 0; i < num_taps; i++) {
        coeffs[i] *= 0.54 - 0.46 * cos(2 * M_PI * (float)(i) / (float)(num_taps - 1));
    }
}

void filter_fir_highpass_get_coeffs(float cutoff_freq, float sample_rate, uint16_t num_taps, float* coeffs) {
    float low_pass_coeffs[num_taps];
    filter_fir_lowpass_get_coeffs(cutoff_freq, sample_rate, num_taps, low_pass_coeffs);

    for (uint16_t i = 0; i < num_taps; i++) {
        if (i == num_taps / 2) {
            coeffs[i] = 1 - low_pass_coeffs[i];
        } else {
            coeffs[i] = -low_pass_coeffs[i];
        }
    }
}

void filter_fir_bandpass_get_coeffs(float low_cutoff_freq, float high_cutoff_freq, float sample_rate, uint16_t num_taps, float* coeffs) {
    float low_pass_coeffs[num_taps];
    float high_pass_coeffs[num_taps];

    filter_fir_lowpass_get_coeffs(high_cutoff_freq, sample_rate, num_taps, low_pass_coeffs);
    filter_fir_highpass_get_coeffs(low_cutoff_freq, sample_rate, num_taps, high_pass_coeffs);

    for (uint16_t i = 0; i < num_taps; i++) {
        coeffs[i] = low_pass_coeffs[i] + high_pass_coeffs[i];
    }
}

void filter_fir_apply(filter_fir_t* filter, float* input, uint16_t input_size, float* output) {
    float extended_input[(FILTER_NUM_TAPS - 1) + input_size];

    memcpy(extended_input, filter->state, (FILTER_NUM_TAPS - 1) * sizeof(float));
    memcpy(&extended_input[FILTER_NUM_TAPS - 1], input, input_size * sizeof(float));

    for (uint16_t i = 0; i < input_size; i++) {
        output[i] = 0.0;
        for (uint16_t j = 0; j < FILTER_NUM_TAPS; j++) {
            output[i] += extended_input[i + j] * filter->coeffs[j];
        }
    }

    memcpy(filter->state, &extended_input[input_size], (FILTER_NUM_TAPS - 1) * sizeof(float));
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
        arr[i] = (int32_t)(81.0000000000000 * lp_output[i] + 6.36492220650636 * bp_output[i] + 0.999953023310421 * hp_output[i]);
    }
}
