/**
 * @file    ec11.h
 * @author  Miaow
 * @version 0.1.0
 * @date    2019/07/01
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of rotary encoder EC11:
 *              1. Initialization
 *              2. Get rotate positon and direction
 *              3. Provide callback function for changes in position.
 * @note
 *          Minimum version of source file:
 *              0.1.0
 *          Pin connection:
 *             ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©´ 
 *             ©¦  PHASEA©À©¤©¤©¤©¤©¤©ÈPB4  ©¦
 *             ©¦  PHASEB©À©¤©¤©¤©¤©¤©ÈPB5  ©¦
 *             ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¼ 
 *            EC11 encoder   STM32F407
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#ifndef __EC11_H
#define __EC11_H

#include "stm32f4xx.h"
#include "utils.h"

/** 
 * @defgroup EC11
 * @brief EC11 driver modules
 * @{
 */

#define EC11_USE_CALLBACK           1 //!< enable or disable callback

/** 
 * @defgroup EC11_gpio_define
 * @{
 */
#define EC11_GPIO_AF                GPIO_AF_TIM3
#define EC11_PHASEA_PORT            GPIOB
#define EC11_PHASEA_PIN             GPIO_Pin_4
#define EC11_PHASEA_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define EC11_PHASEA_GPIO_PINSOURCE  GPIO_PinSource4
#define EC11_PHASEB_PORT            GPIOB
#define EC11_PHASEB_PIN             GPIO_Pin_5
#define EC11_PHASEB_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define EC11_PHASEB_GPIO_PINSOURCE  GPIO_PinSource5
#define EC11_KEY_PORT               GPIOE
#define EC11_KEY_PIN                GPIO_Pin_3
#define EC11_KEY_GPIO_CLK           RCC_AHB1Periph_GPIOE
#define EC11_KEY_GPIO_PINSOURCE     GPIO_PinSource3
#define EC11_GPIO_CLK_ALL          (EC11_PHASEA_GPIO_CLK | EC11_PHASEB_GPIO_CLK | EC11_KEY_GPIO_CLK)
/**
 * @}
 */

/** 
 * @defgroup EC11_exti_define
 * @{
 */
#define EC11_KEY_EXTI_LINE          EXTI_Line3
#define EC11_KEY_IRQ_CHANNEL        EXTI3_IRQn
#define EC11_KEY_IRQ_HANDLER        EXTI3_IRQHandler
/**
 * @}
 */

/** 
 * @defgroup EC11_timer_define
 * @{
 */
#define EC11_TIM                  TIM3
#define EC11_TIM_CLK              RCC_APB1Periph_TIM3
#define EC11_TIM_IRQ_CHANNEL      TIM3_IRQn
#define EC11_TIM_IRQ_HANDLER      TIM3_IRQHandler
/**
 * @}
 */



/**
 * @brief To indicate rotate direction of ec11 encoder.
 */
typedef enum
{
  EC11_CW = 0, //!< Clockwise
  EC11_CCW = 1 //!< Anticlockwise
}EC11_DirectionTypedef;

/**
 * @brief Callback funtion prototype.
 * @param position The current position of the encoder.
 * @param direction Rotate direction, see @ref EC11_DirectionTypedef.
 */
typedef void (*EC11_RefreshHandler)(int32_t position, EC11_DirectionTypedef direction); //!< Callback funtion prototype.

typedef void (*EC11_KeyHandler)(void); //!< Callback funtion prototype.

#if EC11_USE_CALLBACK == 1
void EC11_Init(EC11_RefreshHandler refreshHandler, EC11_KeyHandler keyHandler);
#else
void EC11_Init(void);
#endif
int32_t EC11_GetPosition(void);

void EC11_ClearPosition(void);
/**
 * @}
 */

#endif
