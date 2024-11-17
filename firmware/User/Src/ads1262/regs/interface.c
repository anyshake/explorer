#include "User/Inc/ads1262/regs/interface.h"

void ads1262_reg_set_interface(ads1262_reg_interface_t* interface) {
    uint8_t interface_data = __ADS1262_INTERFACE_DEFAULT_VALUE;
    interface_data &= ~(0x01 << 3);
    interface_data |= (interface->timeout & 0x01) << 3;
    interface_data &= ~(0x01 << 2);
    interface_data |= (interface->status & 0x01) << 2;
    interface_data &= ~0x03;
    interface_data |= interface->crc & 0x03;
    ads1262_write_reg(ADS1262_REG_INTERFACE, &interface_data);
}

void ads1262_reg_get_interface(ads1262_reg_interface_t* interface) {
    uint8_t interface_data = 0;
    ads1262_read_reg(ADS1262_REG_INTERFACE, &interface_data);
    interface->timeout = (interface_data & 0x08) >> 3;
    interface->status = (interface_data & 0x04) >> 2;
    interface->crc = interface_data & 0x03;
}
