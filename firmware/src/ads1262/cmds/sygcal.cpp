#include "ads1262/cmds/sygcal.hpp"

void adc_cmd_sygcal() {
    adc_write_cmd(ADC_CMD_SYGCAL1, NULL, 1, ADC_WRITE_CMD_WAIT_DISABLE);
}
