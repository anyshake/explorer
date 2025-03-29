#include "ads1262/regs/id.h"

void ads1262_reg_get_id(ads1262_reg_id_t* id) {
    uint8_t id_data = 0;
    ads1262_read_reg(ADS1262_REG_ID, &id_data);
    id->dev_id = (id_data >> 5) & 0x07;
    id->rev_id = id_data & 0x1F;
}
