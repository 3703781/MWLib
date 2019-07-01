/**
 * @file    drv8825.h
 * @author  Miaow
 * @version 0.1.0
 * @date    2018/09/24
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of DRV8825:
 *              1. Initialization and Configuration
 *              2. Motor operation
 * @note     
 *           Minimum version of source file:
 *              0.1.0
 *
 *           Steps to modify XIN1, XIN2, STBY:
 *              1.modify GPIO port, pin, clk
 *
 *           Steps to modify PWMx where x can be (A...D):
 *              1.modify GPIO port, pin, clk, pinsource, af
 *              2.modify definitions in @ref DRV8825_pwm_define
 *                to choose timer and channel corresponding to GPIO
 *              3.modify definitions in @ref DRV8825_pwm_define
 *                to select slave and master
 *
 *           Pin connection:
 *             ©°©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©´
 *             ©¦ STP©À©¤©¤©¤©¤©¤©ÈPA2  ©¦
 *             ©¦ DIR©À©¤©¤©¤©¤©¤©ÈPC5  ©¦
 *             ©¸©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¼
 *             DRV8825    STM32F407
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781/mystm32f4-devices-lib
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#ifndef __DRV8825_H
#define __DRV8825_H

#include "stm32f4xx.h"

/** 
 * @defgroup DRV8825
 * @brief DRV8825 driver modules
 * @{
 */

/** 
 * @defgroup DRV8825_gpio_define
 * @{
 */
 
//GPIO
#define DRV8825_STP_GPIO_AF                 GPIO_AF_TIM9
#define DRV8825_STP_GPIO_CLK                RCC_AHB1Periph_GPIOA
#define DRV8825_STP_PORT                    GPIOA
#define DRV8825_STP_PIN                     GPIO_Pin_2
#define DRV8825_STP_GPIO_PINSOURCE          GPIO_PinSource2

#define DRV8825_DIR_GPIO_CLK                RCC_AHB1Periph_GPIOC
#define DRV8825_DIR_PORT                    GPIOC
#define DRV8825_DIR_PIN                     GPIO_Pin_5

#define DRV8825_GPIO_ALL_CLK                (DRV8825_STP_GPIO_CLK | DRV8825_DIR_GPIO_CLK)

/**
 * @}
 */
 
 
/** 
 * @defgroup DRV8825_pwm_define
 * @{
 */
//timer for pwm - slave
#define DRV8825_STP_TIM_CLK_FUNC            RCC_APB2PeriphClockCmd
#define DRV8825_STP_TIM_CLK                 RCC_APB2Periph_TIM9
#define DRV8825_STP_TIM                     TIM9
//timer oc
#define DRV8825_STP_TIM_OC_FUNC             TIM_OC1Init
#define DRV8825_STP_TIM_OC_PRELOAD_FUNC     TIM_OC1PreloadConfig
#define DRV8825_STP_TIM_CHANNEL             TIM_Channel_1
//
#define DRV8825_STP_TIM_TRIG_SOURCE         TIM_TS_ITR2


#define DRV8825_CTRL_TIM_CLK_FUNC           RCC_APB2PeriphClockCmd
#define DRV8825_CTRL_TIM_CLK                RCC_APB2Periph_TIM10
#define DRV8825_CTRL_TIM                    TIM10

#define DRV8825_CTRL_TIM_OC_FUNC            TIM_OC1Init
#define DRV8825_CTRL_TIM_OC_PRELOAD_FUNC    TIM_OC1PreloadConfig
#define DRV8825_CTRL_TIM_CHANNEL            TIM_Channel_1
#define DRV8825_CTRL_TIM_TRGO_SOURCE        TIM_TRGOSource_OC1Ref
/**
 * @}
 */


void DRV8825_Init(void);
extern inline void DRV8825_Stop(void);
void DRV8825_Run(float speed, float angle);

/**
 * @}
 */ 
#endif
