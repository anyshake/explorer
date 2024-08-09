#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

#include "utils/crc32.h"
#include "utils/delay.h"
#include "utils/gpio.h"
#include "utils/led.h"
#include "utils/uart.h"
#include "utils/uptime.h"

#include "ads1262/cmds/rdata.h"
#include "ads1262/regs/id.h"
#include "ads1262/regs/inpmux.h"
#include "ads1262/regs/mode_0.h"
#include "ads1262/regs/mode_1.h"
#include "ads1262/regs/mode_2.h"

#include "array.h"
#include "packet.h"
#include "settings.h"
#include "version.h"

typedef struct {
    // Startup settings
    uint64_t sample_index;
    int64_t prev_time;
    uint8_t sample_span;
    bool led_state_on;
    // ADC mutiplexer, data result
    ads1262_reg_inpmux_t inpmux;
    ads1262_cmd_rdata_t rdata;
    // ADC data buffer and RTOS resources
    int32_array_t* adc_channel_buffer;
    // Packet buffer for UART transmission
    uint8_array_t* uart_packet_buffer;
} explorer_states_t;

static explorer_states_t explorer_states;

void setup(void) {
    // Allocate memory for data buffers
    explorer_states.adc_channel_buffer =
        array_int32_make(3 * EXPLORER_SAMPLERATE);
    explorer_states.uart_packet_buffer =
        array_uint8_make(get_data_packet_size(EXPLORER_SAMPLERATE));

    // Initialize state LED pin
    mcu_utils_gpio_init(false);
    mcu_utils_gpio_mode(MCU_STATE_PIN, MCU_UTILS_GPIO_MODE_OUTPUT);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_delay_ms(1000, false);

    // Initialize serial port
    mcu_utils_uart_init(EXPLORER_BAUDRATE, false);
    mcu_utils_uart_flush();

    // Initialize ADS1262 ADC
    ads1262_init(ADS1262_CTL_PIN, ADS1262_INIT_CONTROL_TYPE_HARD, false);
    ads1262_reg_mode_0_t mode_0 = {.run_mode = ADS1262_MODE_0_RUN_MODE_ONESHOT};
    ads1262_reg_set_mode_0(&mode_0);
    ads1262_reg_mode_1_t mode_1 = {.filter = ADS1262_MODE_1_FILTER_SINC1};
    ads1262_reg_set_mode_1(&mode_1);
    ads1262_reg_mode_2_t mode_2 = {.dr = ADS1262_MODE_2_DR_1200};
    ads1262_reg_set_mode_2(&mode_2);

    // Initialize CRC32 module
    mcu_utils_crc32_init();

    explorer_states.sample_index = 0;
    explorer_states.prev_time = mcu_utils_uptime_ms();
    explorer_states.sample_span = 1000 / EXPLORER_SAMPLERATE;

    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_gpio_high(MCU_STATE_PIN);
    mcu_utils_delay_ms(1000, false);
}

void loop(void) {
    int64_t current_time = mcu_utils_uptime_ms();
    if (current_time - explorer_states.prev_time >=
        explorer_states.sample_span) {
        explorer_states.prev_time = current_time;

        // Read Z-axis geophone data (AIN0, AIN1)
        explorer_states.inpmux.mux_p = ADS1262_INPMUX_AIN0;
        explorer_states.inpmux.mux_n = ADS1262_INPMUX_AIN1;
        ads1262_reg_set_inpmux(&explorer_states.inpmux);
        ads1262_cmd_rdata(ADS1262_CTL_PIN, &explorer_states.rdata,
                          ADS1262_INIT_CONTROL_TYPE_HARD);
        if (EXPLORER_24BIT_MODE) {
            int32_t data_24bit = (explorer_states.rdata.data >> 8) & 0xFFFFFF;
            explorer_states.adc_channel_buffer
                ->data[explorer_states.sample_index % EXPLORER_SAMPLERATE] =
                data_24bit & 0x800000 ? data_24bit | 0xFF000000 : data_24bit;
        } else {
            explorer_states.adc_channel_buffer
                ->data[explorer_states.sample_index % EXPLORER_SAMPLERATE] =
                explorer_states.rdata.data;
        }

        // Read E-axis geophone data (AIN2, AIN3)
        explorer_states.inpmux.mux_p = ADS1262_INPMUX_AIN2;
        explorer_states.inpmux.mux_n = ADS1262_INPMUX_AIN3;
        ads1262_reg_set_inpmux(&explorer_states.inpmux);
        ads1262_cmd_rdata(ADS1262_CTL_PIN, &explorer_states.rdata,
                          ADS1262_INIT_CONTROL_TYPE_HARD);
        if (EXPLORER_24BIT_MODE) {
            int32_t data_24bit = (explorer_states.rdata.data >> 8) & 0xFFFFFF;
            explorer_states.adc_channel_buffer
                ->data[(explorer_states.sample_index % EXPLORER_SAMPLERATE) +
                       EXPLORER_SAMPLERATE] =
                data_24bit & 0x800000 ? data_24bit | 0xFF000000 : data_24bit;
        } else {
            explorer_states.adc_channel_buffer
                ->data[(explorer_states.sample_index % EXPLORER_SAMPLERATE) +
                       EXPLORER_SAMPLERATE] = explorer_states.rdata.data;
        }

        // Read sample_index-axis geophone data (AIN4, AIN5)
        explorer_states.inpmux.mux_p = ADS1262_INPMUX_AIN4;
        explorer_states.inpmux.mux_n = ADS1262_INPMUX_AIN5;
        ads1262_reg_set_inpmux(&explorer_states.inpmux);
        ads1262_cmd_rdata(ADS1262_CTL_PIN, &explorer_states.rdata,
                          ADS1262_INIT_CONTROL_TYPE_HARD);
        if (EXPLORER_24BIT_MODE) {
            int32_t data_24bit = (explorer_states.rdata.data >> 8) & 0xFFFFFF;
            explorer_states.adc_channel_buffer
                ->data[(explorer_states.sample_index % EXPLORER_SAMPLERATE) +
                       2 * EXPLORER_SAMPLERATE] =
                data_24bit & 0x800000 ? data_24bit | 0xFF000000 : data_24bit;
        } else {
            explorer_states.adc_channel_buffer
                ->data[(explorer_states.sample_index % EXPLORER_SAMPLERATE) +
                       2 * EXPLORER_SAMPLERATE] = explorer_states.rdata.data;
        }

        if (explorer_states.sample_index &&
            explorer_states.sample_index % EXPLORER_SAMPLERATE == 0) {
            // Send data packet over UART
            send_data_packet(explorer_states.adc_channel_buffer,
                             explorer_states.uart_packet_buffer,
                             EXPLORER_SAMPLERATE, EXPLORER_DEV_ID);
        }

        explorer_states.sample_index++;
    }
}
