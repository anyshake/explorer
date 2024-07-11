#include "ads1262/utils.hpp"

void ads1262_init(const ads1262_ctl_pin_t* pin, uint8_t control_type) {
    mcu_utils_spi_init();
    mcu_utils_gpio_mode(MCU_UTILS_SPI_SS, MCU_UTILS_GPIO_MODE_OUTPUT);
    mcu_utils_gpio_mode(pin->drdy, MCU_UTILS_GPIO_MODE_INPUT);
    mcu_utils_gpio_mode(pin->rst, MCU_UTILS_GPIO_MODE_OUTPUT);
    mcu_utils_gpio_mode(pin->start, MCU_UTILS_GPIO_MODE_OUTPUT);
    if (control_type == ADS1262_INIT_CONTROL_TYPE_HARD) {
        mcu_utils_gpio_low(pin->start);
    } else if (control_type == ADS1262_INIT_CONTROL_TYPE_SOFT) {
        mcu_utils_gpio_high(pin->start);
    }
}

void ads1262_reset(const ads1262_ctl_pin_t* pin, uint8_t reset_type) {
    if (reset_type == ADS1262_RESET_RESET_TYPE_HARD) {
        mcu_utils_gpio_low(pin->rst);
        mcu_utils_delay_ms(50, false);
        mcu_utils_gpio_high(pin->rst);
    } else if (reset_type == ADS1262_RESET_RESET_TYPE_SOFT) {
        ads1262_write_cmd(pin, ADS1262_CMD_RESET, NULL, 1,
                          ADS1262_WRITE_CMD_WAIT_ENABLE);
    }
}

void ads1262_wait(const ads1262_ctl_pin_t* pin) {
    bool is_ready = false;
    for (uint16_t i = 0; i < UINT16_MAX; i++) {
        if (!mcu_utils_gpio_read(pin->drdy)) {
            is_ready = true;
            break;
        }
    }
    // Hard reset if PIN_ADS1262_DRDY is stuck high
    if (!is_ready) {
        ads1262_reset(pin, ADS1262_RESET_RESET_TYPE_HARD);
    }
}

void ads1262_read_reg(uint8_t reg, uint8_t* rx_data) {
    mcu_utils_gpio_low(MCU_UTILS_SPI_SS);
    mcu_utils_spi_write(ADS1262_CMD_RREG | reg);
    mcu_utils_spi_write(0);  // Read only 1 register
    *rx_data = mcu_utils_spi_write(0xFF);
    mcu_utils_gpio_high(MCU_UTILS_SPI_SS);
}

void ads1262_write_reg(uint8_t reg, uint8_t* rx_data) {
    mcu_utils_gpio_low(MCU_UTILS_SPI_SS);
    mcu_utils_spi_write(ADS1262_CMD_WREG | reg);
    mcu_utils_spi_write(0);  // Write only 1 register
    mcu_utils_spi_write(*rx_data);
    mcu_utils_gpio_high(MCU_UTILS_SPI_SS);
}

void ads1262_write_cmd(const ads1262_ctl_pin_t* pin,
                       uint8_t cmd,
                       uint8_t* rx_data,
                       uint8_t rx_len,
                       uint8_t wait) {
    if (wait == ADS1262_WRITE_CMD_WAIT_ENABLE) {
        ads1262_wait(pin);
    }
    mcu_utils_gpio_low(MCU_UTILS_SPI_SS);
    mcu_utils_spi_write(cmd);
    for (uint8_t i = 0; i < rx_len; i++) {
        uint8_t rx_byte = mcu_utils_spi_write(0xFF);
        if (rx_data != NULL) {
            rx_data[i] = rx_byte;
        }
    }
    mcu_utils_gpio_high(MCU_UTILS_SPI_SS);
}
