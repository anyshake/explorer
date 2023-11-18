#pragma once

#include <SPI.h>
#include <stdint.h>

#include "ads1256/channel.hpp"
#include "ads1256/gain.hpp"
#include "ads1256/mode.hpp"
#include "ads1256/sample.hpp"

#define VREF 2.5
#define SPEED 1920000

#define ENABLE 1
#define DISABLE 0

class ADS1256 {
   private:
    uint8_t _cs_pin;
    uint8_t _drdy_pin;
    uint8_t _rst_pin;
    uint8_t readRegister(uint8_t reg);
    void writeRegister(uint8_t reg, uint8_t value);

   public:
    ADS1256(uint8_t cs_pin, uint8_t drdy_pin, uint8_t rst_pin);
    void setMode(AdcMode mode);
    void setStatus(uint8_t buffer, uint8_t calibration);
    void getStatus(uint8_t *buffer, uint8_t *calibration);
    void setGain(GainRate gain);
    GainRate getGain(void);
    void setSample(SampleRateSPS rate);
    SampleRateSPS getSample(void);
    void setChannel(AnalogInputChannel channel);
    AnalogInputChannel getChannel(void);
    int32_t getSingleOneShot(void);
    int32_t getSingleContinuous(void);
    int32_t getDifferential(AnalogInputChannel positiveCh,
                            AnalogInputChannel negativeCh);
    double getVoltage(int32_t value);
    void begin(void);
    void reset(void);
    void wait(void);
};
