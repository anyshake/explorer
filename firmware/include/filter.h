#ifndef __FILTER_H
#define __FILTER_H

#include <math.h>
#include <stdint.h>
#include <string.h>

#define FILTER_NUM_TAPS 101

typedef struct {
    double coeffs[FILTER_NUM_TAPS];
    double state[FILTER_NUM_TAPS - 1];
} filter_fir_t;

void filter_fir_apply(filter_fir_t* filter, double* input, uint16_t input_size, double* output);
void filter_fir_lowpass_get_coeffs(double cutoff_freq, double sample_rate, uint16_t num_taps, double* coeffs);
void filter_fir_highpass_get_coeffs(double cutoff_freq, double sample_rate, uint16_t num_taps, double* coeffs);
void filter_fir_bandpass_get_coeffs(double low_cutoff_freq, double high_cutoff_freq, double sample_rate, uint16_t num_taps, double* coeffs);
void apply_data_compensation(int32_t* arr, uint16_t len, filter_fir_t* lowpass_filter, filter_fir_t* bandpass_filter, filter_fir_t* highpass_filter);

#endif
