#ifndef __FILTER_H
#define __FILTER_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define IIR_DF1_ORDER 3
#define ANTIALIAS_TAPS 61
#define ANTIALIAS_DELAY_MS 120
#define RESAMPLE_250_PHASE_TAPS 41
#define RESAMPLE_250_DELAY_MS 80

typedef struct {
    float b[IIR_DF1_ORDER];
    float a[IIR_DF1_ORDER];
    float x_h[IIR_DF1_ORDER];
    float y_h[IIR_DF1_ORDER];
} filter_iir_df1_t;

/* Linear-phase low-pass: fs = 250 Hz, fc = 25 Hz (-3 dB).
 * The 30-sample group delay must be removed from acquisition timestamps.
 */
typedef struct {
    int32_t history[ANTIALIAS_TAPS];
    uint8_t position;
    uint8_t invalid_count;
} filter_antialias_t;

void filter_antialias_init(filter_antialias_t* filter);
int32_t filter_antialias_apply(filter_antialias_t* filter, int32_t data);

/* Polyphase FIR for 250 -> 100/200 Hz. All active channels share one output clock.
 * ADC error markers are stored as validity bits, never as numeric FIR inputs.
 */
typedef struct {
    int32_t adc_history[3][RESAMPLE_250_PHASE_TAPS];
    int16_t accel_history[3][RESAMPLE_250_PHASE_TAPS];
    uint8_t adc_invalid[RESAMPLE_250_PHASE_TAPS];
    uint8_t position;
    uint8_t filled;
    uint8_t input_phase;
} filter_resample_250_t;

void filter_resample_250_init(filter_resample_250_t* filter);
/* sample_rate must be 100 or 200; pass NULL for inactive channel groups.
 * Returns true only for an output frame,
 * after the history is full; timestamp_ms is corrected for FIR delay and phase.
 */
bool filter_resample_250_apply(filter_resample_250_t* filter, uint8_t sample_rate,
                              int32_t adc_data[3], int16_t accel_data[3], int64_t* timestamp_ms);

/* Causal fractional delay using the current and previous samples.
 * CH2/CH3 incur 685/1370 us to align with CH1's earlier sample;
 * CH1 has no additional delay.
 */
typedef struct {
    int32_t previous[3];
    uint8_t initialized; // Bit mask of channels with valid history.
    uint8_t sample_rate;
} filter_adc_phase_t;

void filter_adc_phase_init(filter_adc_phase_t* filter, uint8_t sample_rate);
void filter_adc_phase_apply(filter_adc_phase_t* filter, int32_t data[3]);

void filter_iir_df1_new(filter_iir_df1_t* filter, const float* b, const float* a);
int32_t apply_data_compensation(int32_t result, filter_iir_df1_t* df1_filter);

#endif
