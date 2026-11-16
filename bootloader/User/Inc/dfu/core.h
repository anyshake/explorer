#ifndef __USER_DFU_CORE_H
#define __USER_DFU_CORE_H

#include <stdbool.h>
#include <stdint.h>

#include "User/Inc/boot.h"
#include "User/Inc/dfu/crc.h"
#include "User/Inc/dfu/flash.h"
#include "User/Inc/dfu/link.h"
#include "User/Inc/dfu/protocol.h"

#define DFU_LISTEN_WINDOW_MS 500U
#define DFU_SESSION_TIMEOUT_MS 60000U

void dfu_run(void);

#endif
