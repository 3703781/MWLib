/**
 * @file    si7021.h
 * @author  Miaow
 * @version 0.1.0
 * @date    2019/11/16
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of si7021:
 *              1. Initialization
 *              2. Measurement
 * @note
 *          Minimum version of header file:
 *              0.1.0
 *          Pin connection:
 *               ©°©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©´
 *               ©¦    PB8©À©¤©¤©¤©¤©¤©ÈSCL    ©¦
 *               ©¦    PB9©À©¤©¤©¤©¤©¤©ÈSDA    ©¦
 *               ©¸©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¼
 *               STM32F407      SI7021
 *  
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
#ifndef __SI7021_H
#define __SI7021_H

#include "utils.h"

/** 
 * @defgroup SI7021
 * @brief SI7021 Modules.
 * @{
 */
/** 
 * @defgroup SI7021_gpio_define
 * @{
 */
#define SI7021_SCL_PORT         GPIOB
#define SI7021_SCL_PIN          GPIO_Pin_8
#define SI7021_SCL_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define SI7021_SDA_PORT         GPIOB
#define SI7021_SDA_PIN          GPIO_Pin_9
#define SI7021_SDA_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define SI7021_In()             SI7021_SDA_PORT->MODER &= ~0x000C0000;SI7021_SDA_PORT->MODER |= 0x00000000
#define SI7021_Out()            SI7021_SDA_PORT->MODER &= ~0x000C0000;SI7021_SDA_PORT->MODER |= 0x00040000
/**
 * @}
 */
 
void SI7021_Init(void);
uint8_t SI7021_GetTempature(float* tempature);
uint8_t SI7021_GetHumidity(uint8_t* humidity);
/**
 * @}
 */
 
#endif
