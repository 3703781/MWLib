/**
 * @file    utils.h
 * @author  Alientek, Miaow
 * @version 1.0.0
 * @date    2020/04/28
 * @brief   
 *          This file provides utilities:
 *              1. Delay functions
 *              2. Serialport on UART1. Functions from stdio.h are avaliable.
 * @note
 *          Minimum version of source file:
 *              1.0.0
 *
 *          Pin connection of serial port:
 *            ��������������
 *            ��  PA9������������TXD
 *            �� PA10������������RXD
 *            ��������������
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
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "stm32f4xx.h"


typedef struct
{
  uint8_t Hours;
  uint8_t Minutes;
  uint8_t Seconds;
  uint8_t WeekDay;
  uint8_t Month;
  uint8_t Date;
  uint16_t Year;
} UTILS_DateTimeTypeDef;

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

extern uint8_t USART_RX_BUF[UTILS_RECEIEVE_LENTH]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART_RX_STA; //����״̬���

void UTILS_UpdateClocks(void);
void UTILS_InitUart(uint32_t baudrate);
void UTILS_InitDelay(void);
void UTILS_InitDateTime(const char* dateTimeString, FunctionalState forceInitialize);
void UTILS_GetDateTime(UTILS_DateTimeTypeDef* dateTime);
int32_t UTILS_GetDateTimeString(char* dateTimeString);
void UTILS_DelayUs(uint32_t time);
void UTILS_DelayMs(uint16_t time);
uint8_t UTILS_ByteToBcd2(uint8_t value);
uint8_t UTILS_Bcd2ToByte(uint8_t value);

/**
 * @}
 */
#endif
