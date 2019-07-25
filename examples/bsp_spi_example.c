/**
 * @file    ec11_example.c
 * @author  Miaow
 * @date    2019/07/25
 */
 
#include "stm32f4xx.h"
#include "utils.h"
#include "bsp_spi.h"

#if BSP_SPI_USE_RECEIVE_CALLBACK != 0

/**
 * @brief When a frame of data is received, this function is called.
 * @param data Data array that received and parsed in accordance with the protocol.
 * @param length Number of elements in the array.
 */
void ReceiveHandler(uint32_t* data, uint8_t length)
{
  uint8_t i;
  //Show data
  printf("<");
  for (i = 0; i < length; i++)
  {
    printf("%d", data[i]);
    if (i < length - 1)
      printf(", ");
  }
  printf(">\r\n");
}
#endif

/**
 * @brief entry~
 */
int main()
{
  uint8_t i = 0;
  UTILS_InitDelay();
  UTILS_InitUart(115200);
  UTILS_DelayMs(100);
  printf("ahb=%d, apb1=%d, apb2=%d\r\n", AhbClock, Apb1Clock, Apb2Clock);
  
  #if BSP_SPI_USE_RECEIVE_CALLBACK != 0
  BSP_SPI_Init(ReceiveHandler);
  #else
  BSP_SPI_Init();
  #endif
  while (1)
	{
    #if BSP_SPI_USE_RECEIVE_CALLBACK == 0
    uint8_t data;
    UTILS_DelayMs(300);
    data = BSP_SPI_TransferByte(i);
    printf("Sent 0x%02X, received 0x%02X", i, data);
    i++;
    #endif
	}
}
