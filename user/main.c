/**
 * @file    ec11_example.c
 * @author  Miaow
 * @date    2019/07/05
 */
 
#include "stm32f4xx.h"
#include "utils.h"
#include "ec11.h"
#include "oled.h"
#include "si7021.h"
#include "serialport.h"

OLED_HandleTypedef OledHandle = 
{
  .stringX = 0,
  .stringY = 0,
  .stringClear = ENABLE,
};

#if EC11_USE_CALLBACK == 1
/**
 * @brief This is the function called when the the position changes.
 * @param position The current position of the encoder.
 * @param direction Rotate direction, see @ref EC11_DirectionTypedef.
 */
void RefreshHandler(int32_t position, EC11_DirectionTypedef direction)
{
  OledHandle.stringX = 0;
  OledHandle.stringY = 0;
  OledHandle.stringClear = ENABLE;
  OLED_DisplayFormat(&OledHandle, "pos=%d,dir=%d", position, direction);
}

/**
 * @brief This is the function called when the key is pressed.
 */
void KeyHandler()
{
  static uint32_t count = 0;
  OledHandle.stringX = 0;
  OledHandle.stringY = 4;
  OledHandle.stringClear = ENABLE;
  OLED_DisplayFormat(&OledHandle, "pressed=%d", ++count);
}
#endif

/**
 * @brief entry~
 */
int main(void)
{
  UTILS_InitDelay();
  UTILS_InitUart(115200);
  SERIALPORT_Init();
  //SI7021_Init();
  //OLED_Init(&OledHandle);
  //OLED_TurnOn();
  
  #if EC11_USE_CALLBACK == 1
  //EC11_Init(RefreshHandler, KeyHandler); //Initialize with callback
  #else
  EC11_Init(); //Initialize
  #endif
  
  while (1)
  {
	#if EC11_USE_CALLBACK == 0
    printf("%d\r\n", EC11_GetPosition());
    UTILS_DelayMs(200);
	#endif
    //UTILS_DelayMs(500);
    switch (SERIALPORT_GetCommand())
    {
      case SERIALPORT_CommandStartRun:
        SERIALPORT_StartRec();
      break;
      case SERIALPORT_CommandStopRun:
        SERIALPORT_StopRec();
      break;
      default:
        break;
    }
      
  }
}
