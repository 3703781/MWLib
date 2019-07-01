/**
 * @file    oled.h
 * @author  Miaow, Evk123
 * @version 0.1.0
 * @date    2018/10/05
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of 0.96" OLED display:
 *              1. Initialization
 *              2. Display formatted strings, pictures and Chinese characters
 *              3. Turn on/off the screen
 *              4. Show logs
 * @note
 *          Minimum version of source file:
 *              0.1.0
 *          Pin connection:
 *          ┌────────┐     ┌────────┐
 *          │    PC10├─────┤SCL     │
 *          │    PC11├─────┤SDA     │
 *          └────────┘     └────────┘
 *          STM32F407      0.96" OLED
 *          
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
 
#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"


#define OLED_SCL_PORT               GPIOC
#define OLED_SCL_PIN                GPIO_Pin_10
#define OLED_SCL_GPIO_CLK           RCC_AHB1Periph_GPIOC
#define OLED_SDA_PORT               GPIOC
#define OLED_SDA_PIN                GPIO_Pin_11
#define OLED_SDA_GPIO_CLK           RCC_AHB1Periph_GPIOC
#define OLED_IIC_In()                   OLED_SDA_PORT->MODER &= ~0x00C00000;OLED_SDA_PORT->MODER |= 0x00000000//输入模式
#define OLED_IIC_Out()                  OLED_SDA_PORT->MODER &= ~0x00C00000;OLED_SDA_PORT->MODER |= 0x00400000//输出模式

#define OLED_IIC_ADDRESS            0x78
#define	OLED_BRIGHTNESS             255


/**
 * @brief oled句柄.
 * @note stringX, stringY, stringClear, stringContinuous为配置项.
 */
typedef struct {
    uint8_t stringX;//字符串起始列位置(0~20)
    uint8_t stringY;//字符串起始行位置(0~7)
    FunctionalState stringClear;//是否清除上次字符串, 仅限本次OLED_DisplayFormat函数
    FunctionalState stringContinuous;//接着上次结束位置打印
    uint8_t __stringX;//当前打印字符位置
    uint8_t __stringY;//当前打印字符位置
    uint8_t __positionX;//当前显示坐标
    uint8_t __positionY;//当前显示坐标
    uint8_t __stringLastBeignX;//上次打印字符串起始位置
    uint8_t __stringLastBeignY;//上次打印字符串起始位置
    uint8_t __stringLastEndX;//上次打印字符串结束位置
    uint8_t __stringLastEndY;//上次打印字符串结束位置
    char __string[100];//实际打印的字符串
}OLED_HandleTypedef;


void OLED_Init(OLED_HandleTypedef *oledHandle);
void OLED_TurnOn(void);
void OLED_TurnOff(void);
void OLED_Clear(OLED_HandleTypedef *oledHandle);
void OLED_Blank(void);
void OLED_DisplayFormat(OLED_HandleTypedef *oledHandle, const char *format, ...);
void OLED_DisplayLog(OLED_HandleTypedef *oledHandle, const char *format, ...);

#endif 
