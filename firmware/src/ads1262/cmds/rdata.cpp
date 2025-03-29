#include "ads1262/cmds/rdata.h"

void ads1262_cmd_rdata(ads1262_ctl_pin_t pin,
                       ads1262_cmd_rdata_t* rdata,
                       uint8_t control_type) {
    uint8_t rx_data[6] = {0};
    if (control_type == ADS1262_INIT_CONTROL_TYPE_SOFT) {
        ads1262_write_cmd(pin, ADS1262_CMD_START1, NULL, 0, ADS1262_WRITE_CMD_WAIT_DISABLE);
    } else {
        mcu_utils_gpio_high(pin.start);
    }
    ads1262_write_cmd(pin, ADS1262_CMD_RDATA1, rx_data, sizeof(rx_data), ADS1262_WRITE_CMD_WAIT_ENABLE);
    if (control_type == ADS1262_INIT_CONTROL_TYPE_HARD) {
        mcu_utils_gpio_low(pin.start);
    }
    rdata->status = rx_data[0];
    rdata->data = rx_data[1] << 24;
    rdata->data |= rx_data[2] << 16;
    rdata->data |= rx_data[3] << 8;
    rdata->data |= rx_data[4];
    rdata->crc = rx_data[5];
}

bool ads1262_cmd_rdata_is_valid(ads1262_cmd_rdata_t* rdata, uint8_t crc_mode) {
    if (crc_mode == ADS1262_REG_INTERFACE_CRC_DISABLED) {
        return true;
    }

    uint8_t* data = (uint8_t*)(&rdata->data);
    if (crc_mode == ADS1262_REG_INTERFACE_CRC_CHECKSUM) {
        uint16_t calc_checksum = 0x9B;  // Offset given in datasheet
        for (uint8_t i = 0; i < sizeof(rdata->data); i++) {
            calc_checksum += data[i];
        }

        return (uint8_t)(calc_checksum % 256) == rdata->crc;
    } else if (crc_mode == ADS1262_REG_INTERFACE_CRC_CRC) {
        uint8_t calc_crc = 0;
        for (uint8_t i = 0; i < sizeof(rdata->data); i++) {
            calc_crc ^= data[sizeof(rdata->data) - i - 1];
            // CRC-8-ATM algorithm
            for (uint8_t j = 0; j < 8; j++) {
                if (calc_crc & 0x80) {
                    calc_crc = (calc_crc << 1) ^ 0x07;
                } else {
                    calc_crc <<= 1;
                }
            }
        }

        return calc_crc == rdata->crc;
    }

    return false;
}
