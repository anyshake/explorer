#ifndef __SETTINGS_HPP
#define __SETTINGS_HPP

#include <SPI.h>

#define MCU_SPI_CLKDIV SPI_CLOCK_DIV2
#define MCU_SPI_ORDER MSBFIRST
#define MCU_SPI_MODE SPI_MODE2

#define PIN_ADC_START 15
#define PIN_MCU_STATE 2
#define PIN_ADC_DRDY 4
#define PIN_ADC_RST 5
#define PIN_ADC_SS 16

#define PACKET_SIZE 4
#define RESET_WORD 0x61

const uint8_t ACK_WORDS[] = {
    0xFC,
    0x2B,
};
const uint8_t SYNC_WORDS[] = {
    0xFC,
    0x1B,
};

#endif
