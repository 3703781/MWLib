/**
 * @file    hallencoder.c
 * @author  Miaow
 * @version 0.2.0
 * @date    2018/08/08
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of hall encoders:
 *              1. Initialization
 *              2. Get values for speed measurement 
 * @note
 *          Minimum version of header file:
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
 
#include "hallencoder.h"

/** @addtogroup HALLENCODER
 * @{
 */

/**
 * @brief Initialize hall encoders
 */
void HALLENCODER_Init()
{
    //Initialize GPIO after ICs to avoid going into hardfault
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //TIM3 for hall encoder A
    RCC_APB1PeriphClockCmd(HALLENCODER_A_TIM_CLK, ENABLE);
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_Period = UINT16_MAX;//up to a very large number per period
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(HALLENCODER_A_TIM, &TIM_TimeBaseStructure);
    //Initialize IC1, IC2 for phase A, B
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 100;
    TIM_ICInit(HALLENCODER_A_TIM, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(HALLENCODER_A_TIM, &TIM_ICInitStructure);
    TIM_EncoderInterfaceConfig(HALLENCODER_A_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_SetCounter(HALLENCODER_A_TIM, INT16_MAX);
    
    //TIM4 for encode B
    RCC_APB1PeriphClockCmd(HALLENCODER_B_TIM_CLK, ENABLE);
    TIM_TimeBaseInit(HALLENCODER_B_TIM, &TIM_TimeBaseStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(HALLENCODER_B_TIM, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(HALLENCODER_B_TIM, &TIM_ICInitStructure);
    TIM_EncoderInterfaceConfig(HALLENCODER_B_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_SetCounter(HALLENCODER_B_TIM, INT16_MAX);
    
    //GPIO for hall encoder A
    RCC_AHB1PeriphClockCmd(HALLENCODER_A_GPIO_CLK_ALL, ENABLE);
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_Pin = HALLENCODER_A_PHASEA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_PinAFConfig(HALLENCODER_A_PHASEA_PORT, HALLENCODER_A_PHASEA_GPIO_PINSOURCE, HALLENCODER_A_GPIO_AF);
    GPIO_Init(HALLENCODER_A_PHASEA_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = HALLENCODER_A_PHASEB_PIN;
    GPIO_PinAFConfig(HALLENCODER_A_PHASEB_PORT, HALLENCODER_A_PHASEB_GPIO_PINSOURCE, HALLENCODER_A_GPIO_AF);
    GPIO_Init(HALLENCODER_A_PHASEB_PORT, &GPIO_InitStructure);
    
    //GPIO for hall encoder B
    RCC_AHB1PeriphClockCmd(HALLENCODER_B_GPIO_CLK_ALL, ENABLE);
    GPIO_InitStructure.GPIO_Pin = HALLENCODER_B_PHASEA_PIN;
    GPIO_PinAFConfig(HALLENCODER_B_PHASEA_PORT, HALLENCODER_B_PHASEA_GPIO_PINSOURCE, HALLENCODER_B_GPIO_AF);
    GPIO_Init(HALLENCODER_B_PHASEA_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = HALLENCODER_B_PHASEB_PIN;
    GPIO_PinAFConfig(HALLENCODER_B_PHASEB_PORT, HALLENCODER_B_PHASEB_GPIO_PINSOURCE, HALLENCODER_B_GPIO_AF);
    GPIO_Init(HALLENCODER_B_PHASEB_PORT, &GPIO_InitStructure);
    
    TIM_Cmd(HALLENCODER_A_TIM, ENABLE);
    TIM_Cmd(HALLENCODER_B_TIM, ENABLE);
}

/**
 * @brief Get values for speed measurement of specific motor.
 * @param hallEncoderX      HALLENCODER_X where X can be (A...B) to select hall encoder.
 *                          DO NOT pass in any combination of @ref HALLENCODER_hallencoder_select.
 * @return                  Return the increment of CNT register from last invoke.
 */
int32_t HALLENCODER_ReadDeltaValue(uint8_t hallEncoderX)
{
    int32_t deltaValue;    
    switch(hallEncoderX)
    {
        case HALLENCODER_A:
            deltaValue = HALLENCODER_A_TIM->CNT - INT16_MAX;//calculate increment
            HALLENCODER_A_TIM->CNT = INT16_MAX;//clear CNT register
            break;
        case HALLENCODER_B:
            deltaValue = HALLENCODER_B_TIM->CNT - INT16_MAX;//calculate increment
            HALLENCODER_B_TIM->CNT = INT16_MAX;//clear CNT register
            break;
        default:
            return 0;
    }
    return deltaValue;
}

/**
 * @}
 */
