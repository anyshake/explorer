#include "User/Inc/filter.h"

/* scipy.signal.firwin(61, 26.7461839682361, window=("kaiser", 5.65326), fs=250).
 * The design cutoff gives -3.0103 dB at 25 Hz (firwin's cutoff is -6 dB).
 * Symmetric Q30 coefficients sum to exactly 1 << 30. Their absolute sum
 * is below 2 << 30, so full-scale int32 inputs fit in an int64 accumulator.
 * Unlike the previous Butterworth IIR, the 120 ms group delay is constant
 * across the passband and can be removed from the sample timestamps.
 */
static const int32_t ANTIALIAS_COEFFS[ANTIALIAS_TAPS] = {
    224896, 230527, -16116, -532453, -1110531, -1340874,
    -807029, 610603, 2489120, 3873499, 3647597, 1172717,
    -3107241, -7484016, -9493486, -7057386, 188636, 10073732,
    18194432, 19521516, 10847538, -6941579, -27940326, -42409190,
    -39983261, -13856233, 35721472, 100223599, 164705509, 212307652,
    229835176, 212307652, 164705509, 100223599, 35721472, -13856233,
    -39983261, -42409190, -27940326, -6941579, 10847538, 19521516,
    18194432, 10073732, 188636, -7057386, -9493486, -7484016,
    -3107241, 1172717, 3647597, 3873499, 2489120, 610603,
    -807029, -1340874, -1110531, -532453, -16116, 230527,
    224896,
};

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
    value /= (1LL << 30);
    if (value > INT32_MAX) {
        return INT32_MAX;
    }
    if (value < INT32_MIN) {
        return INT32_MIN;
    }
    return (int32_t)value;
}

/* Four phases of scipy.signal.firwin(161, 37.5, window=("kaiser", 5.65326), fs=1000).
 * Each phase is normalized separately to unity DC gain, quantized to Q30, and
 * adjusted so its integer coefficients sum to exactly 1 << 30. Phases 1 and 3
 * are reverses of one another; phases 0 and 2 are individually symmetric.
 * The FIR adds at most 0.012 dB error over 0..25 Hz to the existing 25 Hz LPF.
 * Its 161 taps at the virtual 1000 Hz rate give 80 ms of common group delay.
 * Phase 0 has 41 taps; phases 1..3 have 40 (the final array element is unused).
 * 100 Hz uses the original two phases (now numbered 0 and 2); 200 Hz uses all four.
 * Each phase's absolute coefficient sum is below 2 * 2^30, so full-scale int32
 * inputs fit safely in a signed 64-bit accumulator.
 */
static const int32_t RESAMPLE_250_COEFFS[4][RESAMPLE_250_PHASE_TAPS] = {
    {
        0, -577164, -1179040, -607561, 1725844, 4199196,
        3415876, -2422225, -9852038, -10898538, 0, 17825510,
        26552685, 10927477, -26467905, -58127864, -45305028, 33235500,
        158464148, 274841779, 322240520, 274841779, 158464148, 33235500,
        -45305028, -58127864, -26467905, 10927477, 26552685, 17825510,
        0, -10898538, -9852038, -2422225, 3415876, 4199196,
        1725844, -607561, -1179040, -577164, 0,
    },
    {
        -99586, -765408, -1194567, -171362, 2450477, 4441199,
        2402049, -4399124, -11043733, -9324030, 4294179, 21605225,
        25251266, 2945180, -36438585, -60462501, -31864290, 61880718,
        190993846, 295019970, 319157587, 250200363, 125427132, 7711348,
        -54427516, -52930177, -16208222, 17413328, 26241073, 13479852,
        -3798268, -11670574, -8252052, -571906, 4085315, 3746792,
        1021146, -921457, -1087671, -395192,
    },
    {
        -232931, -942404, -1113990, 378249, 3143477, 4417783,
        1058916, -6386716, -11689264, -6941092, 8872914, 24507536,
        22213732, -6235956, -45480685, -59376969, -14121701, 92904423,
        221923118, 309972472, 309972472, 221923118, 92904423, -14121701,
        -59376969, -45480685, -6235956, 22213732, 24507536, 8872914,
        -6941092, -11689264, -6386716, 1058916, 4417783, 3143477,
        378249, -1113990, -942404, -232931,
    },
    {
        -395192, -1087671, -921457, 1021146, 3746792, 4085315,
        -571906, -8252052, -11670574, -3798268, 13479852, 26241073,
        17413328, -16208222, -52930177, -54427516, 7711348, 125427132,
        250200363, 319157587, 295019970, 190993846, 61880718, -31864290,
        -60462501, -36438585, 2945180, 25251266, 21605225, 4294179,
        -9324030, -11043733, -4399124, 2402049, 4441199, 2450477,
        -171362, -1194567, -765408, -99586,
    },
};

void filter_resample_250_init(filter_resample_250_t* filter) {
    memset(filter, 0, sizeof(*filter));
}

static int32_t filter_resample_clamp(int64_t value, int32_t minimum, int32_t maximum) {
    value /= (1LL << 30);
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

/* Measured spacing between successive ADS1262 conversions (microseconds).
 * These are calibration values, not properties guaranteed by the ADC.
 */
#define ADC_CHANNEL_SPACING_US 685
#define ADC_US_PER_SECOND 1000000

void filter_adc_phase_init(filter_adc_phase_t* filter, uint8_t sample_rate) {
    memset(filter, 0, sizeof(*filter));
    filter->sample_rate = sample_rate;
}

void filter_adc_phase_apply(filter_adc_phase_t* filter, int32_t data[3]) {
    /* Samples from CH2/CH3 represent later instants than CH1; delay them
     * by 685/1370 us to refer all outputs to the earlier CH1 instant.
     * At 50..250 Hz both offsets are less than one sample, so the current
     * and previous values suffice without adding common sample latency.
     */
    const int32_t fractional_us[3] = {0, ADC_CHANNEL_SPACING_US, 2 * ADC_CHANNEL_SPACING_US};
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
    if (result == INT32_MAX) {
        return result;
    }
    float input = (float)result;
    float output = 0.0f;
    filter_iir_df1_apply(df1_filter, &input, &output, 1);

    return (int32_t)output;
}
