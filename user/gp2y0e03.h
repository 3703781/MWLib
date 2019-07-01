/**
 * @file    gp2y0e03.h
 * @author  Miaow
 * @version 0.1.1
 * @date    2018/10/19
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of GP2Y0E03, the active optical distance measurement sensor:
 *              1. Initialization
 *              2. Configurations
 *              3. distance Measurement
 * @note
 *          Minimum version of source file:
 *              0.1.0
 *          Pin connection:
 *          ┌────────┐     ┌────────┐
 *          │    PE12├─────┤SCL     │
 *          │    PE15├─────┤SDA     │
 *          └────────┘     └────────┘
 *          STM32F407       GP2Y0E03
 *          
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
 
#ifndef __GP2Y0E03_H
#define __GP2Y0E03_H

#include "stm32f4xx.h"

/** 
 * @defgroup GP2Y0E03
 * @{
 */
 
/** 
 * @defgroup GP2Y0E03_gpio_define
 * @{
 */
//GPIO
#define GP2Y0E03_1_SCL_PORT         GPIOE
#define GP2Y0E03_1_SCL_PIN          GPIO_Pin_12
#define GP2Y0E03_1_SCL_GPIO_CLK     RCC_AHB1Periph_GPIOE
#define GP2Y0E03_1_SDA_PORT         GPIOE
#define GP2Y0E03_1_SDA_PIN          GPIO_Pin_15
#define GP2Y0E03_1_SDA_GPIO_CLK     RCC_AHB1Periph_GPIOE
#define GP2Y0E03_1_In()             GP2Y0E03_1_SDA_PORT->MODER &= ~0xC0000000;GP2Y0E03_1_SDA_PORT->MODER |= 0x00000000//输入模式
#define GP2Y0E03_1_Out()            GP2Y0E03_1_SDA_PORT->MODER &= ~0xC0000000;GP2Y0E03_1_SDA_PORT->MODER |= 0x40000000//输出模式

#define GP2Y0E03_2_SCL_PORT         GPIOE
#define GP2Y0E03_2_SCL_PIN          GPIO_Pin_8
#define GP2Y0E03_2_SCL_GPIO_CLK     RCC_AHB1Periph_GPIOE
#define GP2Y0E03_2_SDA_PORT         GPIOE
#define GP2Y0E03_2_SDA_PIN          GPIO_Pin_10
#define GP2Y0E03_2_SDA_GPIO_CLK     RCC_AHB1Periph_GPIOE
#define GP2Y0E03_2_In()             GP2Y0E03_2_SDA_PORT->MODER &= ~0x00300000;GP2Y0E03_2_SDA_PORT->MODER |= 0x00000000//输入模式
#define GP2Y0E03_2_Out()            GP2Y0E03_2_SDA_PORT->MODER &= ~0x00300000;GP2Y0E03_2_SDA_PORT->MODER |= 0x00100000//输出模式

#define GP2Y0E03_3_SCL_PORT         GPIOE
#define GP2Y0E03_3_SCL_PIN          GPIO_Pin_2
#define GP2Y0E03_3_SCL_GPIO_CLK     RCC_AHB1Periph_GPIOE
#define GP2Y0E03_3_SDA_PORT         GPIOE
#define GP2Y0E03_3_SDA_PIN          GPIO_Pin_4
#define GP2Y0E03_3_SDA_GPIO_CLK     RCC_AHB1Periph_GPIOE
#define GP2Y0E03_3_In()             GP2Y0E03_3_SDA_PORT->MODER &= ~0x00000300;GP2Y0E03_3_SDA_PORT->MODER |= 0x00000000//输入模式
#define GP2Y0E03_3_Out()            GP2Y0E03_3_SDA_PORT->MODER &= ~0x00000300;GP2Y0E03_3_SDA_PORT->MODER |= 0x00000100//输出模式

#define GP2Y0E03_4_SCL_PORT         GPIOE
#define GP2Y0E03_4_SCL_PIN          GPIO_Pin_5
#define GP2Y0E03_4_SCL_GPIO_CLK     RCC_AHB1Periph_GPIOE
#define GP2Y0E03_4_SDA_PORT         GPIOE
#define GP2Y0E03_4_SDA_PIN          GPIO_Pin_3
#define GP2Y0E03_4_SDA_GPIO_CLK     RCC_AHB1Periph_GPIOE
#define GP2Y0E03_4_In()             GP2Y0E03_4_SDA_PORT->MODER &= ~0x000000C0;GP2Y0E03_4_SDA_PORT->MODER |= 0x00000000//输入模式
#define GP2Y0E03_4_Out()            GP2Y0E03_4_SDA_PORT->MODER &= ~0x000000C0;GP2Y0E03_4_SDA_PORT->MODER |= 0x00000040//输出模式
/**
 * @}
 */

/** 
 * @defgroup GP2Y0E03_gpio_define
 * @{
 */
#define GP2Y0E03_1_IIC_ADDRESS      0x40
#define GP2Y0E03_2_IIC_ADDRESS      0x40
#define GP2Y0E03_3_IIC_ADDRESS      0x40
#define GP2Y0E03_4_IIC_ADDRESS      0x40
/**
 * @}
 */

/** 
 * @defgroup GP2Y0E03_select
 * @{
 */
#define GP2Y0E03_1					0x01
#define GP2Y0E03_2					0x02
#define GP2Y0E03_3					0x04
#define GP2Y0E03_4					0x08
#define GP2Y0E03_ALL				0x0F
/**
 * @}
 */

/** 
 * @defgroup GP2Y0E03_configuration
 * @{
 */
typedef enum
{
    GP2Y0E03_Mpw_40us = 0x03,//12mA
    GP2Y0E03_Mpw_80us,//14mA
    GP2Y0E03_Mpw_160us,//18mA
    GP2Y0E03_Mpw_240us,//22mA
    GP2Y0E03_Mpw_320us,//26mA-default
}GP2Y0E03_MaxPulseWidthTypedef;


/* ┌───────────┬────────────────────────────┐
 * │中值滤波值 │         信号累积数         │
 * ├───────────┼───────┬──────┬──────┬──────┤
 * │           │   1   │  5   │  10  │  30  │
 * ├───────────┼───────┼──────┼──────┼──────┤
 * │     1     │  20ms │ 30ms │ 40ms │ 80ms │
 * ├───────────┼───────┼──────┼──────┼──────┤
 * │     5     │  27ms │ 70ms │ 120ms│ 310ms│
 * ├───────────┼───────┼──────┼──────┼──────┤
 * │     7     │  30ms │ 90ms │ 160ms│ 430ms│
 * ├───────────┼───────┼──────┼──────┼──────┤
 * │     9     │  35ms │ 110ms│ 200ms│ 550ms│
 * └───────────┴───────┴──────┴──────┴──────┘
 */
typedef enum
{
    GP2Y0E03_Sa_1time = 0x00,//20ms
    GP2Y0E03_Sa_5times,//30ms
    GP2Y0E03_Sa_30times,//80ms
    GP2Y0E03_Sa_10times,//40ms-default
}GP2Y0E03_SignalAccumulationTypedef;

typedef enum
{
    GP2Y0E03_Mf_1time = 0x30,//20ms-default
    GP2Y0E03_Mf_5times = 0x10,//30ms
    GP2Y0E03_Mf_7times = 0x00,//40ms
    GP2Y0E03_Mf_9times = 0x20,//80ms
}GP2Y0E03_MedianFilterTypedef;

typedef enum
{
    GP2Y0E03_Mod_128cm = 0x01,
    GP2Y0E03_Mod_64cm//default
}GP2Y0E03_MaxOutputDistanceTypedef;
/**
 * @}
 */

void GP2Y0E03_Init(uint8_t sensorX);
float GP2Y0E03_ReadOutDistance(uint8_t sensorX);
uint32_t GP2Y0E03_ReadOutIntensity(uint8_t sensorX);
extern inline void GP2Y0E03_SoftwareReset(uint8_t sensorX);
extern inline void GP2Y0E03_StateControlState(uint8_t sensorX, FunctionalState setActive);
extern inline void GP2Y0E03_SetMaxOutputDistance(uint8_t sensorX, GP2Y0E03_MaxOutputDistanceTypedef maxOutputDistance);
extern inline void GP2Y0E03_SetSpotSymmetryJudement(uint8_t sensorX, FunctionalState command, uint8_t spotSymmetryThreshold);
extern inline void GP2Y0E03_SetMaxSpotSizeJudement(uint8_t sensorX, FunctionalState command, uint8_t maxSpotSizeThreshold);
extern inline void GP2Y0E03_SetMinSpotSizeJudement(uint8_t sensorX, FunctionalState command, uint8_t minSpotSizeThreshold);
extern inline void GP2Y0E03_SetSignalIntensityJudement(uint8_t sensorX, FunctionalState command, uint8_t signalIntensityThreshold);
extern inline void GP2Y0E03_SetMedianFilter(uint8_t sensorX, GP2Y0E03_MedianFilterTypedef medianFilter);
extern inline void GP2Y0E03_SetSignalAccumulation(uint8_t sensorX, GP2Y0E03_SignalAccumulationTypedef signalAccumulation);
extern inline void GP2Y0E03_SetMaxPulseWidth(uint8_t sensorX, GP2Y0E03_MaxPulseWidthTypedef maxPulseWidth);

/**
 * @}
 */

#endif
