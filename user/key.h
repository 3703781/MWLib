/**
 * @file    key.h
 * @author  Alientek, Miaow
 * @version 0.1.1
 * @date    2019/06/30
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of keys:
 *              1. Initialization
 *              2. Scan function
 * @note
 *          Minimum version of source file:
 *              0.1.1
 *          Pin connection:
 *                         ©°©¤©¤©¤©¤©¤©¤©¤©´
 *                KEY0©¤©¤©¤©¤©¤©ÈPE4    ©¦
 *                KEY1©¤©¤©¤©¤©¤©ÈPE3    ©¦
 *                K_UP©¤©¤©¤©¤©¤©ÈPA0    ©¦
 *                         ©¸©¤©¤©¤©¤©¤©¤©¤©¼
 *                        STM32F407  
 *  
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f4xx.h" 

/** 
 * @defgroup KEY
 * @brief Key Modules.
 * @{
 */

/** 
 * @defgroup KEY_definition
 * @note DO NOT modify the value
 * @{
 */
#define KEY_0 	              1
#define KEY_1	                2
#define KEY_UP	              3

#define KEY_PRESSED_DOWN      1
#define KEY_PRESSED_UP        2
/**
 * @}
 */

/** 
 * @defgroup KEY_gpio_define
 * @{
 */
#define KEY_0_GPIO_CLK          RCC_AHB1Periph_GPIOE
#define KEY_0_PORT              GPIOE
#define KEY_0_PIN               GPIO_Pin_4
#define KEY_0_PRESSED_STATUS    KEY_PRESSED_DOWN

#define KEY_1_GPIO_CLK          RCC_AHB1Periph_GPIOE
#define KEY_1_PORT              GPIOE
#define KEY_1_PIN               GPIO_Pin_3
#define KEY_1_PRESSED_STATUS    KEY_PRESSED_DOWN

#define KEY_UP_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define KEY_UP_PORT             GPIOA
#define KEY_UP_PIN              GPIO_Pin_0
#define KEY_UP_PRESSED_STATUS   KEY_PRESSED_UP
/**
 * @}
 */

void KEY_Init(void);
uint8_t KEY_Scan(void);

#endif
/**
 * @}
 */
