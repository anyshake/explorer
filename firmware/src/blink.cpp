#include "blink.hpp"

void blinkLED(uint8_t times, uint16_t interval) {
    pinMode(LED_BUILTIN, OUTPUT);

    for (uint8_t i = 0; i < times; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(interval);
        digitalWrite(LED_BUILTIN, LOW);
        delay(interval);
    }
}
