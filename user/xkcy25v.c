/**
 * @file    xkcy25v.h
 * @author  Miaow
 * @version 0.1.0
 * @date    2018/10/28
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of water level sensor:
 *              1. Initialization.
 *              2. Get water level status.
 * @note     
 *           Minimum version of source file:
 *              0.1.0
 *
 *           Pin connection on pcb:
 *             ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©´
 *             ©¦XKCY25V1©À©¤©¤©¤©¤©¤©ÈPB6   ©¦
 *             ©¦XKCY25V2©À©¤©¤©¤©¤©¤©ÈPB7   ©¦
 *             ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¼
 *              XKCY25V     STM32F407
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#include "xkcy25v.h"
#include "utils.h"


/** @addtogroup XKCY25V
  * @{
  */

void (*XKCY25V_1_IrqHandler)(uint8_t state);
void (*XKCY25V_2_IrqHandler)(uint8_t state);

void XKCY25V_Init(void (*irqHandler1)(uint8_t state), void (*irqHandler2)(uint8_t state))
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    //GPIO
    RCC_AHB1PeriphClockCmd(XKCY25V_1_INT_GPIO_CLK |XKCY25V_2_INT_GPIO_CLK , ENABLE);//???GPIO???
    GPIO_InitStructure.GPIO_Pin = XKCY25V_1_INT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(XKCY25V_1_INT_PORT, &GPIO_InitStructure);//?????
    GPIO_InitStructure.GPIO_Pin = XKCY25V_2_INT_PIN;
    GPIO_Init(XKCY25V_2_INT_PORT, &GPIO_InitStructure);//?????
    //SYSCFG
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(XKCY25V_1_EXTI_PORT_SOURCE, XKCY25V_1_EXTI_PIN_SOURCE);
    SYSCFG_EXTILineConfig(XKCY25V_2_EXTI_PORT_SOURCE, XKCY25V_2_EXTI_PIN_SOURCE);
    //EXTI
    EXTI_InitStructure.EXTI_Line = XKCY25V_1_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = XKCY25V_2_EXTI_LINE;
    EXTI_Init(&EXTI_InitStructure);
    //IRQ
    XKCY25V_1_IrqHandler = irqHandler1;
    XKCY25V_2_IrqHandler = irqHandler2;
}

void XKCY25V_BeginReceive()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //NVIC
    NVIC_InitStructure.NVIC_IRQChannel = XKCY25V_NVIC_IRQCHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}

void XKCY25V_StopReceive()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //NVIC
    NVIC_InitStructure.NVIC_IRQChannel = XKCY25V_NVIC_IRQCHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}

void XKCY25V_EXTI_IRQHANDLER()
{
    if((EXTI->PR & XKCY25V_1_EXTI_LINE) != (uint32_t)RESET)
    {
        XKCY25V_1_IrqHandler(!!(XKCY25V_1_INT_PORT->IDR & XKCY25V_1_INT_PIN));
        EXTI->PR = XKCY25V_1_EXTI_LINE;
    }
    if((EXTI->PR & XKCY25V_2_EXTI_LINE) != (uint32_t)RESET)
    {
        XKCY25V_2_IrqHandler(!!(XKCY25V_2_INT_PORT->IDR & XKCY25V_2_INT_PIN));
        EXTI->PR = XKCY25V_2_EXTI_LINE;
    }
}

/**
 * @}
 */

