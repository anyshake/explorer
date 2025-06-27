#include "filter.h"

void filter_iir_df1_new(filter_iir_df1_t* filter, const float* b, const float* a) {
    for (uint8_t i = 0; i < IIR_DF1_ORDER; i++) {
        filter->b[i] = b[i];
        filter->a[i] = a[i];
        filter->x_h[i] = 0.0;
        filter->y_h[i] = 0.0;
    }
}

void filter_iir_df1_apply(filter_iir_df1_t* f, const float* input, float* output, uint16_t length) {
    for (uint16_t n = 0; n < length; n++) {
        for (uint8_t i = IIR_DF1_ORDER - 1; i > 0; i--) {
            f->x_h[i] = f->x_h[i - 1];
            f->y_h[i] = f->y_h[i - 1];
        }
        f->x_h[0] = input[n];

        float yn = 0.0f;
        for (uint8_t i = 0; i < IIR_DF1_ORDER; i++) {
            yn += f->b[i] * f->x_h[i];
        }
        for (uint8_t i = 1; i < IIR_DF1_ORDER; i++) {
            yn -= f->a[i] * f->y_h[i];
        }

        f->y_h[0] = yn;

        if (isnan(yn) || isinf(yn)) {
            yn = 0.0f;
        }

        output[n] = yn;
    }
}

void apply_data_compensation(int32_t* arr, uint16_t len, filter_iir_df1_t* df1_filter) {
    float input[len];
    for (uint16_t i = 0; i < len; i++) {
        input[i] = (float)arr[i];
    }

    float comp_output[len];
    filter_iir_df1_apply(df1_filter, input, comp_output, len);

    for (uint16_t i = 0; i < len; i++) {
        arr[i] = (int32_t)(comp_output[i] * 2.0f);
    }
}

void filter_iir_sos_new(filter_iir_sos_t* filter, const float sos[2][6]) {
    for (uint8_t i = 0; i < IIR_SOS_SECTIONS; i++) {
        for (uint8_t j = 0; j < 6; j++) {
            filter->sos[i][j] = sos[i][j];
        }
        for (uint8_t j = 0; j < 2; j++) {
            filter->state[i][j] = 0.0;
        }
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
