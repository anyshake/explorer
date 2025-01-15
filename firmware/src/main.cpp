#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "utils/delay.h"
#include "utils/gpio.h"
#include "utils/led.h"
#include "utils/uart.h"
#include "utils/uptime.h"

#include "ads1262/regs/interface.h"
#include "ads1262/regs/mode_0.h"
#include "ads1262/regs/mode_2.h"

#include "array.h"
#include "calibration.h"
#include "packet.h"
#include "reader.h"
#include "settings.h"
#include "version.h"

typedef struct {
    uint8_t sample_span;
    uint8_t channel_samples;

    int32_t adc_readout_z_axis[MAINLINE_PACKET_CHANNEL_SAMPLES];
    int32_t adc_readout_e_axis[MAINLINE_PACKET_CHANNEL_SAMPLES];
    int32_t adc_readout_n_axis[MAINLINE_PACKET_CHANNEL_SAMPLES];

    filter_state_t filter_state_z_axis;
    filter_state_t filter_state_e_axis;
    filter_state_t filter_state_n_axis;

    uint8_array_t* uart_packet_buffer;
} explorer_states_t;

static explorer_states_t explorer_states;

void setup(void) {
    explorer_states.sample_span = 1000 / EXPLORER_SAMPLERATE;

    uint8_t packet_size = get_data_packet_size(MAINLINE_PACKET_CHANNEL_SAMPLES);
    explorer_states.uart_packet_buffer = array_uint8_make(packet_size);

    mcu_utils_gpio_init(false);
    mcu_utils_gpio_mode(MCU_STATE_PIN, MCU_UTILS_GPIO_MODE_OUTPUT);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_delay_ms(1000, false);

    mcu_utils_uart_init(EXPLORER_BAUDRATE, false);
    mcu_utils_uart_flush();
    uint8_t version[] = FW_VERSION;
    mcu_utils_uart_write(version, sizeof(version), true);

    ads1262_init(ADS1262_CTL_PIN, ADS1262_INIT_CONTROL_TYPE_HARD, false);
    ads1262_reg_interface_t interface = {
        .status = ADS1262_INTERFACE_STATUS_ENABLED,
        .crc = ADS1262_INTERFACE_CRC_CRC,
    };
    ads1262_reg_set_interface(&interface);
    ads1262_reg_mode_0_t mode_0 = {
        .run_mode = ADS1262_MODE_0_RUN_MODE_ONESHOT,
    };
    ads1262_reg_set_mode_0(&mode_0);
    ads1262_reg_mode_2_t mode_2 = {
        .dr = ADS1262_MODE_2_DR_1200,
    };
    ads1262_reg_set_mode_2(&mode_2);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);

    calibrate_adc_offset(ADS1262_CTL_PIN);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);

    mcu_utils_delay_ms(1000, false);
    mcu_utils_gpio_high(MCU_STATE_PIN);
}

void loop(void) {
    int64_t current_timestamp = mcu_utils_uptime_ms();
    while (current_timestamp % (explorer_states.sample_span * MAINLINE_PACKET_CHANNEL_SAMPLES) != 0) {
        current_timestamp = mcu_utils_uptime_ms();
    }

    get_adc_readout(ADS1262_CTL_PIN,
                    explorer_states.adc_readout_z_axis,
                    explorer_states.adc_readout_e_axis,
                    explorer_states.adc_readout_n_axis,
                    &explorer_states.filter_state_z_axis,
                    &explorer_states.filter_state_e_axis,
                    &explorer_states.filter_state_n_axis,
                    MAINLINE_PACKET_CHANNEL_SAMPLES);
    send_data_packet(current_timestamp,
                     explorer_states.adc_readout_z_axis,
                     explorer_states.adc_readout_e_axis,
                     explorer_states.adc_readout_n_axis,
                     MAINLINE_PACKET_CHANNEL_SAMPLES,
                     explorer_states.uart_packet_buffer);
}
