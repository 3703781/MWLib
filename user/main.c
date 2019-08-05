/**
 * @file    main.c
 * @author  Miaow
 * @date    2019/07/13
 */
#include "main.h"

int main() 
{        
  float degrees[12] = {0.0f, 30.0f, 60.0f, 90.0f, 120.0f, 150.0f, 180.0f, 
                      150.0f, 120.0f, 90.0f, 60.0f, 30.0f};
  uint8_t i = 0;
  UTILS_InitDelay();
  UTILS_InitUart(115200);
  SERVO_Init();
  printf("PCLK1:%dMHz", Apb1Clock / 1000000);

	while(1)
	{
    SERVO_SetAngle(SERVO_1 | SERVO_2 | SERVO_3 | SERVO_4, degrees[i]);
    UTILS_DelayMs(500);
    if (++i == 12)
      i = 0;
	}
}
