#ifndef __FILTER_H
#define __FILTER_H

#include <math.h>
#include <stdint.h>
#include <string.h>

#define IIR_DF1_ORDER 3

typedef struct {
    float b[IIR_DF1_ORDER];
    float a[IIR_DF1_ORDER];
    float x_h[IIR_DF1_ORDER];
    float y_h[IIR_DF1_ORDER];
} filter_iir_df1_t;

void filter_iir_df1_new(filter_iir_df1_t* filter, const float* b, const float* a);
int32_t apply_data_compensation(int32_t result, filter_iir_df1_t* df1_filter);

#endif
