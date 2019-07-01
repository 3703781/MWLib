/**
 * @file    utils.c
 * @author  Alientek, Miaow
 * @version 0.1.0
 * @date    2019/06/29
 * @brief   
 *          This file provides utilities:
 *              1. Delay functions
 *              2. Serialport on UART1. Functions from stdio.h are avaliable.
 * @note
 *          Minimum version of header file:
 *              0.1.0
 *
 *          Pin connection of serial port:
 *            ┌─────┐
 *            │  PA9├─────TXD
 *            │ PA10├─────RXD
 *            └─────┘
 *            STM32F407
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
 
#include "utils.h"
#include "system_stm32f4xx.h"

/** @addtogroup UTILS
 * @{
 */

static uint8_t fac_us = 0;
static uint16_t fac_ms = 0;
#pragma import(__use_no_semihosting)             

struct __FILE 
{ 
  int handle; 
} __stdout;

#if UTILS_USART_RX_ENABLE

//Receieving Buffer
uint8_t USART_RX_BUF[UTILS_RECEIEVE_LENTH];

//Receiving status
//bit15 - Receieved flag
//bit14 - Receieved byte 0x0D
//bit13 ~ 0 - Number of valid bytes received
uint16_t USART_RX_STA = 0;

void USART1_IRQHandler(void)
{
  uint8_t Res;
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
  {
    Res = USART_ReceiveData(USART1);//(USART1->DR);    //读取接收到的数据

    if ((USART_RX_STA & 0x8000)==0)//接收未完成
    {
      if (USART_RX_STA & 0x4000)//接收到了0x0d
      {
        if (Res != 0x0a)
          USART_RX_STA = 0;//接收错误,重新开始
        else
          USART_RX_STA |= 0x8000;    //接收完成了 
      }
      else //还没收到0X0D
      {    
        if (Res == 0x0d)
        {
          USART_RX_STA |= 0x4000;
        }
        else
        {
          USART_RX_BUF[USART_RX_STA & 0X3FFF]=Res ;
          USART_RX_STA++;
          if (USART_RX_STA > (UTILS_RECEIEVE_LENTH - 1))
            USART_RX_STA = 0;//接收数据错误,重新开始接收      
        }         
      }
    }            
  }
} 
#endif

/**
 * @brief Initialize delay functions.
 */
void UTILS_InitDelay()
{
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SystemCoreClock / 8000000;
	fac_ms = (uint16_t)fac_us * 1000;
}

/**
 * @brief Delay in microsecond.
 * @param time  Time in us. 
 *              This parameter should be within the range of [0, 2 ^ 27 / SYSCLK(Hz)].
 * @note DO NOT use in OS.
 */
void UTILS_DelayUs(uint32_t time)
{
	uint32_t temp;
	SysTick->LOAD = time * fac_us;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp = SysTick->CTRL;
	}while ((temp & 1) && !(temp & (1 << 16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL = 0;
}

/**
 * @brief Delay in millisecond.
 * @param time  Time in ms. 
 *              This parameter should be within the range of [0, 134217720000 / SYSCLK(Hz)].
 * @note DO NOT use in OS.
 */
static inline void UTILS_DelayXms(uint16_t nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD = (uint32_t)nms * fac_ms;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp=SysTick->CTRL;
	}while ((temp & 1) && !(temp & (1 << 16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL = 0;
} 

/**
 * @brief Delay in millisecond.
 * @param time  Time in ms. 
 * @note DO NOT use in OS.
 */
void UTILS_DelayMs(uint16_t time)
{	 	 
	uint8_t repeat = time / 540;
	uint16_t remain = time % 540;
	while (repeat)
	{
		UTILS_DelayXms(540);
		repeat--;
	}
	if (remain)
    UTILS_DelayXms(remain);
}

void _sys_exit(int x) 
{ 
  x = x; 
}

/**
 * @brief Override fputc in stdlib.
 */
int fputc(int ch, FILE *f)
{     
  while((USART1->SR & 0X40) == 0);
  USART1->DR = (uint8_t)ch;      
  return ch;
}

/**
 * @brief Initialize UART1.
 * @param baudrate Baudrate of communication in bps
 */
void UTILS_InitUart(uint32_t baudrate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  //clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

  //USART1 GPIO
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  //USART1
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);

  USART_Cmd(USART1, ENABLE);

  //USART_ClearFlag(USART1, USART_FLAG_TC);

  #if UTILS_USART_RX_ENABLE
  //USART1 receieve interrupt
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 

  //USART1 NVIC
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  #endif
}
/** 
 * @}
 */
