/**
 * @file    servo.c
 * @author  Miaow
 * @version 0.1.0
 * @date    2019/08/05
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of servos:
 *              1. Initialization
 *              2. Servo operation
 * @note
 *          Minimum version of header file:
 *              0.1.0
 *          Pin connection:           
 *                              ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 *                              ©¦ SERVO3 ©¦
 *                              ©¸©¤©¤©¤©Ð©¤©¤©¤©¤©¼
 *                             ©°©¤©¤©¤©¤©Ø©¤©¤©¤©¤©¤©´
 *              ©°©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©´   ©¦   PB10   ©¦   ©°©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©´
 *              ©¦  SERVO1  ©À©¤©¤©¤©ÈPA15   PB3©À©¤©¤©¤©È  SERVO2  ©¦
 *              ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼   ©¦   PB11   ©¦   ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *                    STM32F407©¸©¤©¤©¤©¤©Ð©¤©¤©¤©¤©¤©¼
 *                              ©°©¤©¤©¤©Ø©¤©¤©¤©¤©´
 *                              ©¦ SERVO4 ©¦
 *                              ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *             
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
#include "servo.h"
#include "utils.h"

/** @addtogroup SERVO
 * @{
 */
#if SERVO_NUMBER > 4 || SERVO_NUMBER < 0
#error "Please select SERVO_NUMBER from 1, 2, 3, 4."
#endif

static const float PulseTime[4][2] = { { SERVO_1_PULSE_MS }, 
                                       { SERVO_2_PULSE_MS }, 
                                       { SERVO_3_PULSE_MS }, 
                                       { SERVO_4_PULSE_MS } };

/**
 * @brief Initialize SERVO module.
 */
void SERVO_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  //GPIO
  RCC_AHB1PeriphClockCmd(SERVO_ALL_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  
  GPIO_InitStructure.GPIO_Pin = SERVO_1_PIN;
  GPIO_Init(SERVO_1_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(SERVO_1_PORT, SERVO_1_GPIO_PINSOURCE, SERVO_GPIO_AF);
  
  #if SERVO_NUMBER > 1
  GPIO_InitStructure.GPIO_Pin = SERVO_2_PIN;
  GPIO_Init(SERVO_2_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(SERVO_2_PORT, SERVO_2_GPIO_PINSOURCE, SERVO_GPIO_AF);
  #endif
  
  #if SERVO_NUMBER > 2
  GPIO_InitStructure.GPIO_Pin = SERVO_3_PIN;
  GPIO_Init(SERVO_3_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(SERVO_3_PORT, SERVO_3_GPIO_PINSOURCE, SERVO_GPIO_AF);
  #endif
  
  #if SERVO_NUMBER > 3
  GPIO_InitStructure.GPIO_Pin = SERVO_4_PIN;
  GPIO_Init(SERVO_4_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(SERVO_4_PORT, SERVO_4_GPIO_PINSOURCE, SERVO_GPIO_AF);
  #endif

  //Timer
  RCC_APB1PeriphClockCmd(SERVO_TIM_CLK, ENABLE);
  TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  if (Apb1Clock == AhbClock)
    TIM_BaseInitStructure.TIM_Prescaler = (uint16_t)((float)Apb1Clock / 50000.0f) - 1; //50kHz
  else
    TIM_BaseInitStructure.TIM_Prescaler = (uint16_t)((float)Apb1Clock / 25000.0f) - 1; //50kHz
  TIM_BaseInitStructure.TIM_Period = 50000 / 50 - 1; //=50Hz period=1000
  TIM_ARRPreloadConfig(SERVO_TIM, DISABLE); //do not buffer ARR
  TIM_TimeBaseInit(SERVO_TIM, &TIM_BaseInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //OCxREF is high when TIMx_CNT < TIMx_CCRx else high
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = (uint32_t)((float)(TIM_BaseInitStructure.TIM_Period + 1) * 0.075f);
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //output high when OCxREF is high
  
  SERVO_1_TIM_OC_PRELOAD_FUNC(SERVO_TIM, TIM_OCPreload_Disable);
  SERVO_1_TIM_OC_INIT_FUNC(SERVO_TIM, &TIM_OCInitStructure);
  
  #if SERVO_NUMBER > 1
  SERVO_2_TIM_OC_PRELOAD_FUNC(SERVO_TIM, TIM_OCPreload_Disable);
  SERVO_2_TIM_OC_INIT_FUNC(SERVO_TIM, &TIM_OCInitStructure);
  #endif
  
  #if SERVO_NUMBER > 2
  SERVO_3_TIM_OC_PRELOAD_FUNC(SERVO_TIM, TIM_OCPreload_Disable);
  SERVO_3_TIM_OC_INIT_FUNC(SERVO_TIM, &TIM_OCInitStructure);
  #endif
  
  #if SERVO_NUMBER > 3
  SERVO_4_TIM_OC_PRELOAD_FUNC(SERVO_TIM, TIM_OCPreload_Disable);
  SERVO_4_TIM_OC_INIT_FUNC(SERVO_TIM, &TIM_OCInitStructure);
  #endif
  
  TIM_Cmd(SERVO_TIM, ENABLE);
}

/**
 * @brief Set angle of the servo in degree.
 * @servoX SERVO_X where X can be (1...4) to select servo.
 *         This parameter can be any combination of @ref SERVO_select.
 * @degree Target angle in degree.
 */
void SERVO_SetAngle(uint8_t servoX, float angle)
{
  float microsecondPerStep, interceptY, slope;
  
  if (angle < 0.0f || angle > 180.0f)
    return;
  microsecondPerStep = (float)(SERVO_TIM->PSC + 1) * 500.0f / (float)Apb1Clock;
  if (Apb1Clock == AhbClock)
    microsecondPerStep *= 2.0f;
  
  if (servoX & SERVO_1)
  {
    interceptY = PulseTime[0][0] / microsecondPerStep;
    slope = (PulseTime[0][1] - PulseTime[0][0]) / microsecondPerStep / 180.0f;
    SERVO_TIM->SERVO_1_TIM_CCR = (uint32_t)(angle * slope + interceptY);
  }
  
  #if SERVO_NUMBER > 1
  if (servoX & SERVO_2)
  {
    interceptY = PulseTime[1][0] / microsecondPerStep;
    slope = (PulseTime[1][1] - PulseTime[1][0]) / microsecondPerStep / 180.0f;
    SERVO_TIM->SERVO_2_TIM_CCR = (uint32_t)(angle * slope + interceptY);
  }
  #endif
  
  #if SERVO_NUMBER > 2
  if (servoX & SERVO_3)
  {
    interceptY = PulseTime[2][0] / microsecondPerStep;
    slope = (PulseTime[2][1] - PulseTime[2][0]) / microsecondPerStep / 180.0f;
    SERVO_TIM->SERVO_3_TIM_CCR = (uint32_t)(angle * slope + interceptY);
  }
  #endif
  
  #if SERVO_NUMBER > 3
  if (servoX & SERVO_4)
  {
    interceptY = PulseTime[3][0] / microsecondPerStep;
    slope = (PulseTime[3][1] - PulseTime[3][0]) / microsecondPerStep / 180.0f;
    SERVO_TIM->SERVO_4_TIM_CCR = (uint32_t)(angle * slope + interceptY);
  }
  #endif
}

/**
 * @}
 */
