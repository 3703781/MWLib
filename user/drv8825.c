/**
 * @file    drv8825.c
 * @author  Miaow, JYL
 * @version 0.1.0
 * @date    2018/09/24
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of DRV8825:
 *              1. Initialization and Configuration
 *              2. Motor operation
 * @note
 *          Minimum version of header file:
 *              0.1.0
 *          Pin connection:
 *             ©°©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©´
 *             ©¦  STP©À©¤©¤©¤©¤©¤©ÈPA2    ©¦
 *             ©¦  DIR©À©¤©¤©¤©¤©¤©ÈPC5    ©¦
 *             ©¸©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¼
 *             DRV8825     STM32F407  
 *  
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781/mystm32f4-devices-lib
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
#include "drv8825.h"

/** @addtogroup DRV8825
 * @{
 */

#define DRV8825_CTRL_TIM_ARR                (DRV8825_CTRL_TIM->ARR)
#define DRV8825_CTRL_TIM_CNT                (DRV8825_CTRL_TIM->CNT)
#define DRV8825_CTRL_TIM_CCR                (DRV8825_CTRL_TIM->CCR1)
#define DRV8825_STP_TIM_ARR                 (DRV8825_STP_TIM->ARR)
#define DRV8825_STP_TIM_CNT                 (DRV8825_STP_TIM->CNT)
#define DRV8825_STP_TIM_CCR                 (DRV8825_STP_TIM->CCR1)

/**
 * @brief Initialize DRV8825 module.
 */
void DRV8825_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    //GPIO
    RCC_AHB1PeriphClockCmd(DRV8825_GPIO_ALL_CLK, ENABLE);
    //DIR
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_Pin = DRV8825_DIR_PIN;
    GPIO_Init(DRV8825_DIR_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(DRV8825_DIR_PORT, DRV8825_DIR_PIN);//DIR = L
    //STP
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = DRV8825_STP_PIN;
    GPIO_Init(DRV8825_STP_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DRV8825_STP_PORT, DRV8825_STP_GPIO_PINSOURCE, DRV8825_STP_GPIO_AF);
    
    
    //TIM10 for triggle - master
    DRV8825_CTRL_TIM_CLK_FUNC(DRV8825_CTRL_TIM_CLK, ENABLE);
    TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_Prescaler = 4375 - 1;//38400Hz
    TIM_BaseInitStructure.TIM_Period = 10 - 1;
    TIM_ARRPreloadConfig(DRV8825_CTRL_TIM, DISABLE);//do not buffer ARR
    TIM_TimeBaseInit(DRV8825_CTRL_TIM, &TIM_BaseInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//OCxREF is low when TIMx_CNT < TIMx_CCRx else high
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 5;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//output high when OCxREF is high
    TIM_OC1PreloadConfig(DRV8825_CTRL_TIM, TIM_OCPreload_Disable);
    TIM_OC1Init(DRV8825_CTRL_TIM, &TIM_OCInitStructure);
    TIM_SelectMasterSlaveMode(DRV8825_CTRL_TIM, TIM_MasterSlaveMode_Enable);
    TIM_SelectOnePulseMode(DRV8825_CTRL_TIM, TIM_OPMode_Single);//one pulse mode
    TIM_OC1PreloadConfig(DRV8825_CTRL_TIM, TIM_OCPreload_Disable);
    TIM_SelectOutputTrigger(DRV8825_CTRL_TIM, DRV8825_CTRL_TIM_TRGO_SOURCE);//TRGO is OC1Ref
    
    
    
    //TIM9 for pwm - slave
    DRV8825_STP_TIM_CLK_FUNC(DRV8825_STP_TIM_CLK, ENABLE);
    TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_Prescaler = 35 - 1;
    TIM_BaseInitStructure.TIM_Period = 125 - 1;//38400Hz 1080¡ã/s
    TIM_ARRPreloadConfig(DRV8825_STP_TIM, DISABLE);//do not buffer ARR
    TIM_TimeBaseInit(DRV8825_STP_TIM, &TIM_BaseInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//OCxREF is high when TIMx_CNT < TIMx_CCRx else low
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = (TIM_BaseInitStructure.TIM_Period - 1) >> 2;//50% dutyrate
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//output high when OCxREF is high
    DRV8825_STP_TIM_OC_PRELOAD_FUNC(DRV8825_STP_TIM, TIM_OCPreload_Disable);//do not buffer CCR1
    DRV8825_STP_TIM_OC_FUNC(DRV8825_STP_TIM, &TIM_OCInitStructure);
    
    TIM_SelectSlaveMode(DRV8825_STP_TIM, TIM_SlaveMode_Gated);//GATED slave mode
    TIM_SelectInputTrigger(DRV8825_STP_TIM, DRV8825_STP_TIM_TRIG_SOURCE);//TIM10 will triggle DRV8825_STP_TIM
    TIM_Cmd(DRV8825_STP_TIM, ENABLE);
    
    
}

/**
 * @brief Stop and hold the stepper motor.
 */
inline void DRV8825_Stop()
{
    DRV8825_STP_TIM_CCR = 0;
}

/**
 * @brief Set speed and angle of the stepper motor
 * @param speed             Speed in ¡ã/s of the stepper motor.
 *                          This parameter should be positive.
 *                          This parameter should be within the range of [0, 210]
 * @param angle             Angle in degree of the stepper motor.
 *                          This parameter should be within the range of [-360, 360], value less than zero reverses rotation of stepper motor.
 */
void DRV8825_Run(float speed, float angle)
{
    if(angle < 0)
    {
        angle = -angle;
        DRV8825_DIR_PORT -> BSRRH = DRV8825_DIR_PIN;//DIR = L
    }
    else if(angle > 0)
    {
        DRV8825_DIR_PORT -> BSRRL = DRV8825_DIR_PIN;//DIR = H
    }
    else
    {
        DRV8825_Stop();
        return;
    }
    DRV8825_STP_TIM_ARR = (uint32_t)(135000.0f / speed);//(uint32_t)(4800000.0f/(speed/0.9f*32.0f)) - 1;    675   7111.11Hz
    DRV8825_STP_TIM_CCR = (DRV8825_STP_TIM_ARR - 1) >> 1;//50% dutyrate
    //4800000/DRV8825_STP_TIM_ARR=35.55555555555555555555f*speeds
    //38400/(4800000/DRV8825_STP_TIM_ARR)=1080/speed
    
    DRV8825_CTRL_TIM_CCR = 1;
    DRV8825_CTRL_TIM_ARR = (uint32_t)(38400.0f * angle / speed) + 1;//(angle * 32.0f / 0.9f * 1080 / speed);      172   223.25Hz;
    TIM_Cmd(DRV8825_CTRL_TIM, ENABLE);
}

/** 
 * @}
 */
