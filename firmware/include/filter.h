#ifndef __FILTER_H
#define __FILTER_H

#include <stddef.h>
#include <stdint.h>

static const double LOWPASS_A[] = {0.0049854, 0.0099708, 0.0049854};
static const double BANDPASS_A[] = {0.0423174, 0., -0.0423174};
static const double HIGHPASS_A[] = {0.99778136, -1.99556272, 0.99778136};

void apply_compensation_filter(int32_t* arr, size_t len);

#endif
