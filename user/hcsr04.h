/**
 * @file    hcsr04.h
 * @author  Miaow
 * @version 0.1.0
 * @date    2018/12/07
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of GP2Y0E03, the active optical distance measurement sensor:
 *              1. Initialization
 *              2. Configurations
 *              3. Distance measurement
 * @note
 *          Minimum version of source file:
 *              0.1.0
 *          Pin connection:
 *          ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 *          ©¦    TRIG©À©¤©¤©¤©¤©¤©ÈPE5  PE2©À©¤©¤©¤©¤©¤©ÈTRIG    ©¦
 *          ©¦    ECHO©À©¤©¤©¤©¤©¤©ÈPE3  PE4©À©¤©¤©¤©¤©¤©ÈECHO    ©¦
 *          ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *           HCSR04_1      STM32F407       HCSR04_2
 *          
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
 
#ifndef __HCSR04_H
#define __HCSR04_H
#include "stdint.h"
#include "stm32f4xx.h"

/** 
 * @defgroup GP2Y0E03
 * @{
 */
 
/** 
 * @defgroup HCSR04_timer_define
 * @{
 */
#define HCSR04_TIMER_CLKFUN          RCC_APB2PeriphClockCmd
#define HCSR04_TIMER_CLK             RCC_APB2Periph_TIM1
#define HCSR04_TIMER                 TIM1
#define HCSR04_TIMER_MAX_PERIOD      UINT32_MAX
#define HCSR04_TIMER_IRQCHANNEL      TIM1_UP_TIM10_IRQn
/**
 * @}
 */
 
/** 
 * @defgroup HCSR04_gpio_define
 * @{
 */
#define HCSR04_1_ECHO_GPIO_CLK       RCC_AHB1Periph_GPIOE
#define HCSR04_1_ECHO_GPIO_PORT      GPIOE
#define HCSR04_1_ECHO_GPIO_PIN       GPIO_Pin_3

#define HCSR04_1_TRIG_GPIO_CLK       RCC_AHB1Periph_GPIOE
#define HCSR04_1_TRIG_GPIO_PORT      GPIOE
#define HCSR04_1_TRIG_GPIO_PIN       GPIO_Pin_5

#define HCSR04_2_ECHO_GPIO_CLK       RCC_AHB1Periph_GPIOE
#define HCSR04_2_ECHO_GPIO_PORT      GPIOE
#define HCSR04_2_ECHO_GPIO_PIN       GPIO_Pin_4

#define HCSR04_2_TRIG_GPIO_CLK       RCC_AHB1Periph_GPIOE
#define HCSR04_2_TRIG_GPIO_PORT      GPIOE
#define HCSR04_2_TRIG_GPIO_PIN       GPIO_Pin_2

#define HCSR04_ALL_GPIO_CLK          (HCSR04_1_ECHO_GPIO_CLK | HCSR04_1_TRIG_GPIO_CLK | HCSR04_2_ECHO_GPIO_CLK | HCSR04_2_TRIG_GPIO_CLK)
/**
 * @}
 */
 
/** 
 * @defgroup HCSR04_select
 * @{
 */
#define HCSR04_1                    0x01
#define HCSR04_2                    0x02
/**
 * @}
 */

/** 
 * @defgroup HCSR04_configuration
 * @{
 */
 
/**
 * @brief 
 */
#define HCSR04_MAX_DISTANCE         64.0f
/**
 * @}
 */
 
void HCSR04_Init(void);
float HCSR04_ReadOutDistance(uint8_t sensorX);

/**
 * @}
 */
 
#endif
