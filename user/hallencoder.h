/**
 * @file    hallencoder.h
 * @author  Miaow
 * @version 0.2.0
 * @date    2018/08/08
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of hall encoders:
 *              1. Initialization
 *              2. Get values for speed measurement
 * @note
 *          Minimum version of source file:
 *              0.2.0
 *          Pin connection:
 *             ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 *             ©¦  PHASEA©À©¤©¤©¤©¤©¤©ÈPA7    PD13©À©¤©¤©¤©¤©¤©ÈPHASEA  ©¦
 *             ©¦  PHASEB©À©¤©¤©¤©¤©¤©ÈPB4    PD12©À©¤©¤©¤©¤©¤©ÈPHASEB  ©¦
 *             ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *              encoder_1       STM32F407       encoder_2
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781/mystm32f4-devices-lib
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#ifndef __HALLENCODER_H
#define __HALLENCODER_H

#include "stm32f4xx.h"
#include "utils.h"

/** 
 * @defgroup HALLENCODER
 * @brief HALLENCODER driver modules
 * @{
 */

/** 
 * @defgroup HALLENCODER_gpio_define
 * @{
 */
//GPIO for hall encoder A
#define HALLENCODER_A_GPIO_AF               GPIO_AF_TIM3
#define HALLENCODER_A_PHASEA_PORT           GPIOA
#define HALLENCODER_A_PHASEA_PIN            GPIO_Pin_7
#define HALLENCODER_A_PHASEA_GPIO_CLK       RCC_AHB1Periph_GPIOA
#define HALLENCODER_A_PHASEA_GPIO_PINSOURCE GPIO_PinSource7
#define HALLENCODER_A_PHASEB_PORT           GPIOB
#define HALLENCODER_A_PHASEB_PIN            GPIO_Pin_4
#define HALLENCODER_A_PHASEB_GPIO_CLK       RCC_AHB1Periph_GPIOB
#define HALLENCODER_A_PHASEB_GPIO_PINSOURCE GPIO_PinSource4
#define HALLENCODER_A_GPIO_CLK_ALL          (HALLENCODER_A_PHASEA_GPIO_CLK | HALLENCODER_A_PHASEB_GPIO_CLK)

//GPIO for hall encoder B
#define HALLENCODER_B_GPIO_AF               GPIO_AF_TIM4
#define HALLENCODER_B_PHASEA_PORT           GPIOD
#define HALLENCODER_B_PHASEA_PIN            GPIO_Pin_13
#define HALLENCODER_B_PHASEA_GPIO_CLK       RCC_AHB1Periph_GPIOD
#define HALLENCODER_B_PHASEA_GPIO_PINSOURCE GPIO_PinSource13
#define HALLENCODER_B_PHASEB_PORT           GPIOD
#define HALLENCODER_B_PHASEB_PIN            GPIO_Pin_12
#define HALLENCODER_B_PHASEB_GPIO_CLK       RCC_AHB1Periph_GPIOD
#define HALLENCODER_B_PHASEB_GPIO_PINSOURCE GPIO_PinSource12
#define HALLENCODER_B_GPIO_CLK_ALL          (HALLENCODER_B_PHASEA_GPIO_CLK | HALLENCODER_B_PHASEB_GPIO_CLK)
/**
 * @}
 */


/** 
 * @defgroup HALLENCODER_timer_define
 * @{
 */
#define HALLENCODER_B_TIM                   TIM4
#define HALLENCODER_B_TIM_CLK               RCC_APB1Periph_TIM4
#define HALLENCODER_A_TIM                   TIM3
#define HALLENCODER_A_TIM_CLK               RCC_APB1Periph_TIM3
/**
 * @}
 */


/** 
 * @defgroup HALLENCODER_hallencoder_select
 * @{
 */
#define HALLENCODER_A                       1
#define HALLENCODER_B                       2
/**
 * @}
 */

void HALLENCODER_Init(void);
int32_t HALLENCODER_ReadDeltaValue(uint8_t hallEncoderX);
/**
 * @}
 */

#endif
