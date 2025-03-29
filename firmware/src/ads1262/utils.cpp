#include "ads1262/utils.h"

void ads1262_reset(ads1262_ctl_pin_t pin, uint8_t reset_type, bool is_rtos) {
    if (reset_type == ADS1262_RESET_RESET_TYPE_HARD) {
        mcu_utils_gpio_low(pin.rst);
        mcu_utils_delay_ms(100, is_rtos);
        mcu_utils_gpio_high(pin.rst);
    } else if (reset_type == ADS1262_RESET_RESET_TYPE_SOFT) {
        ads1262_write_cmd(pin, ADS1262_CMD_RESET, NULL, 1, ADS1262_WRITE_CMD_WAIT_ENABLE);
    }
    mcu_utils_delay_ms(100, is_rtos);
}

void ads1262_init(ads1262_ctl_pin_t pin, uint8_t control_type) {
    mcu_utils_gpio_mode(pin.drdy, MCU_UTILS_GPIO_MODE_INPUT);
    mcu_utils_gpio_mode(pin.rst, MCU_UTILS_GPIO_MODE_OUTPUT);
    mcu_utils_gpio_mode(pin.start, MCU_UTILS_GPIO_MODE_OUTPUT);

    if (control_type == ADS1262_INIT_CONTROL_TYPE_HARD) {
        mcu_utils_gpio_low(pin.start);
    } else if (control_type == ADS1262_INIT_CONTROL_TYPE_SOFT) {
        mcu_utils_gpio_high(pin.start);
    }
}

void ads1262_wait(ads1262_ctl_pin_t pin) {
    while (mcu_utils_gpio_read(pin.drdy)) {
        ;
    }
}

void ads1262_read_reg(uint8_t reg, uint8_t* rx_data) {
    mcu_utils_gpio_low(SPI_SS);
    mcu_utils_spi_transfer(ADS1262_CMD_RREG | reg);
    mcu_utils_spi_transfer(0);  // Read only 1 register
    *rx_data = mcu_utils_spi_transfer(0xFF);
    mcu_utils_gpio_high(SPI_SS);
}

void ads1262_write_reg(uint8_t reg, uint8_t* tx_data) {
    mcu_utils_gpio_low(SPI_SS);
    mcu_utils_spi_transfer(ADS1262_CMD_WREG | reg);
    mcu_utils_spi_transfer(0);  // Write only 1 register
    mcu_utils_spi_transfer(*tx_data);
    mcu_utils_gpio_high(SPI_SS);
}

void ads1262_write_cmd(ads1262_ctl_pin_t pin,
                       uint8_t cmd,
                       uint8_t* rx_data,
                       uint8_t rx_len,
                       uint8_t wait) {
    if (wait == ADS1262_WRITE_CMD_WAIT_ENABLE) {
        ads1262_wait(pin);
    }
    mcu_utils_gpio_low(SPI_SS);
    mcu_utils_spi_transfer(cmd);
    for (uint8_t i = 0; i < rx_len; i++) {
        uint8_t rx_byte = mcu_utils_spi_transfer(0xFF);
        if (rx_data != NULL) {
            rx_data[i] = rx_byte;
        }
    }
    mcu_utils_gpio_high(SPI_SS);
}
