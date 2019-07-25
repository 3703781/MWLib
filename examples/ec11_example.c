/**
 * @file    ec11_example.c
 * @author  Miaow
 * @date    2019/07/05
 */
 
#include "stm32f4xx.h"
#include "utils.h"
#include "ec11.h"

#if EC11_USE_CALLBACK == 1
/**
 * @brief This is the function called when the the position changes.
 * @param position The current position of the encoder.
 * @param direction Rotate direction, see @ref EC11_DirectionTypedef.
 */
void RefreshHandler(int32_t position, EC11_DirectionTypedef direction)
{
  printf("position=%d\r\n", position);
}

/**
 * @brief This is the function called when the key is pressed.
 */
void KeyHandler()
{
  printf("Key pressed\r\n");
}
#endif

/**
 * @brief entry~
 */
int main(void)
{
  UTILS_InitDelay();
  UTILS_InitUart(115200);
  
  #if EC11_USE_CALLBACK == 1
  EC11_Init(RefreshHandler, KeyHandler); //Initialize with callback
  #else
  EC11_Init(); //Initialize
  #endif
  
  while (1)
  {
	#if EC11_USE_CALLBACK == 0
    printf("%d\r\n", EC11_GetPosition());
    UTILS_DelayMs(200);
	#endif
  }
}
