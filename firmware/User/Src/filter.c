#include "User/Inc/filter.h"

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

int32_t apply_data_compensation(int32_t result, filter_iir_df1_t* df1_filter) {
    float input = (float)result;
    float output = 0.0f;
    filter_iir_df1_apply(df1_filter, &input, &output, 1);

    return (int32_t)output;
}
