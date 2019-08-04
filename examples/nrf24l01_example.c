/**
 * @file    nrf24l01.c
 * @author  Miaow
 * @date    2019/08/04
 */
 
#include "utils.h"
#include "nrf24l01.h"

#define MODE        NRF24L01_ReceiveMode //NRF24L01_SendMode

int main()
{
  uint32_t i;
  uint8_t buffer[32];

  UTILS_InitDelay();
  UTILS_InitUart(115200);
  NRF24L01_Init(MODE);

  printf("Checking connection......");
  if (NRF24L01_Check() == NRF24L01_Ok)
  {
    printf("pass\r\n");
  }
  else
  {
    printf("fail\r\n");
    while(1);
  }
  
  while(1)
  {
	  
#if MODE == NRF24L01_ReceiveMode

    NRF24L01_Status result = NRF24L01_Receive(buffer);
    if (result == NRF24L01_Ok)
    {
      printf("[");
      for (i = 0; i < NRF24L01_RECEIVE_LENGTH; i++)
        printf("%02X ", (uint32_t)buffer[i]);
      printf("]\r\n");
    }
	
#elif MODE == NRF24L01_SendMode
	
	printf("%dth......", ++i);
    NRF24L01_Status result = NRF24L01_Send(buffer);
    if (result == NRF24L01_Ok)
      printf("OK\r\n");
    else if (result == NRF24L01_MaxSendError)
      printf("MaxSend error\r\n");
    else
      printf("Unknown error\r\n");
    UTILS_DelayMs(500);
	
#else

#error "NRF24L01_ReceiveMode or NRF24L01_SendMode."

#endif

  }
}
