/**
 * @file    utils.h
 * @author  Alientek, Miaow
 * @version 0.2.0
 * @date    2019/07/22
 * @brief   
 *          This file provides utilities:
 *              1. Delay functions
 *              2. Serialport on UART1. Functions from stdio.h are avaliable.
 * @note
 *          Minimum version of source file:
 *              0.2.0
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

#ifndef __UTILS_H
#define __UTILS_H

#include "stdio.h"
#include "stm32f4xx.h"

/** 
 * @defgroup UTILS
 * @brief Tool functions
 * @{
 */
 
/** 
* @defgroup UTILS_configuration
* @{
*/
#define UTILS_RECEIEVE_LENTH       200 //!< Maximum size of receieving buffer in bytes
#define UTILS_USART_RX_ENABLE      1 //!< Can serial port receive
/**
 * @}
 */
 
extern uint32_t AhbClock;
extern uint32_t Apb1Clock;
extern uint32_t Apb2Clock;

extern uint8_t USART_RX_BUF[UTILS_RECEIEVE_LENTH]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern uint16_t USART_RX_STA; //接收状态标记

void UTILS_UpdateClocks(void);
void UTILS_InitUart(uint32_t baudrate);
void UTILS_InitDelay(void);
void UTILS_DelayUs(uint32_t time);
void UTILS_DelayMs(uint16_t time);

/**
 * @}
 */
#endif
