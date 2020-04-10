/**
 * @file    pms7003.h
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
#ifndef __PMS7003_H
#define __PMS7003_H	 
#include "stm32f4xx.h" 

/** 
 * @defgroup PMS7003
 * @brief PMS7003 Modules.
 * @{
 */

/** 
 * @defgroup PMS7003_mode
 * @{
 */
#define PMS7003_MODE_NEGATIVE         0
#define PMS7003_MODE_POSITIVE         1

#define PMS7003_STATUS_STANDBY        0
#define PMS7003_STATUS_RUN            1
/**
 * @}
 */

 /** 
 * @defgroup PMS7003_result
 * @{
 */
typedef struct {
  uint16_t PM1_0_STD;
  uint16_t PM2_5_STD;
  uint16_t PM10_STD;
  uint16_t PM1_0_ATM;
  uint16_t PM2_5_ATM;
  uint16_t PM10_ATM;
  uint16_t NUM_0_3;
  uint16_t NUM_0_5;
  uint16_t NUM_1_0;
  uint16_t NUM_2_5;
  uint16_t NUM_5_0;
  uint16_t NUM_10;
}PMS7003_ResultTypedef;
/**
 * @}
 */

/** 
 * @defgroup PMS7003_gpio_define
 * @{
 */
#define PMS7003_TX_GPIO_AF              GPIO_AF_USART2
#define PMS7003_TX_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define PMS7003_TX_PORT                 GPIOA
#define PMS7003_TX_PIN                  GPIO_Pin_3
#define PMS7003_TX_GPIO_PINSOURCE       GPIO_PinSource3

#define PMS7003_RX_GPIO_AF              GPIO_AF_USART2
#define PMS7003_RX_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define PMS7003_RX_PORT                 GPIOA
#define PMS7003_RX_PIN                  GPIO_Pin_2
#define PMS7003_RX_GPIO_PINSOURCE       GPIO_PinSource2

#define PMS7003_ALL_GPIO_CLK            (PMS7003_TX_GPIO_CLK | PMS7003_RX_GPIO_CLK)
/**
 * @}
 */


/** 
 * @defgroup PMS7003_usart_define
 * @{
 */
#define PMS7003_USART_CLK_FUNC           RCC_APB1PeriphClockCmd
#define PMS7003_USART_CLK                RCC_APB1Periph_USART2
#define PMS7003_USARTX                   USART2
/**
 * @}
 */

void PMS7003_Init(void);
void PMS7003_SetMode(uint8_t mode, uint8_t state);
uint8_t PMS7003_Measure(PMS7003_ResultTypedef* result);
/**
 * @}
 */
 
#endif


