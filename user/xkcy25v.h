/**
 * @file    xkcy25v.c
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
#ifndef __XKCY25V_H
#define __XKCY25V_H


#include "stdint.h"

/** 
 * @defgroup XKCY25V
 * @brief XKCY25V modules
 * @{
 */

/** 
 * @defgroup XKCY25V_gpio_define
 * @{
 */
#define XKCY25V_1_INT_GPIO_CLK RCC_AHB1Periph_GPIOB
#define XKCY25V_1_INT_PIN GPIO_Pin_7
#define XKCY25V_1_INT_PORT GPIOB

#define XKCY25V_2_INT_GPIO_CLK RCC_AHB1Periph_GPIOB
#define XKCY25V_2_INT_PIN GPIO_Pin_6
#define XKCY25V_2_INT_PORT GPIOB

#define XKCY25V_1_GetState() (!!(XKCY25V_1_INT_PORT->IDR & XKCY25V_1_INT_PIN))
#define XKCY25V_2_GetState() (!!(XKCY25V_2_INT_PORT->IDR & XKCY25V_2_INT_PIN))
/**
 * @}
 */

/** 
 * @defgroup XKCY25V_exti_define
 * @{
 */
#define XKCY25V_1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOB
#define XKCY25V_1_EXTI_PIN_SOURCE EXTI_PinSource7
#define XKCY25V_1_EXTI_LINE EXTI_Line7

#define XKCY25V_2_EXTI_PORT_SOURCE EXTI_PortSourceGPIOB
#define XKCY25V_2_EXTI_PIN_SOURCE EXTI_PinSource6
#define XKCY25V_2_EXTI_LINE EXTI_Line6

#define XKCY25V_EXTI_IRQHANDLER Uncomment_EXTI9_5_IRQHandler_if_no_mpu6050_used  // EXTI9_5_IRQHandler

#define XKCY25V_NVIC_IRQCHANNEL EXTI9_5_IRQn 
/**
 * @}
 */

/** 
 * @defgroup XKCY25V_state_define
 * @{
 */
#define XKCY25V_STATE_NotTriggled 0
#define XKCY25V_STATE_Triggled !XKCY25V_NotTriggled
/**
 * @}
 */

void XKCY25V_Init(void (*irqHandler1)(uint8_t state), void (*irqHandler2)(uint8_t state));
void XKCY25V_BeginReceive(void);
void XKCY25V_StopReceive(void);

/**
 * @}
 */

#endif
