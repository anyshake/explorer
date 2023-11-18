// #define DIFFERENTIAL 0

// #include <Arduino.h>

// #include "ads1256/adc.hpp"
// #include "ads1256/channel.hpp"
// #include "ads1256/gain.hpp"
// #include "ads1256/sample.hpp"

// #include "blink.hpp"
// #include "config.hpp"

// ADS1256 adc(SS, PA3, PA2);

// uint8_t isReset() {
//     uint8_t gain = adc.getGain();
//     uint8_t sample = adc.getSample();
//     if (gain != GAIN_RATE || sample != SAMPLE_RATE) {
//         return 1;
//     }

//     return Serial.available() && Serial.read() == RESET_WORD;
// }

// void initADC() {
//     adc.begin();
//     adc.reset();

//     adc.setGain(GAIN_RATE);
//     adc.setSample(SAMPLE_RATE);
//     adc.setStatus(DISABLE, DISABLE);
// }

// void setup() {
//     Serial.begin(SERIAL_BAUD);
//     blinkLED(3, 50);

//     initADC();
//     blinkLED(5, 50);
// }

// void loop() {
//     if (isReset()) {
//         initADC();
//         blinkLED(1, 100);

//         Serial.write(ACK_WORD, sizeof(ACK_WORD));
//         Serial.flush();
//     }

// #if DIFFERENTIAL
//     for (uint8_t i = 1; i <= 4; i++) {
//         int32_t raw;
//         switch (i) {
//             case 1:
//                 raw = adc.getDifferential(ANALOG_INPUT_AIN1, ANALOG_INPUT_AIN2);
//                 break;
//             case 2:
//                 raw = adc.getDifferential(ANALOG_INPUT_AIN3, ANALOG_INPUT_AIN4);
//                 break;
//             case 3:
//                 raw = adc.getDifferential(ANALOG_INPUT_AIN5, ANALOG_INPUT_AIN6);
//                 break;
//             case 4:
//                 raw = adc.getDifferential(ANALOG_INPUT_AIN7, ANALOG_INPUT_AIN8);
//                 break;
//         }

//         double voltage = adc.getVoltage(raw);
//         Serial.print("CH");
//         Serial.print(i);
//         Serial.print(": ");
//         Serial.print(voltage, 10);
//         Serial.print("\t");
//     }
// #else
//     for (uint8_t i = 1; i <= 8; i++) {
//         int32_t raw;
//         switch (i) {
//             case 1:
//                 raw =
//                     adc.getDifferential(ANALOG_INPUT_AIN1, ANALOG_INPUT_AINCOM);
//                 break;
//             case 2:
//                 raw =
//                     adc.getDifferential(ANALOG_INPUT_AIN2, ANALOG_INPUT_AINCOM);
//                 break;
//             case 3:
//                 raw =
//                     adc.getDifferential(ANALOG_INPUT_AIN3, ANALOG_INPUT_AINCOM);
//                 break;
//             case 4:
//                 raw =
//                     adc.getDifferential(ANALOG_INPUT_AIN4, ANALOG_INPUT_AINCOM);
//                 break;
//             case 5:
//                 raw =
//                     adc.getDifferential(ANALOG_INPUT_AIN5, ANALOG_INPUT_AINCOM);
//                 break;
//             case 6:
//                 raw =
//                     adc.getDifferential(ANALOG_INPUT_AIN6, ANALOG_INPUT_AINCOM);
//                 break;
//             case 7:
//                 raw =
//                     adc.getDifferential(ANALOG_INPUT_AIN7, ANALOG_INPUT_AINCOM);
//                 break;
//             case 8:
//                 raw =
//                     adc.getDifferential(ANALOG_INPUT_AIN8, ANALOG_INPUT_AINCOM);
//                 break;
//         }

//         double voltage = adc.getVoltage(raw);
//         Serial.print("CH");
//         Serial.print(i);
//         Serial.print(": ");
//         Serial.print(voltage, 10);
//         Serial.print("\t");
//     }
// #endif

//     Serial.println();
//     Serial.flush();
// }
