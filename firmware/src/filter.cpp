#include "filter.h"

#include <math.h>

void hamming_window(uint16_t n, double* window) {
    for (uint16_t i = 0; i < n; i++) {
        window[i] = 0.54 - 0.46 * cos(2 * M_PI * (double)i / (double)(n - 1));
    }
}

void generate_lowpass_coeffs(double cutoff_freq, double sample_rate, uint16_t num_taps, double* coeffs) {
    double normalized_cutoff = 2 * M_PI * cutoff_freq / sample_rate;
    double window[num_taps];

    for (uint16_t i = 0; i < num_taps; i++) {
        if (i == num_taps / 2) {
            coeffs[i] = normalized_cutoff / M_PI;
        } else {
            coeffs[i] = sin(normalized_cutoff * (i - num_taps / 2)) / (M_PI * (i - num_taps / 2));
        }
    }

    hamming_window(num_taps, window);

    for (uint16_t i = 0; i < num_taps; i++) {
        coeffs[i] *= window[i];
    }
}

void generate_highpass_coeffs(double cutoff_freq, double sample_rate, uint16_t num_taps, double* coeffs) {
    double lowpass_coeffs[num_taps];
    generate_lowpass_coeffs(cutoff_freq, sample_rate, num_taps, lowpass_coeffs);

    for (uint16_t i = 0; i < num_taps; i++) {
        if (i == num_taps / 2) {
            coeffs[i] = 1 - lowpass_coeffs[i];
        } else {
            coeffs[i] = -lowpass_coeffs[i];
        }
    }
}

void generate_bandpass_coeffs(double low_cutoff_freq, double high_cutoff_freq, double sample_rate, uint16_t num_taps, double* coeffs) {
    double lowpass_coeffs[num_taps];
    double highpass_coeffs[num_taps];

    generate_lowpass_coeffs(high_cutoff_freq, sample_rate, num_taps, lowpass_coeffs);
    generate_highpass_coeffs(low_cutoff_freq, sample_rate, num_taps, highpass_coeffs);

    for (uint16_t i = 0; i < num_taps; i++) {
        coeffs[i] = lowpass_coeffs[i] + highpass_coeffs[i];
    }
}

void apply_fir_filter(double* input, uint16_t input_size, double* coeffs, uint16_t num_taps, double* output) {
    for (uint16_t i = 0; i < input_size; i++) {
        output[i] = 0.0;
        for (uint16_t j = 0; j < num_taps; j++) {
            if (i - j >= 0) {
                output[i] += input[i - j] * coeffs[j];
            }
        }
    }
}

void apply_data_compensation(int32_t* arr, size_t len, double* lowpass_coeffs, double* bandpass_coeffs, double* highpass_coeffs) {
    double lp_output[len];
    double bp_output[len];
    double hp_output[len];
    double input[len];

    for (size_t i = 0; i < len; i++) {
        input[i] = (double)arr[i];
    }

    apply_fir_filter(input, len, lowpass_coeffs, len, lp_output);
    apply_fir_filter(input, len, bandpass_coeffs, len, bp_output);
    apply_fir_filter(input, len, highpass_coeffs, len, hp_output);

    for (size_t i = 0; i < len; i++) {
        arr[i] = (int32_t)(1000 * lp_output[i] + 10 * bp_output[i] + hp_output[i]);
    }
}
