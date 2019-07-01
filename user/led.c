/**
 * @file    led.c
 * @author  Miaow
 * @version 0.1.0
 * @date    2018/10/28
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of LEDs on board:
 *              1. Initialization.
 *              2. Turn on or off.
 * @note     
 *           Minimum version of header file:
 *              0.1.0
 *
 *           Pin connection on pcb:
 *             ©°©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©´
 *             ©¦LED2©À©¤©¤©¤©¤©¤©ÈPA6  ©¦
 *             ©¦LED3©À©¤©¤©¤©¤©¤©ÈPA7  ©¦
 *             ©¸©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¼
 *              LEDs     STM32F407
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781/mystm32f4-devices-lib
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
 
#include "led.h"

/** @addtogroup LED
 * @{
 */

/**
 * @brief Initialize the two LEDs on board.
 * @note The LEDs are off after initialization.
 */
void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_AHB1PeriphClockCmd(LED_2_GPIO_CLK | LED_3_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED_2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_2_GPIO, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = LED_3_PIN;
  GPIO_Init(LED_3_GPIO, &GPIO_InitStructure);
  
  LED_2_OFF();
  LED_3_OFF();
}

/** 
 * @}
 */
