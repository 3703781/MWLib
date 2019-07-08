/**
 * @file    tb6612fng.h
 * @author  Miaow
 * @version 0.4.0
 * @date    2018/08/06
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of TB6612FNG:
 *              1. Initialization and Configuration
 *              2. Motor operation
 * @note     
 *           Minimum version of source file:
 *              0.4.0
 *
 *           Steps to modify XIN1, XIN2, STBY:
 *              1.modify GPIO port, pin, clk
 *
 *           Steps to modify PWMx where x can be (A...D):
 *              1.modify GPIO port, pin, clk, pinsource, af
 *              2.modify 
 *                TB6612FNG_PWM_TIM, TB6612FNG_PWM_TIM_CLK, 
 *                TB6612FNG_PWM_TIM_ARR, TB6612FNG_PWMx_TIM_OC_FUNC, 
 *                TB6612FNG_PWMx_TIM_OC_PRELOAD_FUNC, TB6612FNG_PWMx_TIM_CHANNEL, 
 *                TB6612FNG_PWMx_TIM_CCR
 *                to choose timer and channel corresponding to GPIO
 *
 *           Pin connection:
 *              ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©´
 *              ©¦    PWMA©À©¤©¤©¤©¤©¤©ÈPA0    PB10©À©¤©¤©¤©¤©¤©ÈPWMA(PWMC)©¦
 *              ©¦    AIN2©À©¤©¤©¤©¤©¤©ÈPC0     PD3©À©¤©¤©¤©¤©¤©ÈAIN2(CIN2)©¦
 *              ©¦    AIN1©À©¤©¤©¤©¤©¤©ÈPC1     PD4©À©¤©¤©¤©¤©¤©ÈAIN1(CIN1)©¦
 *              ©¦    STBY©À©¤©¤©¤©¤©¤©ÈPC2     PD5©À©¤©¤©¤©¤©¤©ÈSTBY      ©¦
 *              ©¦    BIN1©À©¤©¤©¤©¤©¤©ÈPC3     PD6©À©¤©¤©¤©¤©¤©ÈBIN1(DIN1)©¦
 *              ©¦    BIN2©À©¤©¤©¤©¤©¤©ÈPC4     PD7©À©¤©¤©¤©¤©¤©ÈBIN2(DIN2)©¦
 *              ©¦    PWMB©À©¤©¤©¤©¤©¤©ÈPA1    PB11©À©¤©¤©¤©¤©¤©ÈPWMB(PWMD)©¦
 *              ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *              TB6612FNG_1      STM32F407       TB6612FNG_2
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#ifndef __TB6612FNG_H
#define __TB6612FNG_H

#include "stm32f4xx.h"

/** 
 * @defgroup TB6612FNG
 * @brief TB6612FNG driver modules
 * @{
 */

/** 
 * @defgroup TB6612FNG_gpio_define
 * @{
 */
//GPIO for general use
#define TB6612FNG_PWM_GPIO_AF               GPIO_AF_TIM2
#define TB6612FNG_STBYA_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define TB6612FNG_STBYA_PORT                GPIOC
#define TB6612FNG_STBYA_PIN                 GPIO_Pin_2
#define TB6612FNG_STBYB_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define TB6612FNG_STBYB_PORT                GPIOD
#define TB6612FNG_STBYB_PIN                 GPIO_Pin_5
#define TB6612FNG_STBY_GPIO_CLK             (TB6612FNG_STBYA_GPIO_CLK | TB6612FNG_STBYB_GPIO_CLK)

//GPIO for motorA
#define TB6612FNG_AIN1_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define TB6612FNG_AIN1_PORT                 GPIOC
#define TB6612FNG_AIN1_PIN                  GPIO_Pin_1
#define TB6612FNG_AIN2_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define TB6612FNG_AIN2_PORT                 GPIOC
#define TB6612FNG_AIN2_PIN                  GPIO_Pin_0
#define TB6612FNG_PWMA_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define TB6612FNG_PWMA_PORT                 GPIOA
#define TB6612FNG_PWMA_PIN                  GPIO_Pin_0
#define TB6612FNG_PWMA_GPIO_PINSOURCE       GPIO_PinSource0
#define TB6612FNG_PIN_A_ALL_CLK             (TB6612FNG_AIN1_GPIO_CLK | TB6612FNG_AIN2_GPIO_CLK | TB6612FNG_PWMA_GPIO_CLK)

//GPIO for motorB
#define TB6612FNG_BIN1_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define TB6612FNG_BIN1_PORT                 GPIOC
#define TB6612FNG_BIN1_PIN                  GPIO_Pin_3
#define TB6612FNG_BIN2_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define TB6612FNG_BIN2_PORT                 GPIOC
#define TB6612FNG_BIN2_PIN                  GPIO_Pin_4
#define TB6612FNG_PWMB_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define TB6612FNG_PWMB_PORT                 GPIOA
#define TB6612FNG_PWMB_PIN                  GPIO_Pin_1
#define TB6612FNG_PWMB_GPIO_PINSOURCE       GPIO_PinSource1
#define TB6612FNG_PIN_B_ALL_CLK             (TB6612FNG_BIN1_GPIO_CLK | TB6612FNG_BIN2_GPIO_CLK | TB6612FNG_PWMB_GPIO_CLK)

//GPIO for motorC
#define TB6612FNG_CIN1_GPIO_CLK             RCC_AHB1Periph_GPIOD
#define TB6612FNG_CIN1_PORT                 GPIOD
#define TB6612FNG_CIN1_PIN                  GPIO_Pin_4
#define TB6612FNG_CIN2_GPIO_CLK             RCC_AHB1Periph_GPIOD
#define TB6612FNG_CIN2_PORT                 GPIOD
#define TB6612FNG_CIN2_PIN                  GPIO_Pin_3
#define TB6612FNG_PWMC_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define TB6612FNG_PWMC_PORT                 GPIOB
#define TB6612FNG_PWMC_PIN                  GPIO_Pin_10
#define TB6612FNG_PWMC_GPIO_PINSOURCE       GPIO_PinSource10
#define TB6612FNG_PIN_C_ALL_CLK             (TB6612FNG_CIN1_GPIO_CLK | TB6612FNG_CIN2_GPIO_CLK | TB6612FNG_PWMC_GPIO_CLK)

//GPIO for motorD
#define TB6612FNG_DIN1_GPIO_CLK             RCC_AHB1Periph_GPIOD
#define TB6612FNG_DIN1_PORT                 GPIOD
#define TB6612FNG_DIN1_PIN                  GPIO_Pin_6
#define TB6612FNG_DIN2_GPIO_CLK             RCC_AHB1Periph_GPIOD
#define TB6612FNG_DIN2_PORT                 GPIOD
#define TB6612FNG_DIN2_PIN                  GPIO_Pin_7
#define TB6612FNG_PWMD_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define TB6612FNG_PWMD_PORT                 GPIOB
#define TB6612FNG_PWMD_PIN                  GPIO_Pin_11
#define TB6612FNG_PWMD_GPIO_PINSOURCE       GPIO_PinSource11
#define TB6612FNG_PIN_D_ALL_CLK             (TB6612FNG_DIN1_GPIO_CLK | TB6612FNG_DIN2_GPIO_CLK | TB6612FNG_PWMD_GPIO_CLK)

#define TB6612FNG_GPIO_ALL_CLK              (TB6612FNG_STBY_GPIO_CLK | TB6612FNG_PIN_A_ALL_CLK | TB6612FNG_PIN_B_ALL_CLK | TB6612FNG_PIN_C_ALL_CLK | TB6612FNG_PIN_D_ALL_CLK)

/**
 * @}
 */
 
 
/** 
 * @defgroup TB6612FNG_pwm_define
 * @{
 */
//PWM for general
#define TB6612FNG_PWM_TIM_CLK               RCC_APB1Periph_TIM2
#define TB6612FNG_PWM_TIM                   TIM2
#define TB6612FNG_PWM_TIM_ARR               (TB6612FNG_PWM_TIM->ARR)

//PWM for motorA using TIM2 OC1
#define TB6612FNG_PWMA_TIM_OC_FUNC          TIM_OC1Init
#define TB6612FNG_PWMA_TIM_OC_PRELOAD_FUNC  TIM_OC1PreloadConfig
#define TB6612FNG_PWMA_TIM_CHANNEL          TIM_Channel_1
#define TB6612FNG_PWMA_TIM_CCR              (TB6612FNG_PWM_TIM->CCR1)

//PWM for motorB using TIM2 OC2
#define TB6612FNG_PWMB_TIM_OC_FUNC          TIM_OC2Init
#define TB6612FNG_PWMB_TIM_OC_PRELOAD_FUNC  TIM_OC2PreloadConfig
#define TB6612FNG_PWMB_TIM_CHANNEL          TIM_Channel_2
#define TB6612FNG_PWMB_TIM_CCR              (TB6612FNG_PWM_TIM->CCR2)

//PWM for motorC using TIM2 OC3
#define TB6612FNG_PWMC_TIM_OC_FUNC          TIM_OC3Init
#define TB6612FNG_PWMC_TIM_OC_PRELOAD_FUNC  TIM_OC3PreloadConfig
#define TB6612FNG_PWMC_TIM_CHANNEL          TIM_Channel_3
#define TB6612FNG_PWMC_TIM_CCR              (TB6612FNG_PWM_TIM->CCR3)

//PWM for motorD using TIM2 OC4
#define TB6612FNG_PWMD_TIM_OC_FUNC          TIM_OC4Init
#define TB6612FNG_PWMD_TIM_OC_PRELOAD_FUNC  TIM_OC4PreloadConfig
#define TB6612FNG_PWMD_TIM_CHANNEL          TIM_Channel_4
#define TB6612FNG_PWMD_TIM_CCR              (TB6612FNG_PWM_TIM->CCR4)
/**
 * @}
 */


/** 
 * @defgroup TB6612FNG_motor_select
 * @{
 */
#define TB6612FNG_MOTOR_A                   0x01
#define TB6612FNG_MOTOR_B                   0x02
#define TB6612FNG_MOTOR_C                   0x04
#define TB6612FNG_MOTOR_D                   0x08
#define TB6612FNG_MOTOR_ALL                 0x0F
/**
 * @}
 */

/** 
 * @defgroup TB6612FNG_select
 * @{
 */
#define TB6612FNG_A                         0x01
#define TB6612FNG_B                         0x02
#define TB6612FNG_BOTH                      0x03
/**
 * @}
 */

void TB6612FNG_Init(void);
void TB6612FNG_RunPercentage(uint8_t motorX, float speedPercentage);
void TB6612FNG_Run(uint8_t motorX, int32_t pwmPulse);
void TB6612FNG_Stop(uint8_t motorX);
void TB6612FNG_Sleep(uint8_t driverX);

/**
 * @}
 */ 
#endif
