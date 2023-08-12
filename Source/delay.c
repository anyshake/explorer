#include "delay.h"

uint16_t US_COUNT;

void DELAY_TIMER_IRQHandler() {
    TMR_CLR_FLAG();
    US_COUNT++;
}

void delay_init() {
    timer_parameter_struct tmr;

    rcu_periph_clock_enable(RCU_DELAY_TIMER);
    timer_deinit(DELAY_TIMER);

    tmr.prescaler         = DELAY_TIMER_PRESCALER;
    tmr.alignedmode       = TIMER_COUNTER_EDGE;
    tmr.counterdirection  = TIMER_COUNTER_UP;
    tmr.period            = DELAY_TIMER_PERIOD;
    tmr.clockdivision     = TIMER_CKDIV_DIV1;
    tmr.repetitioncounter = 0;
    timer_init(DELAY_TIMER, &tmr);

    timer_interrupt_enable(DELAY_TIMER,TIMER_INT_UP);
    nvic_irq_enable(DELAY_TIMER_IRQn, 2, 0);
    TMR_DISABLE();
}

void delayMicroseconds(uint16_t count) {
    TMR_SET_CNT(0);
    TMR_ENABLE();

    US_COUNT = 0;
    while (US_COUNT < count) {
        ;
    }

    TMR_DISABLE();
}
 
void delay(uint16_t count) {
    while (count--) {
        delayMicroseconds(1000);
    }          
}
