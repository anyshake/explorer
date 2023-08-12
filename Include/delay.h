#ifndef APP_DELAY_H
#define APP_DELAY_H

#include "gd32f30x_timer.h"

#define DELAY_TIMER TIMER3
#define RCU_DELAY_TIMER RCU_TIMER3
#define DELAY_TIMER_IRQHandler TIMER3_IRQHandler
#define DELAY_TIMER_IRQn TIMER3_IRQn

#define DELAY_TIMER_PRESCALER 0
#define DELAY_TIMER_PERIOD (SystemCoreClock / 1000 / 1000) - 1

#define TMR_ENABLE() TIMER_CTL0(DELAY_TIMER) |= (uint32_t)TIMER_CTL0_CEN
#define TMR_DISABLE() TIMER_CTL0(DELAY_TIMER) &= ~(uint32_t)TIMER_CTL0_CEN
#define TMR_CLR_FLAG() TIMER_INTF(DELAY_TIMER) = ~(uint32_t)TIMER_INT_FLAG_UP
#define TMR_SET_CNT(X) TIMER_CNT(DELAY_TIMER) = (uint32_t)(X)

extern uint16_t us_count;

void delay_init(void);
void delay(uint16_t count);
void delayMicroseconds(uint16_t count);

#endif
