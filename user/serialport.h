/**
 * @file    serialport.h
 * @author  Miaow
 * @version 0.1.0
 * @date    2018/11/06
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of serial port:
 *              1. Initialization and configuration
 *              2. Command sending and analyzing
 * @note
 *          Minimum version of source file:
 *              0.1.0
 *          Recommanded pin connection:
 *              ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 *              ©¦TX(PC12)©À©¤©¤©¤©¤©¤©ÈRX      ©¦
 *              ©¦ RX(PD2)©¦©¤©¤©¤©¤©¤©ÈTX      ©¦
 *              ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
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
#ifndef __SERIALPORT_H
#define __SERIALPORT_H

#include "stm32f4xx.h"

/** 
 * @defgroup SERIALPORT
 * @brief SERIALPORT driver modules
 * @{
 */
 
/** 
 * @defgroup SERIALPORT_gpio_define
 * @{
 */
 
#define SERIALPORT_GPIO_AF                  GPIO_AF_UART5//GPIO_AF_USART1
#define SERIALPORT_TX_GPIO_CLK              RCC_AHB1Periph_GPIOC//RCC_AHB1Periph_GPIOA
#define SERIALPORT_TX_PORT                  GPIOC//GPIOA
#define SERIALPORT_TX_PIN                   GPIO_Pin_12//GPIO_Pin_9
#define SERIALPORT_TX_PINSOURCE             GPIO_PinSource12//GPIO_PinSource9

#define SERIALPORT_RX_GPIO_CLK              RCC_AHB1Periph_GPIOD//RCC_AHB1Periph_GPIOA
#define SERIALPORT_RX_PORT                  GPIOD//GPIOA
#define SERIALPORT_RX_PIN                   GPIO_Pin_2//GPIO_Pin_10
#define SERIALPORT_RX_PINSOURCE             GPIO_PinSource2//GPIO_PinSource10
/**
 * @}
 */

/** 
 * @defgroup SERIALPORT_dma_define
 * @{
 */
#define SERIALPORT_TX_DMA_CLK               RCC_AHB1Periph_DMA1//RCC_AHB1Periph_DMA2
#define SERIALPORT_TX_DMA_CHANNEL           DMA_Channel_4
#define SERIALPORT_TX_DMA_STREAM            DMA1_Stream7//DMA2_Stream7
#define SERIALPORT_TX_DMA_TCIF              DMA_FLAG_TCIF7

#define SERIALPORT_RX_DMA_CLK               RCC_AHB1Periph_DMA1//RCC_AHB1Periph_DMA2
#define SERIALPORT_RX_DMA_CHANNEL           DMA_Channel_4
#define SERIALPORT_RX_DMA_STREAM            DMA1_Stream0//DMA2_Stream2
#define SERIALPORT_RX_DMA_TCIF              DMA_FLAG_TCIF0//DMA_FLAG_TCIF2
/**
 * @}
 */
 
/** 
 * @defgroup SERIALPORT_usart_define
 * @{
 */
#define SERIALPORT_CLK                      RCC_APB1Periph_UART5//RCC_APB2Periph_USART1
#define SERIALPORT_CLK_FUNC                 RCC_APB1PeriphClockCmd//RCC_APB2PeriphClockCmd
#define SERIALPORT_USARTX                   UART5//USART1
#define SERIALPORT_BAUDRATE                 115200
#define SERIALPORT_IRQHANDLER               UART5_IRQHandler
#define SERIALPORT_IRQN                     UART5_IRQn
/**
 * @}
 */

/**
 * @defgroup SERIALPORT_identity
 * @note Choose SERIALPORT_MASTER or SERIALPORT_SLAVE
 * @{
 */
//#define SERIALPORT_MASTER
#define SERIALPORT_SLAVE
/**
 * @}
 */
#if (!defined SERIALPORT_MASTER && !defined SERIALPORT_SLAVE) || (defined SERIALPORT_MASTER && defined SERIALPORT_SLAVE)
    #error Which identity are you using?
#endif
/**
 * @brief Commands enumeration
 */
typedef enum
{
  SERIALPORT_CommandStopRun,
  SERIALPORT_CommandStartRun,
  SERIALPORT_ConnamdNone
} SERIALPORT_CommandTypedef;


void SERIALPORT_Init(void);
void SERIALPORT_SendCommand(const char* command);
SERIALPORT_CommandTypedef SERIALPORT_GetCommand(void);
#define SERIALPORT_StopRec() SERIALPORT_SendCommand("stoprec")
#define SERIALPORT_StartRec() SERIALPORT_SendCommand("startrec")
/**
 * @}
 */ 
#endif
