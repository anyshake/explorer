/*!
    \file  gd32f303c_start.h
    \brief definitions for GD32f303C_START's leds, keys and COM ports hardware resources

    \version 2021-03-23, V2.0.0, demo for GD32F30x
*/

/*
    Copyright (c) 2021, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef GD32F303C_START_H
#define GD32F303C_START_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f30x.h"
     
/* exported types */
typedef enum 
{
    LED1,
    LED2
} led_typedef_enum;

typedef enum 
{
    KEY_USER
} key_typedef_enum;

typedef enum 
{
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
} keymode_typedef_enum;

/* eval board low layer led */
#define LEDn                             2U

#define LED1_PIN                         GPIO_PIN_14
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK                    RCU_GPIOB

#define LED2_PIN                         GPIO_PIN_15
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK                    RCU_GPIOB

#define KEYn                             1U

/* user push-button */
#define USER_KEY_PIN                     GPIO_PIN_0
#define USER_KEY_GPIO_PORT               GPIOA
#define USER_KEY_GPIO_CLK                RCU_GPIOA
#define USER_KEY_EXTI_LINE               EXTI_0
#define USER_KEY_EXTI_PORT_SOURCE        GPIO_PORT_SOURCE_GPIOA
#define USER_KEY_EXTI_PIN_SOURCE         GPIO_PIN_SOURCE_0
#define USER_KEY_EXTI_IRQn               EXTI0_IRQn

/* function declarations */
/* configure led GPIO */
void gd_eval_led_init(led_typedef_enum lednum);
/* turn on selected led */
void gd_eval_led_on(led_typedef_enum lednum);
/* turn off selected led */
void gd_eval_led_off(led_typedef_enum lednum);
/* toggle the selected led */
void gd_eval_led_toggle(led_typedef_enum lednum);
/* configure key */
void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode);
/* return the selected key state */
uint8_t gd_eval_key_state_get(key_typedef_enum key);

#ifdef cplusplus
}
#endif

#endif /* GD32F303C_START_H */
