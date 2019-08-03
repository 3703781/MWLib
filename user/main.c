/**
 * @file    main.c
 * @author  Miaow
 * @date    2019/07/13
 */
#include "main.h"
#include "stm32f4xx.h"
#include "utils.h"
#include "nrf24l01.h"

int main()
{        
  uint8_t buffer[32];
  uint8_t i;
  UTILS_InitDelay();
  UTILS_InitUart(115200);
  NRF24L01_Init(NRF24L01_SendMode);
  
  for (i = 0; i < 32; i++)
    buffer[i] = i;
  
  printf("Check connection......");
  if (NRF24L01_Check() == NRF24L01_Ok)
  {
    printf("passed\r\n");
  }
  else
  {
    printf("failed\r\n");
    while(1);
  }
  
	while(1)
	{
    printf("Send......");
    NRF24L01_Status result = NRF24L01_Send(buffer);
    if (result == NRF24L01_Ok)
      printf("OK\r\n");
    else if (result == NRF24L01_MaxSendError)
      printf("Max send error\r\n");
    else
      printf("Unknown error\r\n");
    UTILS_DelayMs(500);
	}
}
