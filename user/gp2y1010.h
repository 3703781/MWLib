/**
 * @file    gp2y1010.h
 * @author  Miaow
 * @version 0.1.0
 * @date    2021/08/30
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of GP2Y1010:
 *              1. Initialization
 *              2. Measurement
 * @note
 *          Minimum version of source file:
 *              0.1.0
 *          Pin connection:
 *          ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 *          ©¦     PA7©À©¤©¤©¤©¤©¤©ÈVO      ©¦
 *          ©¦     PC5©À©¤©¤©¤©¤©¤©ÈLED     ©¦
 *          ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *          STM32F407       GP2Y1010
 *          
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#if !defined(__GP2Y1010_H)
#define __GP2Y1010_H

#include "utils.h"

/** 
 * @defgroup GP2Y1010
 */

/** 
 * @defgroup GP2Y1010_gpio_define
 * @{
 */
#define GP2Y1010_GPIO_ADC_PORT GPIOA
#define GP2Y1010_GPIO_ADC_PIN GPIO_Pin_7
#define GP2Y1010_GPIO_ADC_CLK RCC_AHB1Periph_GPIOA

#define GP2Y1010_GPIO_LED_PORT GPIOC
#define GP2Y1010_GPIO_LED_PIN GPIO_Pin_5
#define GP2Y1010_GPIO_LED_CLK RCC_AHB1Periph_GPIOC
/**
 * @}
 */

/** 
 * @defgroup GP2Y1010_adc_define
 * @{
 */
#define GP2Y1010_ADC ADC2
#define GP2Y1010_ADC_CHANNEL ADC_Channel_7
#define GP2Y1010_ADC_CLK RCC_APB2Periph_ADC2
/**
 * @}
 */


void GP2Y1010_Init(void);
float GP2Y1010_Get(void);
/**
 * @}
 */

#endif // __GP2Y1010_H
