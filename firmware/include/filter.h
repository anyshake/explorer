#ifndef __FILTER_H
#define __FILTER_H

#include <math.h>
#include <stdint.h>
#include <string.h>

#include "settings.h"

#if ENABLE_COMPENSATION == true
const float COMPENSATION_COEFFS_B[] = {1.1565880101192887, -1.8583260658969014, 0.7775337237560691};
const float COMPENSATION_COEFFS_A[] = {1.0, -1.893528942802451, 0.8989188569698081};
#endif

#define IIR_DF1_ORDER 3

typedef struct {
    float b[IIR_DF1_ORDER];
    float a[IIR_DF1_ORDER];
    float x_h[IIR_DF1_ORDER];
    float y_h[IIR_DF1_ORDER];
} filter_iir_df1_t;

// To filter out low frequency noise lower than 0.2 Hz
// Which is the lowest frequency that can be measured by the geophone
const float PRE_PROC_HPF_COEFFS[][6] = {
    {0.98371517, -1.96743035, 0.98371517, 1.0, -1.97689135, 0.97704745},
    {1.0, -2.0, 1.0, 1.0, -1.99027124, 0.9904284},
};

#define IIR_SOS_SECTIONS 2

typedef struct {
    float sos[IIR_SOS_SECTIONS][6];
    float state[IIR_SOS_SECTIONS][2];
} filter_iir_sos_t;

void filter_iir_df1_new(filter_iir_df1_t* filter, const float* b, const float* a);
void apply_data_compensation(int32_t* arr, uint16_t len, filter_iir_df1_t* df1_filter);
void filter_iir_sos_new(filter_iir_sos_t* filter, const float sos[2][6]);
void filter_iir_sos_apply(filter_iir_sos_t* filter, float* input, uint16_t input_size, float* output);
void apply_data_pre_processing(int32_t* arr, uint16_t len, filter_iir_sos_t* pre_processing_filter);

#endif
