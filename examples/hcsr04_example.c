/**
 * @file    hcsr04_example.c
 * @author  Miaow
 * @date    2019/08/05
 */
#include "utils.h"
#include "servo.h"

int main() 
{
  float distance;
  UTILS_InitDelay();
  UTILS_InitUart(115200);
  HCSR04_Init();

  while(1)
  {
    distance = HCSR04_MeasureDistance(HCSR04_1);
    printf("%.2fcm\r\n", distance);
    UTILS_DelayMs(200);
  }
}
