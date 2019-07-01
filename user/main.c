/**
 * @file    main.c
 * @author  Miaow
 * @date    2019/06/29
 */
#include "main.h"

/**
 * @brief entry~
 */
int main(void)
{
	UTILS_InitDelay();
  UTILS_InitUart(115200);
  LED_Init();
  KEY_Init();
  while (1)
	{
    uint8_t keyResult = KEY_Scan();
		if (keyResult == KEY_0)
      LED_2_TOGGLE();
    if (keyResult == KEY_1)
      LED_3_TOGGLE();
    if (keyResult == KEY_UP)
      LED_2_TOGGLE();
	}
}
