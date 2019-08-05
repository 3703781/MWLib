/**
 * @file    hcsr04.h
 * @author  Miaow
 * @version 0.2.0
 * @date    2019/08/05
 * @brief
 *          This file provides functions to manage the following
 *          functionalities of HCSR04, the distance measurement sensor:
 *              1. Initialization
 *              2. Configurations
 *              3. Distance measurement
 * @note
 *          Minimum version of source file:
 *              0.2.0
 *          Pin connection:
 *          ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 *          ©¦    TRIG©À©¤©¤©¤©¤©¤©ÈPE5  PE2©À©¤©¤©¤©¤©¤©ÈTRIG    ©¦
 *          ©¦    ECHO©À©¤©¤©¤©¤©¤©ÈPE3  PE4©À©¤©¤©¤©¤©¤©ÈECHO    ©¦
 *          ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¦        ©¦     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *           HCSR04_1      ©¦        ©¦      HCSR04_2
 *          ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©¦        ©¦     ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 *          ©¦    TRIG©À©¤©¤©¤©¤©¤©ÈPE7  PE9©À©¤©¤©¤©¤©¤©ÈTRIG    ©¦
 *          ©¦    ECHO©À©¤©¤©¤©¤©¤©ÈPE6  PE8©À©¤©¤©¤©¤©¤©ÈECHO    ©¦
 *          ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *           HCSR04_3      STM32F407       HCSR04_4
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#ifndef __HCSR04_H
#define __HCSR04_H

#include "stdint.h"

/**
 * @defgroup HCSR04
 * @{
 */

/**
 * @defgroup HCSR04_configuration
 * @{
 */
#define HCSR04_NUMBER                  4 //!< Number of servo in use, this macro can be 1, 2, 3 and 4.
#define HCSR04_MAX_DISTANCE            64.0f //!< Maximum distance.
/**
 * @}
 */

/**
 * @defgroup HCSR04_gpio_define
 * @brief GPIO configuration of each HCSR04.
 * @note GPIOs will be initialized according to the number
 *       of sensor used, see @ref HCSR04_configuration.
 * @{
 */
#define HCSR04_1_ECHO_GPIO_CLK       RCC_AHB1Periph_GPIOE
#define HCSR04_1_ECHO_GPIO_PORT      GPIOE
#define HCSR04_1_ECHO_GPIO_PIN       GPIO_Pin_3

#define HCSR04_1_TRIG_GPIO_CLK       RCC_AHB1Periph_GPIOE
#define HCSR04_1_TRIG_GPIO_PORT      GPIOE
#define HCSR04_1_TRIG_GPIO_PIN       GPIO_Pin_5

#define HCSR04_2_ECHO_GPIO_CLK       RCC_AHB1Periph_GPIOE
#define HCSR04_2_ECHO_GPIO_PORT      GPIOE
#define HCSR04_2_ECHO_GPIO_PIN       GPIO_Pin_4

#define HCSR04_2_TRIG_GPIO_CLK       RCC_AHB1Periph_GPIOE
#define HCSR04_2_TRIG_GPIO_PORT      GPIOE
#define HCSR04_2_TRIG_GPIO_PIN       GPIO_Pin_2

#define HCSR04_3_ECHO_GPIO_CLK       RCC_AHB1Periph_GPIOE
#define HCSR04_3_ECHO_GPIO_PORT      GPIOE
#define HCSR04_3_ECHO_GPIO_PIN       GPIO_Pin_6

#define HCSR04_3_TRIG_GPIO_CLK       RCC_AHB1Periph_GPIOE
#define HCSR04_3_TRIG_GPIO_PORT      GPIOE
#define HCSR04_3_TRIG_GPIO_PIN       GPIO_Pin_7

#define HCSR04_4_ECHO_GPIO_CLK       RCC_AHB1Periph_GPIOE
#define HCSR04_4_ECHO_GPIO_PORT      GPIOE
#define HCSR04_4_ECHO_GPIO_PIN       GPIO_Pin_8

#define HCSR04_4_TRIG_GPIO_CLK       RCC_AHB1Periph_GPIOE
#define HCSR04_4_TRIG_GPIO_PORT      GPIOE
#define HCSR04_4_TRIG_GPIO_PIN       GPIO_Pin_9

#define HCSR04_ALL_GPIO_CLK          (HCSR04_1_ECHO_GPIO_CLK | HCSR04_1_TRIG_GPIO_CLK | HCSR04_2_ECHO_GPIO_CLK | HCSR04_2_TRIG_GPIO_CLK |\
                                      HCSR04_3_ECHO_GPIO_CLK | HCSR04_3_TRIG_GPIO_CLK | HCSR04_4_ECHO_GPIO_CLK | HCSR04_4_TRIG_GPIO_CLK)
/**
 * @}
 */

/**
 * @defgroup HCSR04_timer_define
 * @{
 */
#define HCSR04_TIMER_CLKFUN          RCC_APB1PeriphClockCmd
#define HCSR04_TIMER_CLK             RCC_APB1Periph_TIM6
#define HCSR04_TIMER_CLK_VARIABLE    Apb1Clock
#define HCSR04_TIMER                 TIM6
/**
 * @}
 */

/**
 * @defgroup HCSR04_select
 * @{
 */
#define HCSR04_1                    1
#define HCSR04_2                    2
#define HCSR04_3                    3
#define HCSR04_4                    4
/**
 * @}
 */

void HCSR04_Init(void);
float HCSR04_MeasureDistance(uint8_t sensorX);

/**
 * @}
 */

#endif
