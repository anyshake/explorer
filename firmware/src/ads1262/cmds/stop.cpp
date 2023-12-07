#include "ads1262/cmds/stop.hpp"

void adc_cmd_stop() {
    adc_write_cmd(ADC_CMD_STOP1, NULL, 0, ADC_WRITE_CMD_WAIT_DISABLE);
}
