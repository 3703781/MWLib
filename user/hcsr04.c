/**
 * @file    hcsr04.c
 * @author  Miaow
 * @version 0.2.0
 * @date    2019/08/05
 * @brief
 *          This file provides functions to manage the following
 *          functionalities of GP2Y0E03, the active optical distance measurement sensor:
 *              1. Initialization
 *              2. Configurations
 *              3. Distance measurement
 * @note
 *          Minimum version of header file:
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

#include "hcsr04.h"
#include "utils.h"

/** @addtogroup HCSR04
 * @{
 */
#if HCSR04_NUMBER > 4 || HCSR04_NUMBER < 0
#error "Please select HCSR04_NUMBER from 1, 2, 3, 4."
#endif
static uint32_t period = 0;

/**
 * @brief Initialize sensor.
 */
void HCSR04_Init()
{
  GPIO_InitTypeDef GPIO_InitStructer;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;

  RCC_AHB1PeriphClockCmd(HCSR04_ALL_GPIO_CLK, ENABLE);
  HCSR04_TIMER_CLKFUN(HCSR04_TIMER_CLK, ENABLE);

  GPIO_InitStructer.GPIO_Speed = GPIO_Fast_Speed;
  GPIO_InitStructer.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructer.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructer.GPIO_Pin = HCSR04_1_TRIG_GPIO_PIN;
  GPIO_Init(HCSR04_1_TRIG_GPIO_PORT, &GPIO_InitStructer);

#if HCSR04_NUMBER > 1
  GPIO_InitStructer.GPIO_Pin = HCSR04_2_TRIG_GPIO_PIN;
  GPIO_Init(HCSR04_2_TRIG_GPIO_PORT, &GPIO_InitStructer);
#endif

#if HCSR04_NUMBER > 2
  GPIO_InitStructer.GPIO_Pin = HCSR04_3_TRIG_GPIO_PIN;
  GPIO_Init(HCSR04_3_TRIG_GPIO_PORT, &GPIO_InitStructer);
#endif

#if HCSR04_NUMBER > 3
  GPIO_InitStructer.GPIO_Pin = HCSR04_4_TRIG_GPIO_PIN;
  GPIO_Init(HCSR04_4_TRIG_GPIO_PORT, &GPIO_InitStructer);
#endif

  GPIO_InitStructer.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructer.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructer.GPIO_Pin = HCSR04_1_ECHO_GPIO_PIN;
  GPIO_Init(HCSR04_1_ECHO_GPIO_PORT, &GPIO_InitStructer);

#if HCSR04_NUMBER > 1
  GPIO_InitStructer.GPIO_Pin = HCSR04_2_ECHO_GPIO_PIN;
  GPIO_Init(HCSR04_2_ECHO_GPIO_PORT, &GPIO_InitStructer);
#endif

#if HCSR04_NUMBER > 2
  GPIO_InitStructer.GPIO_Pin = HCSR04_3_ECHO_GPIO_PIN;
  GPIO_Init(HCSR04_3_ECHO_GPIO_PORT, &GPIO_InitStructer);
#endif

#if HCSR04_NUMBER > 3
  GPIO_InitStructer.GPIO_Pin = HCSR04_4_ECHO_GPIO_PIN;
  GPIO_Init(HCSR04_4_ECHO_GPIO_PORT, &GPIO_InitStructer);
#endif

  TIM_DeInit(HCSR04_TIMER);
  TIM_TimeBaseInitStructer.TIM_Period = UINT16_MAX;
  if (HCSR04_TIMER_CLK_VARIABLE == AhbClock)
    TIM_TimeBaseInitStructer.TIM_Prescaler = (uint16_t)((float)HCSR04_TIMER_CLK_VARIABLE / 1000000.0f) - 1; //1MHz
  else
    TIM_TimeBaseInitStructer.TIM_Prescaler = (uint16_t)((float)HCSR04_TIMER_CLK_VARIABLE / 500000.0f) - 1; //1MHz
  TIM_TimeBaseInitStructer.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStructer.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStructer.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(HCSR04_TIMER, &TIM_TimeBaseInitStructer);
  TIM_Cmd(HCSR04_TIMER, DISABLE);
  period = (uint32_t)(HCSR04_MAX_DISTANCE * 1000.0f / 17.0f);
}

/**
 * @brief Measure distance in centimeter.
 * @param sensorX where X can be 1 or 2, see @ref HCSR04_select.
 * @return Distance in centimeter, truncate at maximum value .
 */
float HCSR04_MeasureDistance(uint8_t sensorX)
{

  float length;
  __IO uint32_t i = 400;

  if (sensorX == 1)
  {
    HCSR04_1_TRIG_GPIO_PORT->BSRRL |= HCSR04_1_TRIG_GPIO_PIN; //Triggle
    while (--i);
    HCSR04_1_TRIG_GPIO_PORT->BSRRH |= HCSR04_1_TRIG_GPIO_PIN;
    while ((HCSR04_1_ECHO_GPIO_PORT->IDR & HCSR04_1_ECHO_GPIO_PIN) == RESET); //Wait for the pulse.
    HCSR04_TIMER->CR1 |= TIM_CR1_CEN; //Start count.
    while ((HCSR04_1_ECHO_GPIO_PORT->IDR & HCSR04_1_ECHO_GPIO_PIN) != RESET && HCSR04_TIMER->CNT < period); //Time out or end of the pulse.
  }
#if HCSR04_NUMBER > 1
  else if (sensorX == 2)
  {
    //The same as sensor 1
    HCSR04_2_TRIG_GPIO_PORT->BSRRL |= HCSR04_2_TRIG_GPIO_PIN;
    while (--i);
    HCSR04_2_TRIG_GPIO_PORT->BSRRH |= HCSR04_2_TRIG_GPIO_PIN;
    while ((HCSR04_1_ECHO_GPIO_PORT->IDR & HCSR04_2_ECHO_GPIO_PIN) == RESET);
    HCSR04_TIMER->CR1 |= TIM_CR1_CEN;
    while ((HCSR04_1_ECHO_GPIO_PORT->IDR & HCSR04_2_ECHO_GPIO_PIN) != RESET && HCSR04_TIMER->CNT < period);
  }
#endif

#if HCSR04_NUMBER > 2
  else if (sensorX == 3)
  {
    //The same as sensor 1
    HCSR04_3_TRIG_GPIO_PORT->BSRRL |= HCSR04_3_TRIG_GPIO_PIN;
    while (--i);
    HCSR04_3_TRIG_GPIO_PORT->BSRRH |= HCSR04_3_TRIG_GPIO_PIN;
    while ((HCSR04_3_ECHO_GPIO_PORT->IDR & HCSR04_3_ECHO_GPIO_PIN) == RESET);
    HCSR04_TIMER->CR1 |= TIM_CR1_CEN;
    while ((HCSR04_3_ECHO_GPIO_PORT->IDR & HCSR04_3_ECHO_GPIO_PIN) != RESET && HCSR04_TIMER->CNT < period);
  }
#endif

#if HCSR04_NUMBER > 3
  else if (sensorX == 4)
  {
    //The same as sensor 1
    HCSR04_4_TRIG_GPIO_PORT->BSRRL |= HCSR04_4_TRIG_GPIO_PIN;
    while (--i);
    HCSR04_4_TRIG_GPIO_PORT->BSRRH |= HCSR04_4_TRIG_GPIO_PIN;
    while ((HCSR04_4_ECHO_GPIO_PORT->IDR & HCSR04_4_ECHO_GPIO_PIN) == RESET);
    HCSR04_TIMER->CR1 |= TIM_CR1_CEN;
    while ((HCSR04_4_ECHO_GPIO_PORT->IDR & HCSR04_4_ECHO_GPIO_PIN) != RESET && HCSR04_TIMER->CNT < period);
  }
#endif

  HCSR04_TIMER->CR1 &= (uint16_t)~TIM_CR1_CEN; //Stop count.
  if (HCSR04_TIMER->CNT >= period)
  {
    HCSR04_TIMER->CNT = 0; //Clear timer count value.
    return HCSR04_MAX_DISTANCE;
  }

  length = (float)HCSR04_TIMER->CNT * 0.017f; //Calculate distance.
  HCSR04_TIMER->CNT = 0; //Clear timer count value.
  return length;
}
/**
 * @}
 */
