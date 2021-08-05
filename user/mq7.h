/**
 * @file    mq7.h
 * @author  Miaow
 * @version 0.1.0
 * @date    2021/08/01
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of water level sensor:
 *              1. Initialization.
 *              2. Measure and get CO concentration.
 * @note     
 *           Minimum version of source file:
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

#if !defined(__MQ7_H)
#define __MQ7_H
#include "utils.h"

/** 
 * @defgroup MQ7
 * @brief MQ7 modules
 * @{
 */

/** 
 * @defgroup MQ7_gpio_define
 * @{
 */
#define MQ7_ANALOG_GPIO_PORT GPIOA
#define MQ7_ANALOG_GPIO_PIN GPIO_Pin_6
#define MQ7_ANALOG_GPIO_CLK RCC_AHB1Periph_GPIOA
/**
 * @}
 */

/** 
 * @defgroup MQ7_adc_define
 * @{
 */
#define MQ7_ADCx ADC1
#define MQ7_ADCx_CLK RCC_APB2Periph_ADC1
#define MQ7_ADC_CHANNEL ADC_Channel_6
/**
 * @}
 */

void MQ7_Init(void);
void MQ7_PPM_Calibration(void);
float MQ7_GetPPM(void);
float Get_ADCValue_MQ7(void);


/**
 * @}
 */

#endif // __MQ7_H
