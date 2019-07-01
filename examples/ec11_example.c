/**
 * @file    ec11_example.c
 * @author  Miaow
 * @date    2019/06/29
 */
#include "stm32f4xx.h"
#include "utils.h"
#include "ec11.h"

#if EC11_USE_CALLBACK == 1
/**
 * @brief Callback funtion.
 * @param position The current position of the encoder.
 * @param direction Rotate direction, see @ref EC11_DirectionTypedef.
 */
void RefreshHandler(int32_t position, EC11_DirectionTypedef direction)
{
  printf("%d\r\n", position);
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
  EC11_Init(RefreshHandler); //Initialize with callback
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
