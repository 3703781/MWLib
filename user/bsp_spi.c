/**
 * @file    bsp_spi.c
 * @author  Miaow
 * @version 0.2.0
 * @date    2019/07/25
 * @brief   
 *          The STM32, as a slave, transfers data with a host through SPI bus.
 *          The communication protocol is documented in ./docs/基于SPI的通信协议 以树莓派为例.pdf
 *          This file provides functions to manage the following functionalities:
 *              1. Initialization
 *              2. Using the protocol to transmit data
 * @note
 *          Minimum version of header file:
 *              0.2.0
 *          Pin connection:
 *          ┌────────┐     ┌────────┐
 *          │     PA4├─────┤NSS     │
 *          │     PA5├─────┤SCK     │
 *          │     PA6├─────┤MISO    │
 *          │     PA7├─────┤MOSI    │
 *          └────────┘     └────────┘
 *          STM32F407        master
 *          
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
 
#include "bsp_spi.h"
#include "stdlib.h"

static uint8_t isSpiInitialized = 0;
static uint32_t buffer[255] = {0};

#if BSP_SPI_USE_RECEIVE_CALLBACK != 0
static BSP_SPI_ReceiveCallback __receiveCallback = NULL; //!< This variable stores callback function when position change.
#endif

#if BSP_SPI_MODE == BSP_SPI_MODE_MASTER
/**
 * @brief Initialize SPI.
 */
void BSP_SPI_Init()
#elif BSP_SPI_USE_RECEIVE_CALLBACK != 0
/**
 * @brief Initialize SPI.
 * @param receieveCallback Callback function that provides the received data.
 *        Data passed in the callback function is parsed in accordance with communication protocol.
 */
void BSP_SPI_Init(BSP_SPI_ReceiveCallback receiveCallback)
#endif
{
  if(isSpiInitialized)
    return;
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef SPI_InitStructure;
  
  #if BSP_SPI_USE_RECEIVE_CALLBACK != 0
  NVIC_InitTypeDef NVIC_InitStructure;
  __receiveCallback = receiveCallback;
  #endif
  
  //GPIO
  RCC_AHB1PeriphClockCmd(BSP_SPI_NSS_GPIO_CLK | BSP_SPI_SCK_GPIO_CLK | BSP_SPI_MISO_GPIO_CLK | BSP_SPI_MOSI_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_InitStructure.GPIO_Pin = BSP_SPI_NSS_PIN;
  GPIO_Init(BSP_SPI_NSS_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = BSP_SPI_SCK_PIN;
  GPIO_Init(BSP_SPI_SCK_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = BSP_SPI_MISO_PIN;
  GPIO_Init(BSP_SPI_MISO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = BSP_SPI_MOSI_PIN;
  GPIO_Init(BSP_SPI_MOSI_PORT, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(BSP_SPI_NSS_PORT, BSP_SPI_NSS_GPIO_PINSOURCE, BSP_SPI_GPIO_AF);
  GPIO_PinAFConfig(BSP_SPI_SCK_PORT, BSP_SPI_SCK_GPIO_PINSOURCE, BSP_SPI_GPIO_AF);
  GPIO_PinAFConfig(BSP_SPI_MISO_PORT, BSP_SPI_MISO_GPIO_PINSOURCE, BSP_SPI_GPIO_AF);
  GPIO_PinAFConfig(BSP_SPI_MOSI_PORT, BSP_SPI_MOSI_GPIO_PINSOURCE, BSP_SPI_GPIO_AF);
  
  //SPI slave
  RCC_APB2PeriphClockCmd(BSP_SPI_SPI_CLK, ENABLE);
  #if BSP_SPI_MODE == BSP_SPI_MODE_MASTER
  SPI_InitStructure.SPI_BaudRatePrescaler = BSP_SPI_BAUDRATE;
  #else
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  #endif
  SPI_InitStructure.SPI_CPHA = BSP_SPI_CPHA;
  SPI_InitStructure.SPI_CPOL = BSP_SPI_CPOL;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  #if BSP_SPI_MODE == BSP_SPI_MODE_MASTER
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  #elif BSP_SPI_MODE == BSP_SPI_MODE_SLAVE
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  #endif
  SPI_InitStructure.SPI_NSS = BSP_SPI_NSS;
  SPI_Init(BSP_SPI_SPI, &SPI_InitStructure);
  
  //SPI interrupt
  #if BSP_SPI_USE_RECEIVE_CALLBACK != 0
  SPI_I2S_ITConfig(BSP_SPI_SPI, SPI_I2S_IT_RXNE, ENABLE);
  NVIC_InitStructure.NVIC_IRQChannel = BSP_SPI_NVIC_IRQ_CHANNEL;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_Init(&NVIC_InitStructure);
  #endif
  
  //Turn on SPI
  SPI_Cmd(BSP_SPI_SPI, ENABLE);
  
  BSP_SPI_SPI->DR = 0x07; //Initial state.
  isSpiInitialized = 1;
}

#if BSP_SPI_USE_RECEIVE_CALLBACK != 0
/**
 * @brief Interrupt when data received.
 *        Accept and parse data according to communication protocol.
 *        This function also returns result to the host when each byte is transferred.
 * @note  __receiveCallback function is called when a frame of data is received.
 */
void BSP_SPI_IRQ_HANDLER()
{
  static uint8_t num = 0;
  static uint8_t stage = 0;
  uint8_t data;
  
  BSP_SPI_SPI->DR = 0x00; //Not ready.
  data = BSP_SPI_ReadByte();
  SPI_I2S_ClearITPendingBit(BSP_SPI_SPI, SPI_I2S_IT_RXNE);
  
  //stage    0000011111111111222211111111112
  //num      0000000001111222222200001112222
  //buffer   -----...-...-...-----..-..-..--   - invalid, . valid
  //data     afd><123,123,123>%2G<11,22,98>f
  if (data == '<')
  {
    num = 0;
    buffer[0] = 0;
    stage = 1;
  }
  else if (stage == 1 && data == '>')
  {
    stage = 2;
    __receiveCallback(buffer, num + 1);
  }
  else if (stage == 1)
  {
    if (data == ',')
    {
      num++;
      buffer[num] = 0;
    }
    else if (data <= '9' && data >= '0')
    {
      buffer[num] = buffer[num] * 10 + data - '0';
    }
    else
    {
      BSP_SPI_SPI->DR = 0x01;
      return;
    }
  }
  else
  {
    BSP_SPI_SPI->DR = 0x02;
    return;
  }
  BSP_SPI_SPI->DR = 0x34; //Ready.
}
#endif

/**
 * @brief Send a byte.
 * @param data The byte to send.
 */
void BSP_SPI_WriteByte(uint8_t data)
{
    while((BSP_SPI_SPI->SR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);
    BSP_SPI_SPI->DR = (uint16_t)data;
}

/**
 * @brief Read a byte.
 *        Send 0x0 meanwhile.
 * @return The byte to read.
 */
uint8_t BSP_SPI_ReadByte()
{
   while ((BSP_SPI_SPI->SR & SPI_I2S_FLAG_RXNE) == (uint16_t)RESET);
   return (uint8_t)BSP_SPI_SPI->DR;
}

/**
 * @brief Transfer a byte.
 * @param data The byte to send.
 * @return A byte that read in.
 */
uint8_t BSP_SPI_TransferByte(uint8_t data)
{
  while((BSP_SPI_SPI->SR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);
  BSP_SPI_SPI->DR = (uint16_t)data;
  while ((BSP_SPI_SPI->SR & SPI_I2S_FLAG_RXNE) == (uint16_t)RESET);
  return (uint8_t)BSP_SPI_SPI->DR;
}
