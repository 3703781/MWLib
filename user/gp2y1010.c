/**
 * @file    gp2y1010.c
 * @author  Miaow
 * @version 0.1.0
 * @date    2021/08/30
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of GP2Y1010:
 *              1. Initialization
 *              2. Measurement
 * @note
 *          Minimum version of header file:
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

#include "gp2y1010.h"

/** @addtogroup GP2Y1010
  * @{
  */

/**
 * @brief Initialize the sensor
 */
void GP2Y1010_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;

    RCC_AHB1PeriphClockCmd(GP2Y1010_GPIO_ADC_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(GP2Y1010_ADC_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GP2Y1010_GPIO_ADC_PIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;

    GPIO_Init(GP2Y1010_GPIO_ADC_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = GP2Y1010_GPIO_LED_PIN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;

    GPIO_Init(GP2Y1010_GPIO_LED_PORT, &GPIO_InitStructure);

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
    ADC_Init(GP2Y1010_ADC, &ADC_InitStructure);
    ADC_RegularChannelConfig(GP2Y1010_ADC, GP2Y1010_ADC_CHANNEL, 1, ADC_SampleTime_480Cycles);
    ADC_Cmd(GP2Y1010_ADC, ENABLE);
}

static float GP2Y1010_GetAdc(void)
{
    uint32_t val = 0;
    float times = 5;
    uint8_t count;
    for (count = 0; count < times; count++)
    {
        GP2Y1010_ADC->CR2 |= (uint32_t)ADC_CR2_SWSTART;
        while (!(GP2Y1010_ADC->SR & ADC_FLAG_EOC))
            ;

        val += GP2Y1010_ADC->DR;
        UTILS_DelayMs(5);
    }
    return (float)val / times;
}

/**
 * @brief Measure dust content in air.
 * @return The dust concentration, measured in mg/m^3
 */
float GP2Y1010_Get()
{
    uint32_t tmp;
    float pm;
    GP2Y1010_GPIO_LED_PORT->BSRRH |= GP2Y1010_GPIO_LED_PIN;
    UTILS_DelayUs(280);
    tmp = GP2Y1010_GetAdc();
    UTILS_DelayUs(40);
    GP2Y1010_GPIO_LED_PORT->BSRRL |= GP2Y1010_GPIO_LED_PIN;
    UTILS_DelayUs(9680);
    pm = 0.34f * tmp / 4096.0f * 3.3f - 0.1f;
    pm /= 6.0f;
    return pm;
}

/**
 * @}
 */