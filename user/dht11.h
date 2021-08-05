#ifndef __DHT11_H
#define __DHT11_H
#include "utils.h"

//IO��������
#define DHT11_GPIO_PORT GPIOB
#define DHT11_GPIO_PIN GPIO_Pin_0
#define DHT11_GPIO_CLK RCC_AHB1Periph_GPIOB

#define DHT11_GPIO_IN()                             \
    {                                               \
        DHT11_GPIO_PORT->MODER &= ~(3 << (0 * 2)); \
        DHT11_GPIO_PORT->MODER |= 0 << 0 * 2;                \
    }
#define DHT11_GPIO_OUT()                            \
    {                                               \
        DHT11_GPIO_PORT->MODER &= ~(3 << (0 * 2)); \
        DHT11_GPIO_PORT->MODER |= 1 << 0 * 2;                \
    }

uint8_t DHT11_Init(void);                              //��ʼ��DHT11
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi); //��ȡ��ʪ��
void DHT11_Rst(void);                                  //��λDHT11
#endif
