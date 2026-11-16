#include "Core/Inc/main.h"
#include "Core/Inc/crc.h"
#include "Core/Inc/gpio.h"
#include "Core/Inc/usart.h"

#include "User/Inc/boot.h"
#include "User/Inc/dfu/core.h"

int main(void) {
    boot_jump_if_requested();

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
    LL_GPIO_AF_Remap_SWJ_NOJTAG();

    SystemClock_Config();
    LL_SYSTICK_EnableIT();

    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_CRC_Init();
    dfu_link_init();

    dfu_run();

    while (1) {
        ;
    }
}
