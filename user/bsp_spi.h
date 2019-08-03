/**
 * @file    bsp_spi.h
 * @author  Miaow
 * @version 0.2.0
 * @date    2019/07/25
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of nrf24l01:
 *          This file provides functions to manage the following functionalities:
 *              1. Initialization
 *              2. Send data
 *              3. Receive data
 * @note
 *          Minimum version of source file:
 *              0.2.0
 *          Pin connection:
 *          ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 *          ©¦     PA4©À©¤©¤©¤©¤©¤©ÈNSS     ©¦
 *          ©¦     PA5©À©¤©¤©¤©¤©¤©ÈSCK     ©¦
 *          ©¦     PA6©À©¤©¤©¤©¤©¤©ÈMISO    ©¦
 *          ©¦     PA7©À©¤©¤©¤©¤©¤©ÈMOSI    ©¦
 *          ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
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
 
#ifndef __BSP_SPI_H
#define __BSP_SPI_H
#include "stm32f4xx.h"

/** 
 * @defgroup BSP_SPI
 * @{
 */
 
/** 
 * @defgroup BSP_SPI_gpio_define
 * @{
 */
#define BSP_SPI_NSS_PORT                      GPIOA
#define BSP_SPI_NSS_PIN                       GPIO_Pin_4
#define BSP_SPI_NSS_GPIO_CLK                  RCC_AHB1Periph_GPIOA
#define BSP_SPI_NSS_GPIO_PINSOURCE            GPIO_PinSource4
#define BSP_SPI_SCK_PORT                      GPIOA
#define BSP_SPI_SCK_PIN                       GPIO_Pin_5
#define BSP_SPI_SCK_GPIO_CLK                  RCC_AHB1Periph_GPIOA
#define BSP_SPI_SCK_GPIO_PINSOURCE            GPIO_PinSource5
#define BSP_SPI_MISO_PORT                     GPIOA
#define BSP_SPI_MISO_PIN                      GPIO_Pin_6
#define BSP_SPI_MISO_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define BSP_SPI_MISO_GPIO_PINSOURCE           GPIO_PinSource6
#define BSP_SPI_MOSI_PORT                     GPIOA
#define BSP_SPI_MOSI_PIN                      GPIO_Pin_7
#define BSP_SPI_MOSI_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define BSP_SPI_MOSI_GPIO_PINSOURCE           GPIO_PinSource7

#define BSP_SPI_GPIO_AF                       GPIO_AF_SPI1
/**
 * @}
 */

//DO NOT modify BSP_SPI_MODE_MASTER and BSP_SPI_MODE_SLAVE.
//Use BSP_SPI_MODE to select roles.
#define BSP_SPI_MODE_MASTER                   1
#define BSP_SPI_MODE_SLAVE                    2

/** 
 * @defgroup BSP_SPI_bus_define
 * @{
 */
#define BSP_SPI_CLK_FUNC                      RCC_APB2PeriphClockCmd
#define BSP_SPI_SPI                           SPI1
#define BSP_SPI_SPI_CLK                       RCC_APB2Periph_SPI1
#define BSP_SPI_MODE                          BSP_SPI_MODE_SLAVE
#define BSP_SPI_CPHA                          SPI_CPHA_2Edge
#define BSP_SPI_CPOL                          SPI_CPOL_Low
#define BSP_SPI_NSS                           SPI_NSS_Hard
#if BSP_SPI_MODE == BSP_SPI_MODE_MASTER
#define BSP_SPI_BAUDRATE                      SPI_BaudRatePrescaler_2 //Apb2Clock / 2 = 42MHz
                                              //SPI_BaudRatePrescaler_4 //21MHz
                                              //SPI_BaudRatePrescaler_8 // 10.5MHz
                                              //SPI_BaudRatePrescaler_16 //5.25MHz
                                              //SPI_BaudRatePrescaler_32 //2.625MHz
                                              //SPI_BaudRatePrescaler_64 //1.3125MHz
                                              //SPI_BaudRatePrescaler_128 //656.25kHz
 
                                              //SPI_BaudRatePrescaler_256 //328.125kHz
#elif BSP_SPI_MODE == BSP_SPI_MODE_SLAVE
#define BSP_SPI_USE_RECEIVE_CALLBACK          1 //!< enable or disable callback

#if BSP_SPI_USE_RECEIVE_CALLBACK == 1
#define BSP_SPI_NVIC_IRQ_CHANNEL              SPI1_IRQn
#define BSP_SPI_IRQ_HANDLER                   SPI1_IRQHandler
#endif

#else
#error "Please select SPI mode used in your application (in bsp_spi.h file)"
#endif
/**
 * @}
 */



#if BSP_SPI_USE_RECEIVE_CALLBACK != 0
/**
 * @brief Callback funtion prototype.
 *        When a frame of data is received, this function is called by library.
 *        Data array passed in the this function is parsed in accordance with the protocol.
 * @param data The parsed array.
 * @param length Number of elements in the array.
 */
typedef void (*BSP_SPI_ReceiveCallback)(uint32_t* data, uint8_t length); //!< Callback funtion prototype.
#endif


#if BSP_SPI_USE_RECEIVE_CALLBACK == 0
void BSP_SPI_Init(void);
#else
void BSP_SPI_Init(BSP_SPI_ReceiveCallback receiveCallback);
#endif
void BSP_SPI_WriteByte(uint8_t data);
uint8_t BSP_SPI_ReadByte(void);
uint8_t BSP_SPI_TransferByte(uint8_t data);
/**
 * @}
 */
#endif
