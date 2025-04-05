#ifndef __FILTER_H
#define __FILTER_H

#include <math.h>
#include <stdint.h>
#include <string.h>

#define FILTER_NUM_TAPS 301

typedef struct {
    float coeffs[FILTER_NUM_TAPS];
    float state[FILTER_NUM_TAPS - 1];
} filter_fir_t;

void filter_fir_apply(filter_fir_t* filter, float* input, uint16_t input_size, float* output);
void filter_fir_lowpass_get_coeffs(float cutoff_freq, float sample_rate, uint16_t num_taps, float* coeffs);
void filter_fir_highpass_get_coeffs(float cutoff_freq, float sample_rate, uint16_t num_taps, float* coeffs);
void filter_fir_bandpass_get_coeffs(float low_cutoff_freq, float high_cutoff_freq, float sample_rate, uint16_t num_taps, float* coeffs);
void apply_data_compensation(int32_t* arr, uint16_t len, filter_fir_t* lowpass_filter, filter_fir_t* bandpass_filter, filter_fir_t* highpass_filter);

#endif
