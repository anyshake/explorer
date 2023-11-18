#include <Arduino.h>

#include "ads1256/adc.hpp"
#include "ads1256/channel.hpp"
#include "ads1256/gain.hpp"
#include "ads1256/sample.hpp"

#include "blink.hpp"
#include "checksum.hpp"
#include "config.hpp"

ADS1256 adc(SS, PA3, PA2);

int32_t getRawValue(int32_t value) {
    if (value >> 23) {
        value -= 16777216;
    }

    return value;
}

uint8_t shouldReset() {
    uint8_t gain = adc.getGain();
    uint8_t sample = adc.getSample();
    if (gain != GAIN_RATE || sample != SAMPLE_RATE) {
        return 1;
    }

    uint8_t buffer, calibration;
    adc.getStatus(&buffer, &calibration);
    if (buffer != ADC_BUFFER || calibration != ADC_CALIBRATION) {
        return 1;
    }

    return Serial.available() && Serial.read() == RESET_WORD;
}

void initADC() {
    adc.begin();
    adc.reset();

    adc.setGain(GAIN_RATE);
    adc.setSample(SAMPLE_RATE);
    adc.setStatus(ADC_BUFFER, ADC_CALIBRATION);
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    blinkLED(3, 50);

    initADC();
    blinkLED(5, 50);
}

void loop() {
    sensor_t sensor;

    // Get voltage data
    for (uint16_t i = 0; i < PACKET_SIZE; i++) {
        // Support runtime reset
        if (shouldReset()) {
            Serial.write(ACK_WORD, sizeof(ACK_WORD));
            Serial.flush();

            initADC();
            blinkLED(1, 50);
        }

        // Vertical geophone (EHZ)
        adc.getDifferential(ANALOG_INPUT_AIN1, ANALOG_INPUT_AIN2);
        sensor.EHZ[i] = getRawValue(
            adc.getDifferential(ANALOG_INPUT_AIN3, ANALOG_INPUT_AIN4));
        // East-West geophone (EHE)
        sensor.EHE[i] = getRawValue(
            adc.getDifferential(ANALOG_INPUT_AIN5, ANALOG_INPUT_AIN6));
        // North-South geophone (EHN)
        sensor.EHN[i] = getRawValue(
            adc.getDifferential(ANALOG_INPUT_AIN7, ANALOG_INPUT_AIN8));
    }

    // Get checksum
    sensor.Checksum[0] = getChecksum(sensor.EHZ, PACKET_SIZE);
    sensor.Checksum[1] = getChecksum(sensor.EHE, PACKET_SIZE);
    sensor.Checksum[2] = getChecksum(sensor.EHN, PACKET_SIZE);

    // Send syncing word
    Serial.write(SYNC_WORD, sizeof(SYNC_WORD));
    // Send struct data
    Serial.write((uint8_t*)&sensor, sizeof(sensor));
    Serial.flush();
}
