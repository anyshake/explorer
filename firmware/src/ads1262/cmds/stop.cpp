#include "ads1262/cmds/stop.h"

void ads1262_cmd_stop(ads1262_ctl_pin_t pin) {
    ads1262_write_cmd(pin, ADS1262_CMD_STOP1, NULL, 0, ADS1262_WRITE_CMD_WAIT_DISABLE);
}
