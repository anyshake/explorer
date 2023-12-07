#include "ads1262/cmds/sfocal.hpp"

void adc_cmd_sfocal() {
    adc_write_cmd(ADC_CMD_SFOCAL1, NULL, 1, ADC_WRITE_CMD_WAIT_DISABLE);
}
