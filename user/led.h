/**
 * @file    led.h
 * @author  Miaow
 * @version 0.1.0
 * @date    2018/10/28
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of LEDs on board:
 *              1. Initialization.
 *              2. Turn on or off.
 * @note     
 *           Minimum version of source file:
 *              0.1.0
 *
 *           Pin connection on pcb:
 *             ©°©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©´
 *             ©¦LED2©À©¤©¤©¤©¤©¤©ÈPA6  ©¦
 *             ©¦LED3©À©¤©¤©¤©¤©¤©ÈPA7  ©¦
 *             ©¸©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¼
 *              LEDs     STM32F407
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
 
#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

/** 
 * @defgroup LED
 * @brief LED bsp functions
 * @{
 */

/** 
 * @defgroup LED_gpio_define
 * @{
 */

#define LED_2_GPIO                  GPIOA
#define LED_2_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define LED_2_PIN                   GPIO_Pin_6

#define LED_3_GPIO                  GPIOA
#define LED_3_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define LED_3_PIN                   GPIO_Pin_7
/** 
 * @}
 */
 
#define LED_2_ON()                  LED_2_GPIO->BSRRH = LED_2_PIN
#define LED_2_OFF()                 LED_2_GPIO->BSRRL = LED_2_PIN
#define LED_2_TOGGLE()              LED_2_GPIO->ODR ^= LED_2_PIN

#define LED_3_ON()                  LED_3_GPIO->BSRRH = LED_3_PIN
#define LED_3_OFF()                 LED_3_GPIO->BSRRL = LED_3_PIN
#define LED_3_TOGGLE()              LED_3_GPIO->ODR ^= LED_3_PIN


void LED_Init(void);

/** 
 * @}
 */
 
#endif
