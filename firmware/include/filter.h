#ifndef __FILTER_H
#define __FILTER_H

#include <stdint.h>

typedef struct {
    double prev_lowpass[2];
    double prev_bandpass[2];
    double prev_highpass[2];
} filter_state_t;

static const double LOWPASS_A[] = {-1.96002769, 0.9608113};
static const double LOWPASS_B[] = {0.0001959, 0.00039181, 0.0001959};

static const double BANDPASS_A[] = {-1.96002769, 0.9608113};
static const double BANDPASS_B[] = {0.00831443, 0., -0.00831443};

static const double HIGHPASS_A[] = {-1.96002769, 0.9608113};
static const double HIGHPASS_B[] = {0.98020975, -1.9604195, 0.98020975};

void apply_compensation_filter(int32_t* arr, uint8_t len, filter_state_t* state);

#endif
