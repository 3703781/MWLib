/**
 * @file    main.c
 * @author  Miaow
 * @date    2019/07/13
 */
#include "main.h"

int main() 
{
  float distance;
  UTILS_InitDelay();
  UTILS_InitUart(115200);
  HCSR04_Init();

  while(1)
	{
    distance = HCSR04_MeasureDistance(HCSR04_4);
    printf("%5.2fcm\b\b\b\b\b\b\b", distance);
    UTILS_DelayMs(20);
  }
}
