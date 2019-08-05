/**
 * @file    servo_example.c
 * @author  Miaow
 * @date    2019/08/05
 */
#include "utils.h"
#include "servo.h"

int main() 
{        
  float degrees[12] = {0.0f, 30.0f, 60.0f, 90.0f, 120.0f, 150.0f, 180.0f, 
                      150.0f, 120.0f, 90.0f, 60.0f, 30.0f};
  uint8_t i = 0;
  UTILS_InitDelay();
  SERVO_Init();

  while(1)
  {
    SERVO_SetAngle(SERVO_ALL, degrees[i]);
    UTILS_DelayMs(500);
    if (++i == 12)
      i = 0;
  }
}
