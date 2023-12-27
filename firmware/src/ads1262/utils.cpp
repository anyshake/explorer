#include "ads1262/utils.hpp"

void adc_init(uint8_t control_type) {
    spi_init();
    gpio_mode(PIN_ADC_SS, GPIO_MODE_OUTPUT);
    gpio_mode(PIN_ADC_DRDY, GPIO_MODE_INPUT);
    gpio_mode(PIN_ADC_RST, GPIO_MODE_OUTPUT);
    gpio_mode(PIN_ADC_START, GPIO_MODE_OUTPUT);
    if (control_type == ADC_INIT_CONTROL_TYPE_HARD) {
        gpio_low(PIN_ADC_START);
    } else if (control_type == ADC_INIT_CONTROL_TYPE_SOFT) {
        gpio_high(PIN_ADC_START);
    }
}

void adc_reset(uint8_t reset_type) {
    if (reset_type == ADC_RESET_RESET_TYPE_HARD) {
        gpio_low(PIN_ADC_RST);
        delay_ms(50);
        gpio_high(PIN_ADC_RST);
    } else if (reset_type == ADC_RESET_RESET_TYPE_SOFT) {
        adc_write_cmd(ADC_CMD_RESET, NULL, 1, ADC_WRITE_CMD_WAIT_ENABLE);
    }
}

void adc_wait() {
    boolean is_ready = false;
    for (uint16_t i = 0; i < UINT16_MAX; i++) {
        if (!gpio_read(PIN_ADC_DRDY)) {
            is_ready = true;
            break;
        }
        delay_us(5);
    }
    // Hard reset if PIN_ADC_DRDY is stuck high
    if (!is_ready) {
        adc_reset(ADC_RESET_RESET_TYPE_HARD);
    }
}

void adc_read_reg(uint8_t reg, uint8_t* rx_data) {
    gpio_low(PIN_ADC_SS);
    spi_write(ADC_CMD_RREG | reg);
    spi_write(0);  // Read only 1 register
    *rx_data = spi_write(0xFF);
    gpio_high(PIN_ADC_SS);
}

void adc_write_reg(uint8_t reg, uint8_t* rx_data) {
    gpio_low(PIN_ADC_SS);
    spi_write(ADC_CMD_WREG | reg);
    spi_write(0);  // Write only 1 register
    spi_write(*rx_data);
    gpio_high(PIN_ADC_SS);
}

void adc_write_cmd(uint8_t cmd,
                   uint8_t* rx_data,
                   uint8_t rx_len,
                   uint8_t wait) {
    if (wait == ADC_WRITE_CMD_WAIT_ENABLE) {
        adc_wait();
    }
    gpio_low(PIN_ADC_SS);
    spi_write(cmd);
    for (uint8_t i = 0; i < rx_len; i++) {
        uint8_t rx_byte = spi_write(0xFF);
        if (rx_data != NULL) {
            rx_data[i] = rx_byte;
        }
    }
    gpio_high(PIN_ADC_SS);
}
