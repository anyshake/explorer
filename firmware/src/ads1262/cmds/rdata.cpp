#include "ads1262/cmds/rdata.h"

void ads1262_cmd_rdata(ads1262_ctl_pin_t pin,
                       ads1262_cmd_rdata_t* rdata,
                       uint8_t control_type) {
    uint8_t rx_data[6] = {0};
    if (control_type == ADS1262_INIT_CONTROL_TYPE_SOFT) {
        ads1262_write_cmd(pin, ADS1262_CMD_START1, NULL, 0,
                          ADS1262_WRITE_CMD_WAIT_DISABLE);
    } else {
        mcu_utils_gpio_high(pin.start);
    }
    ads1262_write_cmd(pin, ADS1262_CMD_RDATA1, rx_data, 6,
                      ADS1262_WRITE_CMD_WAIT_ENABLE);
    if (control_type == ADS1262_INIT_CONTROL_TYPE_HARD) {
        mcu_utils_gpio_low(pin.start);
    }
    rdata->status = rx_data[0];
    rdata->data =
        (int32_t)(((uint32_t)(rx_data[1] << 24)) |
                  ((uint32_t)(rx_data[2] << 16)) |
                  ((uint32_t)(rx_data[3] << 8)) | (uint32_t)rx_data[4]);
    rdata->crc = rx_data[5];
}
