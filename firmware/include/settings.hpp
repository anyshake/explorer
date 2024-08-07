#ifndef __SETTINGS_HPP
#define __SETTINGS_HPP

#include <SPI.h>

#define SAMPLE_RATE 100

#define MCU_SPI_CLK 19200000
#define MCU_SPI_ORDER MSBFIRST
#define MCU_SPI_MODE SPI_MODE2
#define MCU_UART_RATE 115200

#define PIN_ADC_START 15
#define PIN_MCU_STATE 2
#define PIN_ADC_DRDY 4
#define PIN_ADC_RST 5
#define PIN_ADC_SS 16

#define PACKET_SIZE 5

const uint8_t SYNC_WORDS[] = {0xFC, 0x1B};

#endif
