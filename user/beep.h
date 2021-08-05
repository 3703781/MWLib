/**
 * @file    beep.h
 * @author  Miaow
 * @version 1.0.0
 * @date    2021/08/02
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of LEDs on board:
 *              1. Initialization.
 *              2. Turn on or off.
 * @note     
 *           Minimum version of source file:
 *              1.0.0
 *
 *           Pin connection on pcb:
 *             ©°©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©´
 *             ©¦BEEP©À©¤©¤©¤©¤©¤©ÈPF8  ©¦
 *             ©¸©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¼
 *              BEEP     STM32F407
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
 
#ifndef __BEEP_H
#define __BEEP_H

#include "utils.h"

/** 
 * @defgroup BEEP
 * @brief BEEP bsp functions
 * @{
 */

/** 
 * @defgroup BEEP_gpio_define
 * @{
 */

#define BEEP_GPIO                  GPIOF
#define BEEP_GPIO_CLK              RCC_AHB1Periph_GPIOF
#define BEEP_PIN                   GPIO_Pin_8
/** 
 * @}
 */
 
#define BEEP_ON()                  BEEP_GPIO->BSRRL = BEEP_PIN
#define BEEP_OFF()                 BEEP_GPIO->BSRRH = BEEP_PIN
#define BEEP_TOGGLE()              BEEP_GPIO->ODR ^= BEEP_PIN


void BEEP_Init(void);

/** 
 * @}
 */
 
#endif
