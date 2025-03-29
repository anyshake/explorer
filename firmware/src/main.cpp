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
#include "ads1262/regs/mode_1.h"
#include "ads1262/regs/mode_2.h"

#include "array.h"
#include "calibration.h"
#include "filter.h"
#include "packet.h"
#include "reader.h"
#include "settings.h"

typedef struct {
    uint8_t sample_time_span;
    uint8_t channel_samples;

    int64_t timestamp;
    uint8_t sample_pos = 0;
    int32_t adc_data_buf[3];

    int32_t adc_readout_z_axis[MAINLINE_PACKET_CHANNEL_SAMPLES];
    int32_t adc_readout_e_axis[MAINLINE_PACKET_CHANNEL_SAMPLES];
    int32_t adc_readout_n_axis[MAINLINE_PACKET_CHANNEL_SAMPLES];

    uint8_array_t* uart_packet_buffer;
} explorer_states_t;

static explorer_states_t explorer_states;

void setup(void) {
    explorer_states.sample_time_span = 1000 / EXPLORER_SAMPLERATE;
    explorer_states.sample_pos = 0;
    explorer_states.timestamp = 0;

    uint8_t packet_size = get_data_packet_size(MAINLINE_PACKET_CHANNEL_SAMPLES);
    explorer_states.uart_packet_buffer = array_uint8_make(packet_size);

    mcu_utils_gpio_init(false);
    mcu_utils_gpio_mode(MCU_STATE_PIN, MCU_UTILS_GPIO_MODE_OUTPUT);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_delay_ms(1000, false);

    mcu_utils_uart_init(EXPLORER_BAUDRATE, false);
    mcu_utils_uart_flush();

    char fw_info_buf[43];
    snprintf(fw_info_buf, sizeof(fw_info_buf), "FW REV: %s\r\nBUILD: %s\r\n", FW_REV, FW_BUILD);
    mcu_utils_uart_write((uint8_t*)fw_info_buf, sizeof(fw_info_buf), true);

    mcu_utils_spi_init(false);
    ads1262_reset(ADS1262_CTL_PIN, ADS1262_RESET_RESET_TYPE_HARD, false);
    ads1262_init(ADS1262_CTL_PIN, ADS1262_INIT_CONTROL_TYPE_HARD);

    ads1262_reg_interface_t ads1262_reg_interface = ads1262_reg_new_interface();
    ads1262_reg_interface.status = ADS1262_REG_INTERFACE_STATUS_ENABLED;
    ads1262_reg_interface.crc = ADS1262_REG_INTERFACE_CRC_CRC;
    ads1262_reg_set_interface(&ads1262_reg_interface);

    ads1262_reg_mode_0_t ads1262_reg_mode_0 = ads1262_reg_new_mode_0();
    ads1262_reg_mode_0.run_mode = ADS1262_REG_MODE_0_RUN_MODE_ONESHOT;
    ads1262_reg_set_mode_0(&ads1262_reg_mode_0);

    ads1262_reg_mode_1_t ads1262_reg_mode_1 = ads1262_reg_new_mode_1();
    ads1262_reg_mode_1.filter = ADS1262_REG_MODE_1_FILTER_SINC1;
    ads1262_reg_set_mode_1(&ads1262_reg_mode_1);

    ads1262_reg_mode_2_t ads1262_reg_mode_2 = ads1262_reg_new_mode_2();
    ads1262_reg_mode_2.dr = ADS1262_REG_MODE_2_DR_4800;
    ads1262_reg_mode_2.gain = ADS1262_REG_MODE_2_GAIN_4;
    ads1262_reg_set_mode_2(&ads1262_reg_mode_2);

    calibrate_adc_offset(ADS1262_CTL_PIN);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);

    mcu_utils_delay_ms(1000, false);
    mcu_utils_gpio_high(MCU_STATE_PIN);
}

void loop(void) {
    static uint32_t next_sample_time = 0;
    uint32_t now = mcu_utils_uptime_ms();

    if (now < next_sample_time) {
        return;
    }
    next_sample_time = now + explorer_states.sample_time_span;

    if (explorer_states.sample_pos == 0) {
        explorer_states.timestamp = now;
    }

    get_adc_readout(ADS1262_CTL_PIN, explorer_states.adc_data_buf);
    explorer_states.adc_readout_z_axis[explorer_states.sample_pos] = explorer_states.adc_data_buf[0];
    explorer_states.adc_readout_e_axis[explorer_states.sample_pos] = explorer_states.adc_data_buf[1];
    explorer_states.adc_readout_n_axis[explorer_states.sample_pos] = explorer_states.adc_data_buf[2];
    explorer_states.sample_pos++;

    if (explorer_states.sample_pos >= MAINLINE_PACKET_CHANNEL_SAMPLES) {
#if ENABLE_COMPENSATION == true
        apply_compensation_filter(explorer_states.adc_readout_z_axis, MAINLINE_PACKET_CHANNEL_SAMPLES);
        apply_compensation_filter(explorer_states.adc_readout_e_axis, MAINLINE_PACKET_CHANNEL_SAMPLES);
        apply_compensation_filter(explorer_states.adc_readout_n_axis, MAINLINE_PACKET_CHANNEL_SAMPLES);
#endif
        send_data_packet(explorer_states.timestamp,
                         explorer_states.adc_readout_z_axis,
                         explorer_states.adc_readout_e_axis,
                         explorer_states.adc_readout_n_axis,
                         MAINLINE_PACKET_CHANNEL_SAMPLES,
                         explorer_states.uart_packet_buffer);
        explorer_states.sample_pos = 0;
    }
}
