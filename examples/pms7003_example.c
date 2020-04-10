/**
 * @file    pms7003_example.c
 * @author  Miaow
 * @date    2020/04/09
 */
 
#include "stm32f4xx.h"
#include "utils.h"
#include "pms7003.h"

PMS7003_ResultTypedef result;

/**
 * @brief entry~
 */
int main(void)
{
  UTILS_InitDelay();
  UTILS_InitUart(115200);
  PMS7003_Init();
  while (1)
  {
    if (PMS7003_Measure(&result) == 0)  // Measure succeed.
    {
      printf("PM1.0=%dug/m3\r\n", (int32_t)result.PM1_0_ATM);
      printf("PM2.5=%dug/m3\r\n", (int32_t)result.PM2_5_ATM);
      printf("Number of particles with diameter greater than 0.3um in 0.1L air=%dug/m3\r\n", (int32_t)result.NUM_0_3);
      printf("Number of particles with diameter greater than 5.0um in 0.1L air=%dug/m3\r\n", (int32_t)result.NUM_5_0);
    }
    
    UTILS_DelayMs(500);
  }
}
