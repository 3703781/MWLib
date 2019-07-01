/**
 * @file    tb6612fng.c
 * @author  Miaow, JYL
 * @version 0.4.2
 * @date    2018/08/07
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of TB6612FNG:
 *              1. Initialization and Configuration
 *              2. Motor operation
 * @note
 *          Minimum version of header file:
 *              0.4.0
 *          Pin connection:
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
 *              https://github.com/3703781/mystm32f4-devices-lib
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
#include "tb6612fng.h"

/** @addtogroup TB6612FNG
 * @{
 */

/**
 * @brief Initialize TB6612FNG for all motors
 */
void TB6612FNG_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    //general use
    //GPIO
    RCC_AHB1PeriphClockCmd(TB6612FNG_GPIO_ALL_CLK, ENABLE);
    //STBY
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_STBYA_PIN;
    GPIO_Init(TB6612FNG_STBYA_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(TB6612FNG_STBYA_PORT, TB6612FNG_STBYA_PIN);//STBYA = L
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_STBYB_PIN;
    GPIO_Init(TB6612FNG_STBYB_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(TB6612FNG_STBYB_PORT, TB6612FNG_STBYB_PIN);//STBYB = L
    
    //TIM
    RCC_APB1PeriphClockCmd(TB6612FNG_PWM_TIM_CLK, ENABLE);
    TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_Prescaler = 1 - 1;
    TIM_BaseInitStructure.TIM_Period = 4200;//20KHz
    TIM_TimeBaseInit(TB6612FNG_PWM_TIM, &TIM_BaseInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//OCxREF is high when TIMx_CNT < TIMx_CCRx else low
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;//TIM_BaseInitStructure.TIM_Period + 1;//0% dutyrate
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//output high when OCxREF is high
    TIM_ARRPreloadConfig(TB6612FNG_PWM_TIM, DISABLE);//do not buffer ARR
    
    
    //motorA
    //AIN1
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_AIN1_PIN;
    GPIO_Init(TB6612FNG_AIN1_PORT, &GPIO_InitStructure);
    //AIN2
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_AIN2_PIN;
    GPIO_Init(TB6612FNG_AIN2_PORT, &GPIO_InitStructure);
    //PWMA
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_PWMA_PIN;
    GPIO_Init(TB6612FNG_PWMA_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(TB6612FNG_PWMA_PORT, TB6612FNG_PWMA_GPIO_PINSOURCE, TB6612FNG_PWM_GPIO_AF);
    //set initial output
    GPIO_SetBits(TB6612FNG_AIN1_PORT, TB6612FNG_AIN1_PIN);//AIN1 = H
    GPIO_ResetBits(TB6612FNG_AIN2_PORT, TB6612FNG_AIN2_PIN);//BIN2 = L
    //PWMA use TIM2 OC1 20kHz
    TB6612FNG_PWMA_TIM_OC_FUNC(TB6612FNG_PWM_TIM, &TIM_OCInitStructure);
    TB6612FNG_PWMA_TIM_OC_PRELOAD_FUNC(TB6612FNG_PWM_TIM, TIM_OCPreload_Disable);//do not buffer CCR1
    
    
    //motorB
    //BIN1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_BIN1_PIN;
    GPIO_Init(TB6612FNG_BIN1_PORT, &GPIO_InitStructure);
    //BIN2
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_BIN2_PIN;
    GPIO_Init(TB6612FNG_BIN2_PORT, &GPIO_InitStructure);
    //PWMB
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_PWMB_PIN;
    GPIO_Init(TB6612FNG_PWMB_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(TB6612FNG_PWMB_PORT, TB6612FNG_PWMB_GPIO_PINSOURCE, TB6612FNG_PWM_GPIO_AF);
    //set initial output
    GPIO_SetBits(TB6612FNG_BIN1_PORT, TB6612FNG_BIN1_PIN);//BIN1 = H
    GPIO_ResetBits(TB6612FNG_BIN2_PORT, TB6612FNG_BIN2_PIN);//BIN2 = L
    //PWMB use TIM2 OC2 20kHz
    TB6612FNG_PWMB_TIM_OC_FUNC(TB6612FNG_PWM_TIM, &TIM_OCInitStructure);
    TB6612FNG_PWMB_TIM_OC_PRELOAD_FUNC(TB6612FNG_PWM_TIM, TIM_OCPreload_Disable);//do not buffer CCR2

    
    //motorC
    //CIN1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_CIN1_PIN;
    GPIO_Init(TB6612FNG_CIN1_PORT, &GPIO_InitStructure);
    //CIN2
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_CIN2_PIN;
    GPIO_Init(TB6612FNG_CIN2_PORT, &GPIO_InitStructure);
    //PWMC
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_PWMC_PIN;
    GPIO_Init(TB6612FNG_PWMC_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(TB6612FNG_PWMC_PORT, TB6612FNG_PWMC_GPIO_PINSOURCE, TB6612FNG_PWM_GPIO_AF);
    //set initial output
    GPIO_SetBits(TB6612FNG_CIN1_PORT, TB6612FNG_CIN1_PIN);//CIN1 = H
    GPIO_ResetBits(TB6612FNG_CIN2_PORT, TB6612FNG_CIN2_PIN);//CIN2 = L
    //PWMC use TIM2 OC3 20kHz
    TB6612FNG_PWMC_TIM_OC_FUNC(TB6612FNG_PWM_TIM, &TIM_OCInitStructure);
    TB6612FNG_PWMC_TIM_OC_PRELOAD_FUNC(TB6612FNG_PWM_TIM, TIM_OCPreload_Disable);//do not buffer CCR3
    
    
    //motorD
    //DIN1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_DIN1_PIN;
    GPIO_Init(TB6612FNG_DIN1_PORT, &GPIO_InitStructure);
    //DIN2
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_DIN2_PIN;
    GPIO_Init(TB6612FNG_DIN2_PORT, &GPIO_InitStructure);
    //PWMD
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = TB6612FNG_PWMD_PIN;
    GPIO_Init(TB6612FNG_PWMD_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(TB6612FNG_PWMD_PORT, TB6612FNG_PWMD_GPIO_PINSOURCE, TB6612FNG_PWM_GPIO_AF);
    //set initial output
    GPIO_SetBits(TB6612FNG_DIN1_PORT, TB6612FNG_DIN1_PIN);//DIN1 = H
    GPIO_ResetBits(TB6612FNG_DIN2_PORT, TB6612FNG_DIN2_PIN);//DIN2 = L
    //PWMD use TIM2 OC2 20kHz
    TB6612FNG_PWMD_TIM_OC_FUNC(TB6612FNG_PWM_TIM, &TIM_OCInitStructure);
    TB6612FNG_PWMD_TIM_OC_PRELOAD_FUNC(TB6612FNG_PWM_TIM, TIM_OCPreload_Disable);//do not buffer CCR4
    
    //start pwm output
    TIM_Cmd(TB6612FNG_PWM_TIM, ENABLE);
    
}

/**
 * @brief Set speed of specific motor
 * @param motorX            TB6612FNG_MOTOR_X where X can be (A...D) to select motor.
 *                          This parameter can be any combination of @ref TB6612FNG_motor_select.
 * @param speedPercentage   This parameter represents voltage percentage for the specified motors.
 *                          This parameter should be within the range of [-100.0f, 100.0f], value less than zero reverses rotation of motor(s).
 */
void TB6612FNG_RunPercentage(uint8_t motorX, float speedPercentage)
{
    TB6612FNG_Run(motorX, (uint32_t)speedPercentage / 100 * TB6612FNG_PWM_TIM->ARR);
}

/**
 * @brief Set speed of specific motor
 * @param motorX            TB6612FNG_MOTOR_X where X can be (A...D) to select motor.
 *                          This parameter can be any combination of @ref TB6612FNG_motor_select.
 * @param pwmPulse          The absolute value of this parameter configures the pulse time of pwm period.
 *                          This parameter should be within the range of [-4200, 4200], value less than zero reverses rotation of motor(s).
 *                          The dutyrate is computed using the following formula:
 *                           - dutyrate = abs(pwmPulse) / period
 *                          Where period is the value in the ARR register.
 */
void TB6612FNG_Run(uint8_t motorX, int32_t pwmPulse)
{
    if(motorX & TB6612FNG_MOTOR_A)
    {
        if(pwmPulse > 0)
        {
            TB6612FNG_AIN1_PORT->BSRRL = TB6612FNG_AIN1_PIN;//AIN1 = H
            TB6612FNG_AIN2_PORT->BSRRH = TB6612FNG_AIN2_PIN;//AIN2 = L
            TB6612FNG_PWMA_TIM_CCR = pwmPulse;//set CCR1
        }
        else if (pwmPulse < 0)
        {
            TB6612FNG_AIN1_PORT->BSRRH = TB6612FNG_AIN1_PIN;//AIN1 = L
            TB6612FNG_AIN2_PORT->BSRRL = TB6612FNG_AIN2_PIN;//AIN2 = H
            TB6612FNG_PWMA_TIM_CCR = -pwmPulse;//set CCR1
        }
    }
    if(motorX & TB6612FNG_MOTOR_B)
    {
        if(pwmPulse > 0)
        {
            TB6612FNG_BIN1_PORT->BSRRL = TB6612FNG_BIN1_PIN;//BIN1 = H
            TB6612FNG_BIN2_PORT->BSRRH = TB6612FNG_BIN2_PIN;//BIN2 = L
            TB6612FNG_PWMB_TIM_CCR = pwmPulse;//set CCR2
        }
        else if (pwmPulse < 0)
        {
            TB6612FNG_BIN1_PORT->BSRRH = TB6612FNG_BIN1_PIN;//BIN1 = L
            TB6612FNG_BIN2_PORT->BSRRL = TB6612FNG_BIN2_PIN;//BIN2 = H
            TB6612FNG_PWMB_TIM_CCR = -pwmPulse;//set CCR2
        }
    }
    if(motorX & TB6612FNG_MOTOR_C)
    {
        if(pwmPulse > 0)
        {
            TB6612FNG_CIN1_PORT->BSRRL = TB6612FNG_CIN1_PIN;//CIN1 = H
            TB6612FNG_CIN2_PORT->BSRRH = TB6612FNG_CIN2_PIN;//CIN2 = L
            TB6612FNG_PWMC_TIM_CCR = pwmPulse;//set CCR3
        }
        else if (pwmPulse < 0)
        {
            TB6612FNG_CIN1_PORT->BSRRH = TB6612FNG_CIN1_PIN;//CIN1 = L
            TB6612FNG_CIN2_PORT->BSRRL = TB6612FNG_CIN2_PIN;//CIN2 = H
            TB6612FNG_PWMC_TIM_CCR = -pwmPulse;//set CCR3
        }
    }
    if(motorX & TB6612FNG_MOTOR_D)
    {
        if(pwmPulse > 0)
        {
            TB6612FNG_DIN1_PORT->BSRRL = TB6612FNG_DIN1_PIN;//DIN1 = H
            TB6612FNG_DIN2_PORT->BSRRH = TB6612FNG_DIN2_PIN;//DIN2 = L
            TB6612FNG_PWMD_TIM_CCR = pwmPulse;//set CCR4
        }
        else if (pwmPulse < 0)
        {
            TB6612FNG_DIN1_PORT->BSRRH = TB6612FNG_DIN1_PIN;//DIN1 = L
            TB6612FNG_DIN2_PORT->BSRRL = TB6612FNG_DIN2_PIN;//DIN2 = H
            TB6612FNG_PWMD_TIM_CCR = -pwmPulse;//set CCR4
        }
    }
    
    TB6612FNG_STBYA_PORT->BSRRL = TB6612FNG_STBYA_PIN;//STBYA = H
    TB6612FNG_STBYB_PORT->BSRRL = TB6612FNG_STBYB_PIN;//STBYB = H
}

/**
 * @brief Stop(brake) the specified motor(s)
 * @param motorX            TB6612FNG_MOTOR_X where X can be (A...D) to select motor.
 *                          This parameter can be any combination of @ref TB6612FNG_motor_select.
 */
void TB6612FNG_Stop(uint8_t motorX)
{
    if(motorX & TB6612FNG_MOTOR_A)
        TB6612FNG_PWMA_TIM_CCR = 0;//PWMA = L
    if(motorX & TB6612FNG_MOTOR_B)
        TB6612FNG_PWMB_TIM_CCR = 0;//PWMB = L
    if(motorX & TB6612FNG_MOTOR_C)
        TB6612FNG_PWMC_TIM_CCR = 0;//PWMC = L
    if(motorX & TB6612FNG_MOTOR_D)
        TB6612FNG_PWMD_TIM_CCR = 0;//PWMD = L
    
    TB6612FNG_STBYA_PORT->BSRRL = TB6612FNG_STBYA_PIN;//STBYA = H
    TB6612FNG_STBYB_PORT->BSRRL = TB6612FNG_STBYB_PIN;//STBYB = H
}

/**
 * @brief Sleep all motors
 * @param driverX           TB6612FNG_X where X can be (A, B) to select motor.
 *                          This parameter can be any combination of @ref TB6612FNG_select.
 */
void TB6612FNG_Sleep(uint8_t driverX)
{
    if(driverX & TB6612FNG_A)
        TB6612FNG_STBYA_PORT->BSRRH = TB6612FNG_STBYA_PIN;//STBYA = L
    if(driverX & TB6612FNG_B)
        TB6612FNG_STBYB_PORT->BSRRH = TB6612FNG_STBYB_PIN;//STBYB = L
}
/**
 * @}
 */

