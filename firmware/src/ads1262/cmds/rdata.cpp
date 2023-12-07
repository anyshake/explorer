#include "ads1262/cmds/rdata.hpp"

void adc_cmd_rdata(adc_cmd_rdata_t* rdata, uint8_t control_type) {
    uint8_t rx_data[6] = {0};
    if (control_type == ADC_INIT_CONTROL_TYPE_SOFT) {
        adc_write_cmd(ADC_CMD_START1, NULL, 0, ADC_WRITE_CMD_WAIT_DISABLE);
    } else {
        gpio_high(PIN_ADC_START);
    }
    adc_write_cmd(ADC_CMD_RDATA1, rx_data, 6, ADC_WRITE_CMD_WAIT_ENABLE);
    if (control_type == ADC_INIT_CONTROL_TYPE_HARD) {
        gpio_low(PIN_ADC_START);
    }
    rdata->status = rx_data[0];
    rdata->data =
        (int32_t)(((uint32_t)(rx_data[1] << 24)) |
                  ((uint32_t)(rx_data[2] << 16)) |
                  ((uint32_t)(rx_data[3] << 8)) | (uint32_t)rx_data[4]);
    rdata->crc = rx_data[5];
}
