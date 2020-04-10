/**
 * @file    serialport.c
 * @author  Miaow
 * @version 0.1.0
 * @date    2018/11/06
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of serial port:
 *              1. Initialization and configuration
 *              2. Command sending and analyzing
 * @note
 *          Minimum version of header file:
 *              0.1.0
 *          Recommanded pin connection:
 *              ┌────────┐     ┌────────┐
 *              │TX(PC12)├─────┤RX      │
 *              │ RX(PD2)├─────┤TX      │
 *              └────────┘     └────────┘
 *              STM32F407_1    STM32F407_2
 *             
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781/mystm32f4-devices-lib
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
#include "serialport.h"
#include "string.h"

/** @addtogroup SERIALPORT
 * @{
 */

#define SERIALPORT_WAIT_BUSY() while((SERIALPORT_USARTX->SR & 0X40) == 0)
static char RxBuffer[20] = {0};
static uint8_t RxOkFlag = 0;

/**
 * @brief Initialize serialport with dma.
 */
void SERIALPORT_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(SERIALPORT_TX_GPIO_CLK, ENABLE);//tx gpio clock
  RCC_AHB1PeriphClockCmd(SERIALPORT_RX_GPIO_CLK, ENABLE);//rx gpio clock
  SERIALPORT_CLK_FUNC(SERIALPORT_CLK, ENABLE);//usart clock

  //gpio
  GPIO_PinAFConfig(SERIALPORT_TX_PORT, SERIALPORT_TX_PINSOURCE, SERIALPORT_GPIO_AF);//alternative function
  GPIO_PinAFConfig(SERIALPORT_RX_PORT, SERIALPORT_RX_PINSOURCE, SERIALPORT_GPIO_AF);//alternative function
  GPIO_InitStructure.GPIO_Pin = SERIALPORT_TX_PIN;//tx pin
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//alternative function
  GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;//50MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//out pp mode
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//pull up
  GPIO_Init(SERIALPORT_TX_PORT, &GPIO_InitStructure);//init tx
  GPIO_InitStructure.GPIO_Pin = SERIALPORT_RX_PIN;//rx pin
  GPIO_Init(SERIALPORT_RX_PORT, &GPIO_InitStructure);//init rx

  //usart
  USART_InitStructure.USART_BaudRate = SERIALPORT_BAUDRATE;//baudrate
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8 bits
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//1 stop bit
  USART_InitStructure.USART_Parity = USART_Parity_No;//no parity
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//no flow control
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//enable tx and rx
  USART_OverSampling8Cmd(SERIALPORT_USARTX, ENABLE);//over sampling
  USART_Init(SERIALPORT_USARTX, &USART_InitStructure);//init usart
  USART_Cmd(SERIALPORT_USARTX, ENABLE);//enable usart
  USART_ITConfig(SERIALPORT_USARTX, USART_IT_RXNE, ENABLE);

  //USART1 NVIC
  NVIC_InitStructure.NVIC_IRQChannel = SERIALPORT_IRQN;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  USART_ClearFlag(SERIALPORT_USARTX, USART_FLAG_TC);
}

static inline void SERIALPORT_StartCommunication()
{
  SERIALPORT_WAIT_BUSY();
  SERIALPORT_USARTX->DR = (uint8_t)'A';
  SERIALPORT_WAIT_BUSY();
  SERIALPORT_USARTX->DR = (uint8_t)'A';
}

static inline void SERIALPORT_StopCommunication()
{
  SERIALPORT_WAIT_BUSY();
  SERIALPORT_USARTX->DR = (uint8_t)'B';
  SERIALPORT_WAIT_BUSY();
  SERIALPORT_USARTX->DR = (uint8_t)'B';
}

/**
 * @brief Send command.
 */
void SERIALPORT_SendCommand(const char* command)
{
  uint8_t i;
  const uint8_t length = strlen(command);
  SERIALPORT_StartCommunication();
  SERIALPORT_WAIT_BUSY();
  SERIALPORT_USARTX->DR = length;
  
  for (i = 0; i < length; i++)
  {
    SERIALPORT_WAIT_BUSY();
    SERIALPORT_USARTX->DR = (uint8_t)command[i];
  }
  SERIALPORT_StopCommunication();
}

/**
 * @brief Receieve and analyze command.
 * @param parameter Parameter of the receieved command.
 * @return Receieved command in @ref SERIALPORT_CommandTypedef.
 */
SERIALPORT_CommandTypedef SERIALPORT_GetCommand()
{
    if (!RxOkFlag)
      return SERIALPORT_ConnamdNone;
    RxOkFlag = 0;
    if (strcmp("stoprun", RxBuffer) == 0)
      return SERIALPORT_CommandStopRun;
    if (strcmp("startrun", RxBuffer) == 0)
      return SERIALPORT_CommandStartRun;
    return SERIALPORT_ConnamdNone;
}

void SERIALPORT_IRQHANDLER()
{
  uint8_t received = SERIALPORT_USARTX->DR & (uint16_t)0x01FF;
  static uint8_t numOfA = 0;
  static uint8_t length = 0;
  static uint8_t i = 0;
  
  
  if (length) //命令还没接收完毕
  {
    length--;
    RxBuffer[i++] = received; //存储接收到的一字节数据
    
    if (length == 0)
    {
      RxBuffer[i] = 0; //字符串最后要以0结尾
      RxOkFlag = 1;
    }
  }

  if (numOfA == 2) //收到过两个A了，这次收到的就是字节数
    length = received;
  
  if (received == 'A')
  {
    if (numOfA == 0) //第一次收到A
    {
      i = 0;
      RxOkFlag = 0;
    }
    numOfA++;
  }
  else //只要不是A，就把A的数量清零
  {
    numOfA = 0;
  }
}

/**
 * @}
 */
