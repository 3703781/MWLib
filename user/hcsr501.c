/**
 * @file    hcsr501.c
 * @author  Miaow
 * @version 1.0.0
 * @date    2021/08/01
 * @brief
 *          This file provides the following functions:
 *              1. Initialization.
 *              2. PIR motion detection.
 * @note
 *          Minimum version of header file:
 *              1.0
 *
 *          Pin connection:
 *          ��������������������     ��������������������
 *          ��     PC4��������������OUT     ��
 *          ��������������������     ��������������������
 *          STM32F407       HCSR501
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#include "hcsr501.h"

/** @addtogroup HCSR501
  * @{
  */

/**
 * @brief Initialize the sensor
 */
void HCSR501_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(HCSR501_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_Pin = HCSR501_GPIO_PIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(HCSR501_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief Get whether there is animal/human movement.
 * @return SET or RESET, SET refers movement detected.
 */
FlagStatus HCSR501_GetStatus()
{
    if (HCSR501_GPIO_PORT->IDR & HCSR501_GPIO_PIN)
    {
        return SET;
    }
    return RESET;

}

/**
 * @}
 */