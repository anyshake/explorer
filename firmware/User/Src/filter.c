#include "User/Inc/filter.h"

void filter_antialias_init(filter_antialias_t* filter) {
    memset(filter, 0, sizeof(*filter));
}

int32_t filter_antialias_apply(filter_antialias_t* filter, int32_t data) {
    uint8_t newest = filter->position;
    if (filter->history[newest] == INT32_MAX) {
        filter->invalid_count--;
    }
    filter->history[newest] = data;
    if (data == INT32_MAX) {
        filter->invalid_count++;
    }
    if (++filter->position == ANTIALIAS_TAPS) {
        filter->position = 0;
    }
    // Never multiply an error marker; invalidate its entire FIR footprint.
    if (filter->invalid_count != 0) {
        return INT32_MAX;
    }

    int64_t value = 0;
    uint8_t index = newest;
    for (uint8_t tap = 0; tap < ANTIALIAS_TAPS; tap++) {
        value += (int64_t)ANTIALIAS_COEFFS[tap] * filter->history[index];
        index = index == 0 ? ANTIALIAS_TAPS - 1 : index - 1;
    }
    value /= FILTER_COEFF_SCALE;
    if (value > INT32_MAX) {
        return INT32_MAX;
    }
    if (value < INT32_MIN) {
        return INT32_MIN;
    }
    return (int32_t)value;
}

void filter_resample_250_init(filter_resample_250_t* filter) {
    memset(filter, 0, sizeof(*filter));
}

static int32_t filter_resample_clamp(int64_t value, int32_t minimum, int32_t maximum) {
    value /= FILTER_COEFF_SCALE;
    if (value > maximum) {
        return maximum;
    }
    if (value < minimum) {
        return minimum;
    }
    return (int32_t)value;
}

bool filter_resample_250_apply(filter_resample_250_t* filter, uint8_t sample_rate,
                              int32_t adc_data[3], int16_t accel_data[3], int64_t* timestamp_ms) {
    if (sample_rate != 100 && sample_rate != 200) {
        return false;
    }
    uint8_t newest = filter->position;
    filter->adc_invalid[newest] = 0;
    for (uint8_t ch = 0; ch < 3; ch++) {
        if (adc_data != NULL) {
            if (adc_data[ch] == INT32_MAX) {
                filter->adc_invalid[newest] |= 1U << ch;
                filter->adc_history[ch][newest] = 0;
            } else {
                filter->adc_history[ch][newest] = adc_data[ch];
            }
        }
        if (accel_data != NULL) {
            filter->accel_history[ch][newest] = accel_data[ch];
        }
    }
    if (++filter->position == RESAMPLE_250_PHASE_TAPS) {
        filter->position = 0;
    }
    if (filter->filled < RESAMPLE_250_PHASE_TAPS) {
        filter->filled++;
    }

    // In each five-input cycle, 100 Hz emits phases 0/2 and 200 Hz emits 0/1/2/3.
    uint8_t input_phase = filter->input_phase;
    if (++filter->input_phase == 5) {
        filter->input_phase = 0;
    }
    bool output_due = sample_rate == 200 ? input_phase < 4 : (input_phase == 0 || input_phase == 2);
    if (filter->filled < RESAMPLE_250_PHASE_TAPS || !output_due) {
        return false;
    }
    uint8_t coefficient_phase = input_phase;
    uint8_t tap_count = RESAMPLE_250_PHASE_TAPS - (coefficient_phase != 0);
    for (uint8_t ch = 0; ch < 3; ch++) {
        int64_t adc_sum = 0;
        int64_t accel_sum = 0;
        bool adc_valid = true;
        uint8_t index = newest;
        for (uint8_t tap = 0; tap < tap_count; tap++) {
            int32_t coefficient = RESAMPLE_250_COEFFS[coefficient_phase][tap];
            if (adc_data != NULL) {
                if (filter->adc_invalid[index] & (1U << ch)) {
                    adc_valid = false;
                }
                adc_sum += (int64_t)coefficient * filter->adc_history[ch][index];
            }
            if (accel_data != NULL) {
                accel_sum += (int64_t)coefficient * filter->accel_history[ch][index];
            }
            index = index == 0 ? RESAMPLE_250_PHASE_TAPS - 1 : index - 1;
        }
        if (adc_data != NULL) {
            adc_data[ch] = adc_valid ? filter_resample_clamp(adc_sum, INT32_MIN, INT32_MAX) : INT32_MAX;
        }
        if (accel_data != NULL) {
            accel_data[ch] = (int16_t)filter_resample_clamp(accel_sum, INT16_MIN, INT16_MAX);
        }
    }

    // The virtual FIR phase adds 0..3 ms after the latest input, with 80 ms FIR delay.
    *timestamp_ms -= RESAMPLE_250_DELAY_MS - coefficient_phase;
    return true;
}

void filter_adc_phase_init(filter_adc_phase_t* filter, uint8_t sample_rate, bool channel_6d) {
    memset(filter, 0, sizeof(*filter));
    filter->sample_rate = sample_rate;
    filter->channel_6d = channel_6d;
}

void filter_adc_phase_apply(filter_adc_phase_t* filter, int32_t data[3]) {
    /* Delay CH2/CH3 using their independently calibrated SINC2 offsets
     * relative to CH1; CH1's time reference is unchanged.
     * At 50..250 Hz both offsets are less than one sample, so the current
     * and previous values suffice without adding common sample latency.
     */
    const int32_t* fractional_us = ADC_CHANNEL_DELAYS_US[filter->channel_6d];
    for (uint8_t ch = 0; ch < 3; ch++) {
        uint8_t channel_mask = 1U << ch;
        if (data[ch] == INT32_MAX) {
            filter->initialized &= ~channel_mask;
            continue;
        }
        if (!(filter->initialized & channel_mask)) {
            filter->previous[ch] = data[ch];
            filter->initialized |= channel_mask;
        }
        int32_t current = data[ch];
        int32_t fraction = fractional_us[ch] * filter->sample_rate;
        /* 64-bit intermediates prevent overflow for full-scale ADC codes. */
        int64_t interpolated = ((int64_t)(ADC_US_PER_SECOND - fraction) * current +
                                (int64_t)fraction * filter->previous[ch]) / ADC_US_PER_SECOND;
        data[ch] = (int32_t)interpolated;
        filter->previous[ch] = current;
    }
}

void filter_iir_df1_new(filter_iir_df1_t* filter, const float* b, const float* a) {
    for (uint8_t i = 0; i < IIR_DF1_ORDER; i++) {
        filter->b[i] = b[i];
        filter->a[i] = a[i];
        filter->x_h[i] = 0.0f;
        filter->y_h[i] = 0.0f;
    }
    filter->initialized = false;
}

void filter_iir_df1_apply(filter_iir_df1_t* f, const float* input, float* output, uint16_t length) {
    for (uint16_t n = 0; n < length; n++) {
        if (!f->initialized) {
            float b_sum = 0.0f;
            float a_sum = 0.0f;
            for (uint8_t i = 0; i < IIR_DF1_ORDER; i++) {
                b_sum += f->b[i];
                a_sum += f->a[i];
            }
            // Prime the constant-input steady state after startup or a gap.
            // Zero/stale history would excite the compensation's slow poles.
            float initial_output = input[n] * (b_sum / a_sum);
            for (uint8_t i = 0; i < IIR_DF1_ORDER; i++) {
                f->x_h[i] = input[n];
                f->y_h[i] = initial_output;
            }
            f->initialized = true;
        }
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

        if (isfinite(yn)) {
            f->y_h[0] = yn;
        } else {
            // Do not retain a nonfinite value in the feedback history.
            f->initialized = false;
        }

        output[n] = yn;
    }
}

int32_t apply_data_compensation(int32_t result, filter_iir_df1_t* df1_filter) {
    if (result == INT32_MAX) {
        // The antialias FIR can invalidate 61 frames after one ADC failure.
        df1_filter->initialized = false;
        return result;
    }
    float input = (float)result;
    float output = 0.0f;
    filter_iir_df1_apply(df1_filter, &input, &output, 1);

    // The float representation of INT32_MAX rounds up to 2^31.
    // Check before casting so a bad/overrange output cannot become a spike.
    if (!isfinite(output) || output >= (float)INT32_MAX || output < (float)INT32_MIN) {
        df1_filter->initialized = false;
        return INT32_MAX;
    }

    return (int32_t)output;
}
