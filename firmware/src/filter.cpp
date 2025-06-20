#include "filter.h"

void filter_fir_apply(filter_fir_t* filter, float* input, uint16_t input_size, float* output) {
    float extended_input[(FIR_NUM_TAPS - 1) + input_size];

    memcpy(extended_input, filter->state, (FIR_NUM_TAPS - 1) * sizeof(float));
    memcpy(&extended_input[FIR_NUM_TAPS - 1], input, input_size * sizeof(float));

    for (uint16_t i = 0; i < input_size; i++) {
        output[i] = 0.0;
        for (uint16_t j = 0; j < FIR_NUM_TAPS; j++) {
            output[i] += extended_input[i + j] * filter->coeffs[j];
        }
    }

    memcpy(filter->state, &extended_input[input_size], (FIR_NUM_TAPS - 1) * sizeof(float));
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
        float result = (LPF_MAG * lp_output[i] + BPF_MAG * bp_output[i] + HPF_MAG * hp_output[i]) * 2;
        arr[i] = (int32_t)result;
    }
}

void filter_iir_sos_apply(filter_iir_sos_t* filter, float* input, uint16_t input_size, float* output) {
    for (uint16_t n = 0; n < input_size; ++n) {
        float x = input[n];

        for (uint16_t s = 0; s < IIR_SOS_SECTIONS; ++s) {
            float* coeff = filter->sos[s];
            float* z = filter->state[s];

            float b0 = coeff[0], b1 = coeff[1], b2 = coeff[2];
            float a1 = coeff[4], a2 = coeff[5];

            float y = b0 * x + z[0];
            z[0] = b1 * x - a1 * y + z[1];
            z[1] = b2 * x - a2 * y;

            x = y;
        }

        output[n] = x;
    }
}

void apply_data_pre_processing(int32_t* arr, uint16_t len, filter_iir_sos_t* pre_processing_filter) {
    float input[len];
    for (uint16_t i = 0; i < len; i++) {
        input[i] = (float)arr[i];
    }

    float output[len];
    filter_iir_sos_apply(pre_processing_filter, input, len, output);

    for (uint16_t i = 0; i < len; i++) {
        arr[i] = (int32_t)output[i];
    }
}
