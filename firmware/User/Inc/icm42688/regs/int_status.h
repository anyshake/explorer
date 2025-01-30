#ifndef __ICM42688_REGS_INT_STATUS_H
#define __ICM42688_REGS_INT_STATUS_H

#include <stdint.h>

#include "User/Inc/icm42688/reg_map.h"
#include "User/Inc/icm42688/utils.h"

typedef struct {
    // This bit automatically sets to 1 when a UI FSYNC interrupt is generated
    // The bit clears to 0 after the register has been read
    uint8_t ui_fsync_int;
    // This bit automatically sets to 1 when a PLL Ready interrupt is generated
    // The bit clears to 0 after the register has been read
    uint8_t pll_rdy_int;
    // This bit automatically sets to 1 when software reset is complete
    // The bit clears to 0 after the register has been read
    uint8_t reset_done_int;
    // This bit automatically sets to 1 when a Data Ready interrupt is generated
    // The bit clears to 0 after the register has been read
    uint8_t data_rdy_int;
    // This bit automatically sets to 1 when the FIFO buffer reaches the threshold
    // The bit clears to 0 after the register has been read
    uint8_t fifo_ths_int;
    // This bit automatically sets to 1 when the FIFO buffer is full
    // The bit clears to 0 after the register has been read
    uint8_t fifo_full_int;
    // This bit automatically sets to 1 when an AGC Ready interrupt is generated
    // The bit clears to 0 after the register has been read
    uint8_t agc_rdy_int;
} icm42688_reg_int_status_t;

void icm42688_reg_get_int_status(icm42688_reg_int_status_t* int_status);

#endif
