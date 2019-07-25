/**
 * @file    ec11_example.c
 * @author  Miaow
 * @date    2019/07/25
 */
 
#include "stm32f4xx.h"
#include "utils.h"
#include "stdio.h"
#include "bmp280.h"

//If forced mode is used, then umcomment USE_FORCED_MODE.
//#define USE_FORCED_MODE

/**
 * @brief Initialization.
 */
static void MAIN_Init()
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  UTILS_InitDelay();
  UTILS_InitUart(115200);
  BMP280_Init();
  //TIM6
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  TIM_TimeBaseInitStructure.TIM_Period = 1000;
  TIM_TimeBaseInitStructure.TIM_Prescaler = 10500;
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);
  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); 
  TIM_Cmd(TIM6, ENABLE);
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief Timer6 interrupt.
 */
void TIM6_DAC_IRQHandler() //0.25s
{
  if((TIM6->SR & TIM_IT_Update) != RESET)
  {
    float tempature, pressure;
    TIM6->SR = (uint16_t)~TIM_IT_Update;
	#if defined(USE_FORCED_MODE)
    BMP280_PerformMeasurement();
	#endif
    BMP280_GetTemperatureAndPressure(&pressure, &tempature);
    printf("%f,%f\r\n", tempature, pressure);
  }
}

/**
 * @brief ecvtntry~
 */
int main()
{
	MAIN_Init();
  while (1)
	{
    
	}
}