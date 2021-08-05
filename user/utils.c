/**
 * @file    utils.c
 * @author  Alientek, Miaow
 * @version 2.0.0
 * @date    2021/08/01
 * @brief
 *          This file provides utilities:
 *              1. Delay functions
 *              2. Serialport on UART1. Functions from stdio.h are avaliable.
 *              3. Real time clock functions.
 * @note
 *          Minimum version of header file:
 *              2.0.0
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
#if defined(__GNUC__)
#include "unistd.h"
#endif
#include "system_stm32f4xx.h"

/** @addtogroup UTILS
  * @{
  */

uint32_t AhbClock = 0;  //!< AHB clock in Hz.
uint32_t Apb1Clock = 0; //!< PCLK1(APB1 clock) in Hz.
uint32_t Apb2Clock = 0; //!< PCLK2(APB2 clock) in Hz.

static float fac_us = 0;
static float fac_ms = 0;

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
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
  {
    Res = USART_ReceiveData(USART1); //(USART1->DR);    //读取接收到的数据

    if ((USART_RX_STA & 0x8000) == 0) //接收未完成
    {
      if (USART_RX_STA & 0x4000) //接收到了0x0d
      {
        if (Res != 0x0a)
          USART_RX_STA = 0; //接收错误,重新开始
        else
          USART_RX_STA |= 0x8000; //接收完成了
      }
      else //还没收到0X0D
      {
        if (Res == 0x0d)
        {
          USART_RX_STA |= 0x4000;
        }
        else
        {
          USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
          USART_RX_STA++;
          if (USART_RX_STA > (UTILS_RECEIEVE_LENTH - 1))
            USART_RX_STA = 0; //接收数据错误,重新开始接收
        }
      }
    }
  }
}
#endif

/**
 * @brief Update SystemCoreClock/AhbClock/Apb1Clock/Apb2Clock variable according to Clock Register Values.
 *        The clock variables can be used by the user application to configure peripherals.
 *
 * @note  Each time the core clock changes, this function must be called.
 */
void UTILS_UpdateClocks()
{
  uint32_t ahbClockTmp = (RCC->CFGR & ((uint32_t)15 << 4));
  uint32_t apb1ClockTmp = (RCC->CFGR & ((uint32_t)7 << 10));
  uint32_t apb2ClockTmp = (RCC->CFGR & ((uint32_t)7 << 13));

  SystemCoreClockUpdate();
  switch (ahbClockTmp)
  {
  case RCC_CFGR_HPRE_DIV1:
    AhbClock = SystemCoreClock;
    break;
  case RCC_CFGR_HPRE_DIV2:
    AhbClock = SystemCoreClock >> 1;
    break;
  case RCC_CFGR_HPRE_DIV4:
    AhbClock = SystemCoreClock >> 2;
    break;
  case RCC_CFGR_HPRE_DIV8:
    AhbClock = SystemCoreClock >> 3;
    break;
  case RCC_CFGR_HPRE_DIV16:
    AhbClock = SystemCoreClock >> 4;
    break;
  case RCC_CFGR_HPRE_DIV64:
    AhbClock = SystemCoreClock >> 6;
    break;
  case RCC_CFGR_HPRE_DIV128:
    AhbClock = SystemCoreClock >> 7;
    break;
  case RCC_CFGR_HPRE_DIV256:
    AhbClock = SystemCoreClock >> 8;
    break;
  case RCC_CFGR_HPRE_DIV512:
    AhbClock = SystemCoreClock >> 9;
    break;
  default:
    AhbClock = 0;
    break;
  }

  switch (apb1ClockTmp)
  {
  case RCC_CFGR_PPRE1_DIV1:
    Apb1Clock = AhbClock;
    break;
  case RCC_CFGR_PPRE1_DIV2:
    Apb1Clock = AhbClock >> 1;
    break;
  case RCC_CFGR_PPRE1_DIV4:
    Apb1Clock = AhbClock >> 2;
    break;
  case RCC_CFGR_PPRE1_DIV8:
    Apb1Clock = AhbClock >> 3;
    break;
  case RCC_CFGR_PPRE1_DIV16:
    Apb1Clock = AhbClock >> 4;
    break;
  default:
    Apb1Clock = 0;
    break;
  }

  switch (apb2ClockTmp)
  {
  case RCC_CFGR_PPRE2_DIV1:
    Apb2Clock = AhbClock;
    break;
  case RCC_CFGR_PPRE2_DIV2:
    Apb2Clock = AhbClock >> 1;
    break;
  case RCC_CFGR_PPRE2_DIV4:
    Apb2Clock = AhbClock >> 2;
    break;
  case RCC_CFGR_PPRE2_DIV8:
    Apb2Clock = AhbClock >> 3;
    break;
  case RCC_CFGR_PPRE2_DIV16:
    Apb2Clock = AhbClock >> 4;
    break;
  default:
    Apb2Clock = 0;
    break;
  }
}

/**
 * @brief Initialize delay functions.
 */
void UTILS_InitDelay()
{
  UTILS_UpdateClocks();
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  fac_us = (float)SystemCoreClock / 8000000.0f;
  fac_ms = fac_us * 1000.0f;
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
  SysTick->LOAD = (uint32_t)((float)time * fac_us);
  SysTick->VAL = 0;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  do
  {
    temp = SysTick->CTRL;
  } while ((temp & 1) && !(temp & (1 << 16)));
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
  SysTick->LOAD = (uint32_t)((float)nms * fac_ms);
  SysTick->VAL = 0;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  do
  {
    temp = SysTick->CTRL;
  } while ((temp & 1) && !(temp & (1 << 16)));
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL = 0;
}

/**
 * @brief Delay in millisecond.
 * @param timev Time in ms.
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

#if defined(__CC_ARM)
#pragma import(__use_no_semihosting)

struct __FILE
{
  int handle;
} __stdout;

void _sys_exit(int x)
{
  x = x;
}

void _ttywrch(int ch)
{
  ch = ch;
}

/**
 * @brief Override fputc in stdlib.
 */
int fputc(int ch, FILE *f)
{
  while ((USART1->SR & 0X40) == 0)
    ;
  USART1->DR = (uint8_t)ch;
  return ch;
}
#elif defined(__GNUC__)
#include <errno.h>
#include <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO

int _write(int file, char *data, int len)
{

  if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
  {
    errno = EBADF;
    return -1;
  }
  int i = len;
  // arbitrary timeout 1000
  while (i--)
  {
    while ((USART1->SR & 0X40) == 0)
      ;
    USART1->DR = *data;
    data++;
  }

  // return # of bytes written - as best we can tell
  return len;
}
#endif

/**
 * @brief Initialize UART1.
 * @param baudrate Baudrate of communication in bps.
 */
void UTILS_InitUart(uint32_t baudrate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  //clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  //USART1 GPIO
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //USART1
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);

  USART_Cmd(USART1, ENABLE);

#if UTILS_USART_RX_ENABLE
  //USART1 receieve interrupt
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  //USART1 NVIC
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
}

/**
 * @brief  Converts a 2 digit decimal to BCD format.
 * @param  value Byte to be converted.
 * @return Converted byte
 */
uint8_t UTILS_ByteToBcd2(uint8_t value)
{
  uint8_t bcdhigh = 0;

  while (value >= 10)
  {
    bcdhigh++;
    value -= 10;
  }
  return ((uint8_t)(bcdhigh << 4) | value);
}

/**
 * @brief  Convert from 2 digit BCD to Binary.
 * @param  value BCD value to be converted.
 * @return Converted word
 */
uint8_t UTILS_Bcd2ToByte(uint8_t value)
{
  uint8_t tmp = 0;
  tmp = ((uint8_t)(value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (value & (uint8_t)0x0F));
}

/**
 * @brief  Initializes the RTC peripheral according to the date time sting.
 * @param  dateTimeString A string in format "yyyy-MM-dd ddd HH:mm:ss" or "yyyy-MM-dd dddd HH:mm:ss".
 *                       For example, "2020-04-05 Sun 17:02:00", "2020-04-05 Sunday 08:02:00".
 *                       The weekday can be omitted - "2020-04-05 None 08:30:30"
 * @param forceInitialize ENABLE - Initialize all the way.
 *                        DISABLE - Initialize only when not initialized.
 */
void UTILS_InitDateTime(const char *dateTimeString, FunctionalState forceInitialize)
{
  uint8_t i = 0, j, c1, c2;
  uint8_t weekdayTable[16] = {2, 0, 4, 0, 1, 0, 5, 0, 7, 6, 0, 0, 0, 0, 0, 3};

  RTC_InitTypeDef RTC_InitStructure;
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_DateTypeDef RTC_DateStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  PWR_BackupAccessCmd(ENABLE);
  RTC_WriteBackupRegister(RTC_BKP_DR1, (uint32_t)(dateTimeString[0] - '0') * 1000 + (uint32_t)(dateTimeString[1] - '0') * 100);

  while (dateTimeString[i++] != ' ')
    ;
  j = i;
  while (dateTimeString[j++] != ' ')
    ;
  c1 = dateTimeString[i] <= 'Z' ? dateTimeString[i] + 32 : dateTimeString[i];
  c2 = dateTimeString[i + 1] <= 'Z' ? dateTimeString[i + 1] + 32 : dateTimeString[i + 1];

  if (forceInitialize || RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x1234)
  {
    RCC_LSEConfig(RCC_LSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
      ;

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);
    RTC_InitStructure.RTC_AsynchPrediv = 127;
    RTC_InitStructure.RTC_SynchPrediv = 255;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_Init(&RTC_InitStructure);

    RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours = ((dateTimeString[j] - '0') << 4) | (dateTimeString[j + 1] - '0');
    RTC_TimeStructure.RTC_Minutes = ((dateTimeString[j + 3] - '0') << 4) | (dateTimeString[j + 4] - '0');
    RTC_TimeStructure.RTC_Seconds = ((dateTimeString[j + 6] - '0') << 4) | (dateTimeString[j + 7] - '0');
    RTC_DateStructure.RTC_Date = ((dateTimeString[8] - '0') << 4) | (dateTimeString[9] - '0');
    RTC_DateStructure.RTC_Month = ((dateTimeString[5] - '0') << 4) | (dateTimeString[6] - '0');
    RTC_DateStructure.RTC_WeekDay = weekdayTable[(uint8_t)(c1 * c2) >> 4];
    RTC_DateStructure.RTC_Year = ((dateTimeString[2] - '0') << 4) | (dateTimeString[3] - '0');
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

    RTC_WriteBackupRegister(RTC_BKP_DR0, 0x1234);
  }
}

/**
 * @brief  Get date and time from RTC.
 * @param  dateTime Pointer to a structure where the date and time from RTC is stored.
 */
void UTILS_GetDateTime(UTILS_DateTimeTypeDef *dateTime)
{
  uint32_t tmpreg = (uint32_t)(RTC->TR & 0x007F7F7F);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  PWR_BackupAccessCmd(ENABLE);

  dateTime->Hours = UTILS_Bcd2ToByte((uint8_t)((tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> 16));
  dateTime->Minutes = UTILS_Bcd2ToByte((uint8_t)((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >> 8));
  dateTime->Seconds = UTILS_Bcd2ToByte((uint8_t)(tmpreg & (RTC_TR_ST | RTC_TR_SU)));

  tmpreg = (uint32_t)(RTC->DR & 0x00FFFF3F);
  dateTime->Year = RTC_ReadBackupRegister(RTC_BKP_DR1) + (uint16_t)UTILS_Bcd2ToByte((uint8_t)((tmpreg & (RTC_DR_YT | RTC_DR_YU)) >> 16));
  dateTime->Month = UTILS_Bcd2ToByte((uint8_t)((tmpreg & (RTC_DR_MT | RTC_DR_MU)) >> 8));
  dateTime->Date = UTILS_Bcd2ToByte((uint8_t)(tmpreg & (RTC_DR_DT | RTC_DR_DU)));
  dateTime->WeekDay = UTILS_Bcd2ToByte((uint8_t)((tmpreg & (RTC_DR_WDU)) >> 13));
}

/**
 * @brief  Get the formatted string of date and time in RTC.
 *         The string looks like "yyyy-MM-dd ddd HH:mm:ss" or "yyyy-MM-dd HH:mm:ss"
 * @param  dateTimeString Pointer to a buffer where the resulting string is stored.
 *                        The buffer should be large enough to contain the resulting string.
 * @return On success, the total number of characters written is returned. 
 *         On failure, a negative number is returned.
 */
int32_t UTILS_GetDateTimeString(char *dateTimeString)
{
  static char *weekdayStrings[8] = {"", "Mon ", "Tue ", "Wed ", "Thur ", "Fri ", "Sat ", "Sun "};
  uint32_t tmpdr = (uint32_t)(RTC->DR & 0x00FFFF3F);
  uint32_t tmptr = (uint32_t)(RTC->TR & 0x007F7F7F);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  PWR_BackupAccessCmd(ENABLE);

  return sprintf(dateTimeString, "%4ld-%02x-%02x %s%02x:%02x:%02x",
                 RTC_ReadBackupRegister(RTC_BKP_DR1) + (uint32_t)UTILS_Bcd2ToByte((uint8_t)((tmpdr & (RTC_DR_YT | RTC_DR_YU)) >> 16)),
                 (uint16_t)((tmpdr & (RTC_DR_MT | RTC_DR_MU)) >> 8),
                 (uint16_t)(tmpdr & (RTC_DR_DT | RTC_DR_DU)),
                 weekdayStrings[(tmpdr & (RTC_DR_WDU)) >> 13],
                 (uint16_t)((tmptr & (RTC_TR_HT | RTC_TR_HU)) >> 16),
                 (uint16_t)((tmptr & (RTC_TR_MNT | RTC_TR_MNU)) >> 8),
                 (uint16_t)(tmptr & (RTC_TR_ST | RTC_TR_SU)));
}
/**
 * @}
 */
