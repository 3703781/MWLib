/**
 * @file    pms7003.c
 * @author  Miaow
 * @version 0.1.0
 * @date    2020/04/09
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of pms7003:
 *              1. Initialization
 *              2. Measurement
 * @note
 *          Minimum version of header file:
 *              0.1.0
 *          Pin connection:
 *               ©°©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©´
 *               ©¦    PA3©À©¤©¤©¤©¤©¤©ÈTX     ©¦
 *               ©¦    PA2©À©¤©¤©¤©¤©¤©ÈRX     ©¦
 *               ©¸©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¼
 *               STM32F407      PMS7003 
 *  
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
#include "pms7003.h"
#include "utils.h" 

/** @addtogroup PMS7003
 * @{
 */
 
#define PMS7003_WAIT_UART_BUSY() while((PMS7003_USARTX->SR & 0X40) == 0)
static uint8_t PMS7003_Command[] = {0x42, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x00};

/**
 * @brief Initialization.
 */
void PMS7003_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_AHB1PeriphClockCmd(PMS7003_ALL_GPIO_CLK, ENABLE);
  PMS7003_USART_CLK_FUNC(PMS7003_USART_CLK, ENABLE);  // usart clock
  GPIO_PinAFConfig(PMS7003_TX_PORT, PMS7003_TX_GPIO_PINSOURCE, PMS7003_TX_GPIO_AF);
  GPIO_PinAFConfig(PMS7003_RX_PORT, PMS7003_RX_GPIO_PINSOURCE, PMS7003_RX_GPIO_AF);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = PMS7003_TX_PIN;
  GPIO_Init(PMS7003_TX_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = PMS7003_RX_PIN;
  GPIO_Init(PMS7003_RX_PORT, &GPIO_InitStructure);
  
  USART_InitStructure.USART_BaudRate = 9600;  // baudrate
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;  // 8 bits
  USART_InitStructure.USART_StopBits = USART_StopBits_1;  // 1 stop bit
  USART_InitStructure.USART_Parity = USART_Parity_No;  // no parity
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // no flow control
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  // enable tx and rx
  USART_OverSampling8Cmd(PMS7003_USARTX, ENABLE);  // over sampling
  USART_Init(PMS7003_USARTX, &USART_InitStructure);  // init usart
  USART_Cmd(PMS7003_USARTX, ENABLE);  // enable usart
  
  USART_ClearFlag(PMS7003_USARTX, USART_FLAG_TC);
  USART_ClearFlag(PMS7003_USARTX, USART_FLAG_RXNE);
  PMS7003_SetMode(PMS7003_MODE_NEGATIVE, PMS7003_STATUS_RUN);
  USART_ClearFlag(PMS7003_USARTX, USART_FLAG_TC);
  USART_ClearFlag(PMS7003_USARTX, USART_FLAG_RXNE);
}

/**
 * @brief Set mode and status of the sensor.
 * @param mode PMS7003_MODE_NEGATIVE or PMS7003_MODE_POSITIVE.
 * @param status PMS7003_STATUS_STANDBY or PMS7003_STATUS_RUN.
 */
void PMS7003_SetMode(uint8_t mode, uint8_t state)
{
  uint8_t i;
  uint16_t tmp = 0;
  
  PMS7003_Command[2] = 0xE1;
  PMS7003_Command[4] = mode;
  for (i = 0; i < 5; i++)
    tmp += PMS7003_Command[i];
  PMS7003_Command[5] = (uint8_t)(tmp >> 8);
  PMS7003_Command[6] = (uint8_t)tmp;
  tmp = 0;
  for (i = 0; i < 7; i++)
  {
    PMS7003_USARTX->DR = PMS7003_Command[i];
    PMS7003_WAIT_UART_BUSY();
  }
  
  PMS7003_Command[2] = 0xE2;
  PMS7003_Command[4] = state;
  for (i = 0; i < 5; i++)
    tmp += PMS7003_Command[i];
  PMS7003_Command[5] = (uint8_t)(tmp >> 8);
  PMS7003_Command[6] = (uint8_t)tmp;
  for (i = 0; i < 7; i++)
  {
    PMS7003_USARTX->DR = PMS7003_Command[i];
    PMS7003_WAIT_UART_BUSY();
  }
}

/**
 * @brief Perform measurement.
 * @param result Measured data.
 * @return 0 success, otherwise error code.
 */
uint8_t PMS7003_Measure(PMS7003_ResultTypedef* result)
{
  uint8_t i;
  __IO uint16_t tmp = 0;
  uint8_t tmpResult[32];
  PMS7003_Command[2] = 0xE2;
  for (i = 0; i < 5; i++)
    tmp += PMS7003_Command[i];
  PMS7003_Command[5] = (uint8_t)(tmp >> 8);
  PMS7003_Command[6] = (uint8_t)tmp;
  tmp = 0;
  for (i = 0; i < 7; i++)
  {
    PMS7003_USARTX->DR = PMS7003_Command[i];
    PMS7003_WAIT_UART_BUSY();
  }
  
  i = 0;
  while (++tmp < 65535 && i < 32)
  {
    if (PMS7003_USARTX->SR & USART_FLAG_RXNE)
    {
      tmpResult[i++] = (uint8_t)(PMS7003_USARTX->DR);
      PMS7003_USARTX->SR &= ~USART_FLAG_RXNE;
    }
  }
  if (tmp >= 65535)
    return 1;
  if (tmpResult[0] != 0x42 || tmpResult[1] != 0x4D)
    return 2;
  if ((((uint16_t)tmpResult[2] << 8) | tmpResult[3]) != 28)
    return 3;
  tmp = 0;
  for (i = 0; i < 30; i++)
    tmp += tmpResult[i];
  if (tmp != (((uint16_t)tmpResult[30] << 8) | tmpResult[31]))
    return 4;
  
  result->PM1_0_STD = ((uint16_t)tmpResult[4] << 8) | tmpResult[5];
  result->PM2_5_STD = ((uint16_t)tmpResult[6] << 8) | tmpResult[7];
  result->PM10_STD = ((uint16_t)tmpResult[8] << 8) | tmpResult[9];
  result->PM1_0_ATM = ((uint16_t)tmpResult[10] << 8) | tmpResult[11];
  result->PM2_5_ATM = ((uint16_t)tmpResult[12] << 8) | tmpResult[13];
  result->PM10_ATM = ((uint16_t)tmpResult[14] << 8) | tmpResult[15];
  result->NUM_0_3 = ((uint16_t)tmpResult[16] << 8) | tmpResult[17];
  result->NUM_0_5 = ((uint16_t)tmpResult[18] << 8) | tmpResult[19];
  result->NUM_1_0 = ((uint16_t)tmpResult[20] << 8) | tmpResult[21];
  result->NUM_2_5 = ((uint16_t)tmpResult[22] << 8) | tmpResult[23];
  result->NUM_5_0 = ((uint16_t)tmpResult[24] << 8) | tmpResult[25];
  result->NUM_10 = ((uint16_t)tmpResult[26] << 8) | tmpResult[27];
  
  return 0;
}
/**
 * @}
 */
