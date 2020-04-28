/**
 * @file    utils_example.c
 * @author  Miaow
 * @date    2020/04/28
 */
#include "utils.h"

/**
 * @brief entry~
 */
int main(void)
{
  char dateTimeString[32];
  
  // initialize delay functions.
  UTILS_InitDelay();
  
  // initialize usart1 and set baudrate 115200
  UTILS_InitUart(115200);
  
  // initialize the real time clock if not initialized
  UTILS_InitDateTime("2020-01-02 None 15:34:12", DISABLE);

  while (1)
  {
    UTILS_DateTimeTypeDef dateTime;
    
    // get the string of date and time
    // in format of "2020-01-02 15:30:00" or "2020-01-02 Thur 15:50:46"
    UTILS_GetDateTimeString(dateTimeString);
    
    // read date and time from RTC
    UTILS_GetDateTime(&dateTime);
    
    // print data and time via usart1
    printf("%s\r\n", dateTimeString);
    
    // print data and time like "20200102153000" via usart1
    printf("%04d%02d%02d%02d%02d%02d\r\n\r\n", (uint32_t)dateTime.Year, 
           (uint32_t)dateTime.Month, (uint32_t)dateTime.Date,
           (uint32_t)dateTime.Hours, (uint32_t)dateTime.Minutes, (uint32_t)dateTime.Seconds);
    
    // delay 1000ms
    UTILS_DelayMs(1000);
  }
}
