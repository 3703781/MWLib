/**
 * @file    servo.h
 * @author  Miaow
 * @version 0.1.0
 * @date    2019/08/05
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of servos:
 *              1. Initialization
 *              2. Servo operation
 * @note
 *          Minimum version of source file:
 *              0.1.0
 *          Pin connection:           
 *                              ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 *                              ©¦ SERVO3 ©¦
 *                              ©¸©¤©¤©¤©Ð©¤©¤©¤©¤©¼
 *                             ©°©¤©¤©¤©¤©Ø©¤©¤©¤©¤©¤©´
 *              ©°©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©´   ©¦   PB10   ©¦   ©°©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©´
 *              ©¦  SERVO1  ©À©¤©¤©¤©ÈPA15   PB3©À©¤©¤©¤©È  SERVO2  ©¦
 *              ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼   ©¦   PB11   ©¦   ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *                    STM32F407©¸©¤©¤©¤©¤©Ð©¤©¤©¤©¤©¤©¼
 *                              ©°©¤©¤©¤©Ø©¤©¤©¤©¤©´
 *                              ©¦ SERVO4 ©¦
 *                              ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *             
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
#ifndef __SERVO_H
#define __SERVO_H

#include "stdint.h"

/** 
 * @defgroup SERVO
 * @brief SERVO driver modules
 * @{
 */

/** 
 * @defgroup SERVO_configuration
 * @{
 */
#define SERVO_NUMBER                  4 //!< Number of servo in use, this macro can be 1, 2, 3 and 4.
#define SERVO_1_PULSE_MS              0.65f, 2.44f //!< Pulse width (microsecond) of servo1 at 0 and 180¡ã.
#define SERVO_2_PULSE_MS              0.50f, 2.50f //!< Pulse width (microsecond) of servo2 at 0 and 180¡ã.
#define SERVO_3_PULSE_MS              0.50f, 2.50f //!< Pulse width (microsecond) of servo3 at 0 and 180¡ã.
#define SERVO_4_PULSE_MS              0.50f, 2.50f //!< Pulse width (microsecond) of servo4 at 0 and 180¡ã.
/**
 * @}
 */

/** 
 * @defgroup SERVO_gpio_define
 * @brief GPIO configuration of each servo.
 * @note GPIOs will be initialized according to the number
 *       of servo used, see @ref SERVO_configuration.
 * @{
 */
#define SERVO_1_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define SERVO_1_PORT                  GPIOA
#define SERVO_1_PIN                   GPIO_Pin_15
#define SERVO_1_GPIO_PINSOURCE        GPIO_PinSource15

#define SERVO_2_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define SERVO_2_PORT                  GPIOB
#define SERVO_2_PIN                   GPIO_Pin_3
#define SERVO_2_GPIO_PINSOURCE        GPIO_PinSource3

#define SERVO_3_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define SERVO_3_PORT                  GPIOB
#define SERVO_3_PIN                   GPIO_Pin_10
#define SERVO_3_GPIO_PINSOURCE        GPIO_PinSource10

#define SERVO_4_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define SERVO_4_PORT                  GPIOB
#define SERVO_4_PIN                   GPIO_Pin_11
#define SERVO_4_GPIO_PINSOURCE        GPIO_PinSource11

#define SERVO_GPIO_AF                 GPIO_AF_TIM2
#define SERVO_ALL_GPIO_CLK            (SERVO_1_GPIO_CLK | SERVO_2_GPIO_CLK | SERVO_3_GPIO_CLK | SERVO_4_GPIO_CLK)
/**
 * @}
 */
 
/**
 * @defgroup SERVO_pwm_define
 * @brief Timer and OC channel of each servo.
 * @note Timer OCs will be initialized according to the number
 *       of servo used, see @ref SERVO_configuration.
 * @{
 */
#define SERVO_TIM_CLK                 RCC_APB1Periph_TIM2 //!< Only RCC_APB1Periph_TIM2...5 supported.
#define SERVO_TIM                     TIM2 //!< Only TIM2...5 supported.

#define SERVO_1_TIM_OC_PRELOAD_FUNC   TIM_OC1PreloadConfig
#define SERVO_1_TIM_OC_INIT_FUNC      TIM_OC1Init
#define SERVO_1_TIM_CCR               CCR1

#define SERVO_2_TIM_OC_PRELOAD_FUNC   TIM_OC2PreloadConfig
#define SERVO_2_TIM_OC_INIT_FUNC      TIM_OC2Init
#define SERVO_2_TIM_CCR               CCR2

#define SERVO_3_TIM_OC_PRELOAD_FUNC   TIM_OC3PreloadConfig
#define SERVO_3_TIM_OC_INIT_FUNC      TIM_OC3Init
#define SERVO_3_TIM_CCR               CCR3

#define SERVO_4_TIM_OC_PRELOAD_FUNC   TIM_OC4PreloadConfig
#define SERVO_4_TIM_OC_INIT_FUNC      TIM_OC4Init
#define SERVO_4_TIM_CCR               CCR4
/**
 * @}
 */

/**
 * @defgroup SERVO_select
 * @{
 */
#define SERVO_1                       0x01
#define SERVO_2                       0x02
#define SERVO_3                       0x04
#define SERVO_4                       0x08
#define SERVO_ALL                     0x0F
/**
 * @}
 */

void SERVO_Init(void);
void SERVO_SetAngle(uint8_t servoX, float degree);

/**
 * @}
 */ 
#endif
