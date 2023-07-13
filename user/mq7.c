/**
 * @file    mq7.c
 * @author  Miaow
 * @version 0.1.1
 * @date    2023/07/13
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of water level sensor:
 *              1. Initialization.
 *              2. Measure and get CO concentration.
 * @note     
 *           Minimum version of header file:
 *              0.1.0
 *
 *          Pin connection:
 *          ┌────────┐     ┌────────┐
 *          │     PA6├─────┤AO      │
 *          └────────┘     └────────┘
 *          STM32F407       MQ7 Sensor
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */


#include "mq7.h"
#include "math.h"
#define CAL_PPM 10 // 校准环境中PPM值
#define RL 10      // RL阻值


/** @addtogroup MQ7
  * @{
  */

/**
 * 
 * 
 */
void MQ7_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;

    RCC_AHB1PeriphClockCmd(MQ7_ANALOG_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = MQ7_ANALOG_GPIO_PIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_Init(MQ7_ANALOG_GPIO_PORT, &GPIO_InitStructure);

    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;

    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;

    ADC_CommonInit(&ADC_CommonInitStructure);
    ADC_Init(MQ7_ADCx, &ADC_InitStructure);
    ADC_RegularChannelConfig(MQ7_ADCx, MQ7_ADC_CHANNEL, 1, ADC_SampleTime_480Cycles);
    ADC_Cmd(MQ7_ADCx, ENABLE);
}

static float R0 = 8.00;

/**
 * 
 * 
 */
float Get_ADCValue_MQ7()
{
    uint32_t val = 0;
    float times = 10;
    uint8_t count;
    for (count = 0; count < times; count++)
    {
        MQ7_ADCx->CR2 |= (uint32_t)ADC_CR2_SWSTART;
        while (!(MQ7_ADCx->SR & ADC_FLAG_EOC))
            ;

        val += MQ7_ADCx->DR;
        UTILS_DelayMs(5);
    }
    return (float)val / times;
}

/**
 * 
 * 
 */
void MQ7_PPM_Calibration()
{
    float Vrl = 3.3f * Get_ADCValue_MQ7() / 4096.f;
    Vrl = ((float)((int)((Vrl + 0.005f) * 100.0f))) / 100.0f;
    float RS = (3.3f - Vrl) / Vrl * RL;
    R0 = RS / pow(CAL_PPM / 98.322, 1 / -1.458f);
}

/**
 * 
 * 
 */
float MQ7_GetPPM()
{
    float Vrl = 3.3f * Get_ADCValue_MQ7() / 4096.f;
    Vrl = ((float)((int)((Vrl + 0.005f) * 100.0f))) / 100.0f;
    float RS = (3.3f - Vrl) / Vrl * RL;

    float ppm = 98.322f * pow(RS / R0, -1.458f);

    return ppm;
}

/**
 * @}
 */

