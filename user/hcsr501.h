/**
 * @file    hcsr501.h
 * @author  Miaow
 * @version 1.0.0
 * @date    2021/08/01
 * @brief
 *          This file provides the following functions:
 *              1. Initialization.
 *              2. PIR motion detection.
 * @note
 *          Minimum version of source file:
 *              1.0
 *
 *          Pin connection:
 *          ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 *          ©¦     PC4©À©¤©¤©¤©¤©¤©ÈOUT     ©¦
 *          ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *          STM32F407       HCSR501
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#if !defined(__HCSR_501_H)
#define __HCSR_501_H
#include "utils.h"

/** 
 * @defgroup HCSR501
 */

/** 
 * @defgroup HCSR501_gpio_define
 * @{
 */
#define HCSR501_GPIO_CLK       RCC_AHB1Periph_GPIOC
#define HCSR501_GPIO_PORT      GPIOC
#define HCSR501_GPIO_PIN       GPIO_Pin_4
/**
 * @}
 */


void HCSR501_Init(void);
FlagStatus HCSR501_GetStatus(void);

/**
 * @}
 */

#endif // __HCSR_501_H
