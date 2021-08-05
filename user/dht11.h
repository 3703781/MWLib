#ifndef __DHT11_H
#define __DHT11_H
#include "utils.h"

//IO方向设置
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

uint8_t DHT11_Init(void);                              //初始化DHT11
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi); //读取温湿度
void DHT11_Rst(void);                                  //复位DHT11
#endif
