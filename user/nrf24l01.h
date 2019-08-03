/**
 * @file    nrf24l01.h
 * @author  Miaow
 * @version 0.1.0
 * @date    2019/08/04
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of nrf24l01:
 *          This file provides functions to manage the following functionalities:
 *              1. Initialization
 *              2. Send data
 *              3. Receive data
 * @note
 *          Minimum version of source file:
 *              0.1.0
 *          Pin connection:
 *          ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 *          ©¦     PB3©À©¤©¤©¤©¤©¤©ÈSCK     ©¦
 *          ©¦     PB4©À©¤©¤©¤©¤©¤©ÈMISO    ©¦
 *          ©¦     PB5©À©¤©¤©¤©¤©¤©ÈMOSI    ©¦
 *          ©¦     PB6©À©¤©¤©¤©¤©¤©ÈCE      ©¦
 *          ©¦     PB7©À©¤©¤©¤©¤©¤©ÈCS      ©¦
 *          ©¦     PB8©À©¤©¤©¤©¤©¤©ÈIRQ     ©¦
 *          ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *          STM32F407       NRF24L01
 *          
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
 
#ifndef __NRF24L01_H
#define __NRF24L01_H	 		  
#include "stm32f4xx.h"

/** 
 * @defgroup NRF24L01
 * @{
 */

/** 
 * @defgroup NRF24L01_configuration
 * @{
 */
#define NRF24L01_TX_PLOAD_WIDTH               32
#define NRF24L01_RX_PLOAD_WIDTH               32
#define NRF24L01_TX_ADDRESS                   0x6A,0x03,0xB4,0x29,0xCD
#define NRF24L01_RX_ADDRESS                   NRF24L01_TX_ADDRESS
/**
 * @}
 */
 
/** 
 * @defgroup NRF24L01_gpio_define
 * @{
 */
#define NRF24L01_SCK_PORT                     GPIOB
#define NRF24L01_SCK_PIN                      GPIO_Pin_3
#define NRF24L01_SCK_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define NRF24L01_SCK_GPIO_PINSOURCE           GPIO_PinSource3
#define NRF24L01_MISO_PORT                    GPIOB
#define NRF24L01_MISO_PIN                     GPIO_Pin_4
#define NRF24L01_MISO_GPIO_CLK                RCC_AHB1Periph_GPIOB
#define NRF24L01_MISO_GPIO_PINSOURCE          GPIO_PinSource4
#define NRF24L01_MOSI_PORT                    GPIOB
#define NRF24L01_MOSI_PIN                     GPIO_Pin_5
#define NRF24L01_MOSI_GPIO_CLK                RCC_AHB1Periph_GPIOB
#define NRF24L01_MOSI_GPIO_PINSOURCE          GPIO_PinSource5
#define NRF24L01_CE_PORT                      GPIOB
#define NRF24L01_CE_PIN                       GPIO_Pin_6
#define NRF24L01_CE_GPIO_CLK                  RCC_AHB1Periph_GPIOB
#define NRF24L01_CS_PORT                      GPIOB
#define NRF24L01_CS_PIN                       GPIO_Pin_7
#define NRF24L01_CS_GPIO_CLK                  RCC_AHB1Periph_GPIOB
#define NRF24L01_IRQ_PORT                     GPIOB
#define NRF24L01_IRQ_PIN                      GPIO_Pin_8
#define NRF24L01_IRQ_GPIO_CLK                 RCC_AHB1Periph_GPIOB

#define NRF24L01_GPIO_AF                      GPIO_AF_SPI1
/**
 * @}
 */

/** 
 * @defgroup NRF24L01_spi_define
 * @{
 */
#define NRF24L01_SPI                          SPI1
#define NRF24L01_CLK_FUNC                     RCC_APB2PeriphClockCmd
#define NRF24L01_SPI_CLK                      RCC_APB2Periph_SPI1
/**
 * @}
 */


typedef enum
{
  NRF24L01_SendMode,
  NRF24L01_ReceiveMode,
  NRF24L01_None,
} NRF24L01_Mode;

typedef enum
{
  NRF24L01_Ok = 0,
  NRF24L01_MaxSendError,
  NRF24L01_CheckError,
  NRF24L01_ReceiveError,
  NRF24L01_UnknownError
} NRF24L01_Status;


void NRF24L01_Init(NRF24L01_Mode mode);
uint8_t NRF24L01_Check(void);
void NRF24L01_SetMode(NRF24L01_Mode mode);
NRF24L01_Status NRF24L01_Send(uint8_t* data);
NRF24L01_Status NRF24L01_Receive(uint8_t* data);

/**
 * @}
 */
#endif
