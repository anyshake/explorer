#ifndef __FILTER_H
#define __FILTER_H

#include <stddef.h>
#include <stdint.h>

#define FILTER_NUM_TAPS 10

void hamming_window(uint16_t n, double* window);
void generate_lowpass_coeffs(double cutoff_freq, double sample_rate, uint16_t num_taps, double* coeffs);
void generate_highpass_coeffs(double cutoff_freq, double sample_rate, uint16_t num_taps, double* coeffs);
void generate_bandpass_coeffs(double low_cutoff_freq, double high_cutoff_freq, double sample_rate, uint16_t num_taps, double* coeffs);
void apply_fir_filter(double* input, uint16_t input_size, double* coeffs, uint16_t num_taps, double* output);
void apply_data_compensation(int32_t* arr, size_t len, double* lowpass_coeffs, double* bandpass_coeffs, double* highpass_coeffs);

#endif
