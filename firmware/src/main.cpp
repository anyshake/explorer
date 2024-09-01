#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

#include "utils/delay.h"
#include "utils/gpio.h"
#include "utils/led.h"
#include "utils/uart.h"
#include "utils/uptime.h"

#include "ads1262/regs/mode_0.h"
#include "ads1262/regs/mode_1.h"
#include "ads1262/regs/mode_2.h"

#include "array.h"
#include "packet.h"
#include "reader.h"
#include "settings.h"
#include "version.h"

typedef struct {
    // Startup settings
    uint8_t sample_span;
    uint8_t channel_samples;
    // ADC readout buffer
    int32_array_t* adc_channel_buffer;
    // UART transmission buffer
    uint8_array_t* uart_packet_buffer;
} explorer_states_t;

static explorer_states_t explorer_states;

void setup(void) {
    explorer_states.sample_span = 1000 / EXPLORER_SAMPLERATE;
    explorer_states.channel_samples = EXPLORER_LEGACY_MODE
                                          ? LEGACY_PACKET_CHANNEL_SAMPLES
                                          : MAINLINE_PACKET_CHANNEL_SAMPLES;

    // Allocate memory for data buffers
    explorer_states.adc_channel_buffer =
        array_int32_make(3 * explorer_states.channel_samples);
    if (!EXPLORER_LEGACY_MODE) {
        uint8_t packet_size =
            get_data_packet_size(explorer_states.channel_samples);
        explorer_states.uart_packet_buffer = array_uint8_make(packet_size);
    }

    // Initialize state LED pin
    mcu_utils_gpio_init(false);
    mcu_utils_gpio_mode(MCU_STATE_PIN, MCU_UTILS_GPIO_MODE_OUTPUT);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_delay_ms(1000, false);

    // Initialize serial port
    mcu_utils_uart_init(EXPLORER_BAUDRATE, false);
    mcu_utils_uart_flush();
    uint8_t version[] = FW_VERSION;
    mcu_utils_uart_write(version, sizeof(version), true);

    // Initialize ADS1262 ADC
    ads1262_init(ADS1262_CTL_PIN, ADS1262_INIT_CONTROL_TYPE_HARD, false);
    ads1262_reg_mode_0_t mode_0 = {.run_mode = ADS1262_MODE_0_RUN_MODE_ONESHOT};
    ads1262_reg_set_mode_0(&mode_0);
    ads1262_reg_mode_1_t mode_1 = {.filter = ADS1262_MODE_1_FILTER_SINC1};
    ads1262_reg_set_mode_1(&mode_1);
    ads1262_reg_mode_2_t mode_2 = {.dr = ADS1262_MODE_2_DR_1200};
    ads1262_reg_set_mode_2(&mode_2);

    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_gpio_high(MCU_STATE_PIN);
    mcu_utils_delay_ms(1000, false);
}

void loop(void) {
    int64_t current_timestamp = mcu_utils_uptime_ms();
    while (current_timestamp % (explorer_states.sample_span *
                                explorer_states.channel_samples) !=
           0) {
        current_timestamp = mcu_utils_uptime_ms();
    }

    get_adc_readout(ADS1262_CTL_PIN, explorer_states.adc_channel_buffer,
                    explorer_states.channel_samples, EXPLORER_24BIT_MODE);

    if (EXPLORER_LEGACY_MODE) {
        send_legacy_data_packet(explorer_states.adc_channel_buffer,
                                explorer_states.channel_samples);
    } else {
        send_data_packet(explorer_states.adc_channel_buffer,
                         explorer_states.uart_packet_buffer, current_timestamp,
                         explorer_states.channel_samples);
    }
}
