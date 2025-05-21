#ifndef __PERIPHERAL_H
#define __PERIPHERAL_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/settings.h"

#include "User/Inc/gnss/utils.h"

#include "User/Inc/ssd1306/display.h"
#include "User/Inc/ssd1306/utils.h"

#include "User/Inc/eeprom/utils.h"

#include "User/Inc/ads1262/regs/interface.h"
#include "User/Inc/ads1262/regs/mode_0.h"
#include "User/Inc/ads1262/regs/mode_2.h"

#include "User/Inc/icm42688/lsb_per_g.h"
#include "User/Inc/icm42688/regs/accel_config0.h"
#include "User/Inc/icm42688/regs/accel_config_static2.h"
#include "User/Inc/icm42688/regs/gyro_accel_config0.h"
#include "User/Inc/icm42688/regs/pwr_mgmt0.h"
#include "User/Inc/icm42688/utils.h"

#include "User/Inc/lsm6ds3/lsb_per_g.h"
#include "User/Inc/lsm6ds3/regs/ctrl10_c.h"
#include "User/Inc/lsm6ds3/regs/ctrl1_xl.h"
#include "User/Inc/lsm6ds3/regs/ctrl3_c.h"
#include "User/Inc/lsm6ds3/regs/ctrl4_c.h"
#include "User/Inc/lsm6ds3/regs/ctrl8_xl.h"
#include "User/Inc/lsm6ds3/utils.h"

void peri_gnss_init(void);
void peri_screen_init(void);
void peri_eeprom_init(void);
uint16_t peri_imu_init(uint8_t sample_rate);
void peri_adc_init(uint8_t control_type, uint8_t sample_rate, bool channel_6d);

#endif
