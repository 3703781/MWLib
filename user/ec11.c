/**
 * @file    ec11.c
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
 *          Minimum version of header file:
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
 
#include "ec11.h"

/** @addtogroup EC11
 * @{
 */

int32_t __circleCount = 0; //<! Record the number of counter overflows.
#if EC11_USE_CALLBACK == 1
static EC11_RefreshHandler __refreshHandler = NULL; //<! Callback function.
#endif

#if EC11_USE_CALLBACK == 1
/**
 * @brief Initialize EC11 encoders.
 * @param refreshHandler Callback function when the position of encoder changed.
 */
void EC11_Init(EC11_RefreshHandler refreshHandler)
#else
/**
 * @brief Initialize EC11 encoders.
 */
void EC11_Init()
#endif
{
  //Initialize GPIO after ICs to avoid going into hardfault.
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  //TIM base for encoder.
  RCC_APB1PeriphClockCmd(EC11_TIM_CLK, ENABLE);
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  #if EC11_USE_CALLBACK == 1
  TIM_TimeBaseStructure.TIM_Period = 4 - 1;
  __refreshHandler = refreshHandler;
  #else
  TIM_TimeBaseStructure.TIM_Period = 32768 - 1;
  #endif
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(EC11_TIM, &TIM_TimeBaseStructure);

  //Initialize encoder interface.
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(EC11_TIM, &TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInit(EC11_TIM, &TIM_ICInitStructure);
  TIM_EncoderInterfaceConfig(EC11_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_SetCounter(EC11_TIM, 0);

  //Interrupt.
  TIM_ClearITPendingBit(EC11_TIM, TIM_IT_Update);
  TIM_ITConfig(EC11_TIM, TIM_IT_Update, ENABLE);
  NVIC_InitStructure.NVIC_IRQChannel = EC11_TIM_IRQ_CHANNEL;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  //Initialize the GPIOs.
  RCC_AHB1PeriphClockCmd(EC11_GPIO_CLK_ALL, ENABLE);
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_InitStructure.GPIO_Pin = EC11_PHASEA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_PinAFConfig(EC11_PHASEA_PORT, EC11_PHASEA_GPIO_PINSOURCE, EC11_GPIO_AF);
  GPIO_Init(EC11_PHASEA_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = EC11_PHASEB_PIN;
  GPIO_PinAFConfig(EC11_PHASEB_PORT, EC11_PHASEB_GPIO_PINSOURCE, EC11_GPIO_AF);
  GPIO_Init(EC11_PHASEB_PORT, &GPIO_InitStructure);
  
  //Turn on the timer.
  TIM_Cmd(EC11_TIM, ENABLE);
}

/**
 * @brief Get the current position of the encoder.
 * @return The rotate position.
 */
int32_t EC11_GetPosition()
{
  #if EC11_USE_CALLBACK == 1
  return __circleCount * (int32_t)(EC11_TIM->ARR + 1) / 4;
  #else
  return (__circleCount * (int32_t)(EC11_TIM->ARR + 1) + EC11_TIM->CNT) / 4;
  #endif
}

/**
 * @brief Interrupt when the position changes.
 */
void EC11_TIM_IRQ_HANDLER()
{
  if((EC11_TIM->SR & TIM_IT_Update) != RESET)
  {
    EC11_DirectionTypedef direction;
    //Clear IT bit immediately. Why - https://blog.csdn.net/songisgood/article/details/52527242
    EC11_TIM->SR = (uint16_t)~TIM_IT_Update;
    direction = (EC11_DirectionTypedef)(EC11_TIM->CR1 >> 4 & 0x01);
    if(direction == EC11_CW)
      __circleCount++;
    else
      __circleCount--;
    
    
    #if EC11_USE_CALLBACK == 1
    if (__refreshHandler != NULL)
      __refreshHandler(EC11_GetPosition(), direction);
    #endif
  }
}
/**
 * @}
 */
