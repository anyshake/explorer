#include "ads1262/cmds/start.hpp"

void adc_cmd_start() {
    adc_write_cmd(ADC_CMD_START1, NULL, 0, ADC_WRITE_CMD_WAIT_DISABLE);
}
