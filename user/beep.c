/**
 * @file    beep.c
 * @author  Miaow
 * @version 1.0.0
 * @date    2021/08/02
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of LEDs on board:
 *              1. Initialization.
 *              2. Turn on or off.
 * @note     
 *           Minimum version of header file:
 *              1.0.0
 *
 *           Pin connection on pcb:
 *             ©°©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©´
 *             ©¦BEEP©À©¤©¤©¤©¤©¤©ÈPF8  ©¦
 *             ©¸©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¼
 *              BEEP     STM32F407
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#include "beep.h"

/** @addtogroup BEEP
 * @{
 */

/**
 * @brief Initialize the BEEP on board.
 * @note The BEEP is off after initialization.
 */
void BEEP_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);
	BEEP_OFF();
}

/** 
 * @}
 */
