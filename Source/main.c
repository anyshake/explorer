#include <string.h>

#include "systick.h"
#include "gd32f30x.h"
#include "gd32f303c_start.h"

#include "blink.h"
#include "delay.h"
#include "config.h"
#include "serial.h"
#include "checksum.h"

#include "ads1256/utils.h"
#include "ads1256/gain.h"
#include "ads1256/reader.h"
#include "ads1256/sample.h"
#include "ads1256/status.h"
#include "ads1256/channel.h"

void send_sensor_data(sensor_t* dat) {
    uint8_t dataBytes[sizeof(sensor_t)];
    memcpy(dataBytes, dat, sizeof(sensor_t));

    for (uint8_t i = 0; i < sizeof(sensor_t); i++) {
        serial_write(dataBytes[i]);
    }
}

int32_t get_adc_raw(int32_t value) {
    if (value >> 23) {
        value -= 16777216;
    }

    return value;
}

uint8_t is_reset() {
    uint8_t gain = adc_get_gain();
    uint8_t sample = adc_get_sample();
    if (gain != GAIN_RATE || sample != SAMPLE_RATE) {
        return 1;
    }

    return serial_available() && serial_read() == RESET_WORD;
}

void init_adc() {
    adc_begin();
    adc_reset();

    adc_set_gain(GAIN_RATE);
    adc_set_sample(SAMPLE_RATE);
    adc_set_status(ENABLE, DISABLE);
}

void setup() {
    serial_begin(SERIAL_BAUD);
    blink_led(3, 50);

    init_adc();
    blink_led(5, 50);
}

void loop() {
    sensor_t sensor;

    // Get voltage data
    for (uint16_t i = 0; i < PACKET_SIZE; i++) {
        // Support runtime reset
        if (is_reset()) {
            init_adc();
            blink_led(1, 50);

            for (uint8_t i = 0; i < sizeof(ACK_WORD); i++) {
                serial_write(ACK_WORD[i]);
            }
        }

        // Vertical geophone (EHZ)
        sensor.EHZ[i] = get_adc_raw(adc_get_differential(ANALOG_INPUT_AIN1, ANALOG_INPUT_AINCOM));
        adc_get_differential(ANALOG_INPUT_AIN2, ANALOG_INPUT_AINCOM);
        adc_get_differential(ANALOG_INPUT_AIN3, ANALOG_INPUT_AINCOM);

        // East-West geophone (EHE)
        sensor.EHE[i] = get_adc_raw(adc_get_differential(ANALOG_INPUT_AIN4, ANALOG_INPUT_AINCOM));
        adc_get_differential(ANALOG_INPUT_AIN5, ANALOG_INPUT_AINCOM);
        adc_get_differential(ANALOG_INPUT_AIN6, ANALOG_INPUT_AINCOM);

        // North-South geophone (EHN)
        sensor.EHN[i] = get_adc_raw(adc_get_differential(ANALOG_INPUT_AIN7, ANALOG_INPUT_AINCOM));
        adc_get_differential(ANALOG_INPUT_AIN8, ANALOG_INPUT_AINCOM);
    }

    // Get checksum
    sensor.Checksum[0] = get_checksum(sensor.EHZ, PACKET_SIZE);
    sensor.Checksum[1] = get_checksum(sensor.EHE, PACKET_SIZE);
    sensor.Checksum[2] = get_checksum(sensor.EHN, PACKET_SIZE);

    // Send struct data
    for (uint8_t i = 0; i < sizeof(SYNC_WORD); i++) {
        serial_write(SYNC_WORD[i]);
    }
    send_sensor_data(&sensor);
}

int main() {
    systick();
    delay_init();

    setup();
    while (1) {
        loop();
    }
}
