/**
 * @file    si7021.c
 * @author  Miaow
 * @version 0.1.0
 * @date    2019/11/16
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of si7021:
 *              1. Initialization
 *              2. Measurement
 * @note
 *          Minimum version of header file:
 *              0.1.0
 *          Pin connection:
 *               ┌───────┐     ┌───────┐
 *               │    PB8├─────┤SCL    │
 *               │    PB9├─────┤SDA    │
 *               └───────┘     └───────┘
 *               STM32F407      SI7021
 *  
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
#include "si7021.h"

/** @addtogroup SI7021
 * @{
 */
 
#define SI7021_ADDRESS          0x80
#define SI7021_WRITE_ADDRESS    SI7021_ADDRESS
#define SI7021_READ_ADDRESS     (SI7021_ADDRESS | 0x01)

/**
 * @brief 不准的延时函数
 */
static inline void SI7021_DelayUs(uint32_t us)
{
  __IO uint32_t tmp = us;
  tmp <<= 2;
  while (tmp--);
}

/**
 * @brief 产生起始信号
 */
static void SI7021_Start()
{
  SI7021_Out();
  SI7021_SDA_PORT->BSRRL = SI7021_SDA_PIN; 
  SI7021_SCL_PORT->BSRRL = SI7021_SCL_PIN;
  SI7021_DelayUs(1);
  SI7021_SDA_PORT->BSRRH = SI7021_SDA_PIN;
  SI7021_DelayUs(1);
  SI7021_SCL_PORT->BSRRH = SI7021_SCL_PIN;
}

/**
 * @brief 产生停止信号
 */
static void SI7021_Stop()
{
  SI7021_Out();
  SI7021_SCL_PORT->BSRRH = SI7021_SCL_PIN;
  SI7021_SDA_PORT->BSRRH = SI7021_SDA_PIN;
  SI7021_DelayUs(1);
  SI7021_SCL_PORT->BSRRL = SI7021_SCL_PIN;
  SI7021_DelayUs(1);
  SI7021_SDA_PORT->BSRRL = SI7021_SDA_PIN;
  SI7021_DelayUs(1);
}

/**
 * @brief 等待应答信号
 * @return 1-接收应答失败; 0-接收应答成功
 */
static uint8_t SI7021_WaitAck()
{
  __IO uint8_t ucErrTime = 0;
  SI7021_In();
  SI7021_SDA_PORT->BSRRL = SI7021_SDA_PIN;
  SI7021_SCL_PORT->BSRRL = SI7021_SCL_PIN;
  SI7021_DelayUs(1);
  while (SI7021_SDA_PORT->IDR & SI7021_SDA_PIN)
  {
    ucErrTime++;
    if (ucErrTime > 250)
    {
      SI7021_Stop();
      return 1;
    }
  }
  SI7021_SCL_PORT->BSRRH = SI7021_SCL_PIN;     
  return 0;
}
/**
 * @brief 产生应答信号
 */
static void SI7021_Ack()
{
  SI7021_SCL_PORT->BSRRH = SI7021_SCL_PIN;
  SI7021_Out();
  SI7021_SDA_PORT->BSRRH = SI7021_SDA_PIN;
  SI7021_DelayUs(1);
  SI7021_SCL_PORT->BSRRL = SI7021_SCL_PIN;
  SI7021_DelayUs(1);
  SI7021_SCL_PORT->BSRRH = SI7021_SCL_PIN;
}
/**
 * @brief 产生非应答信号
 */
static void SI7021_NAck()
{
  __IO uint32_t j;
  SI7021_SCL_PORT->BSRRH = SI7021_SCL_PIN;
  SI7021_Out();
  SI7021_SDA_PORT->BSRRL = SI7021_SDA_PIN;
  SI7021_DelayUs(1);
  SI7021_SCL_PORT->BSRRL = SI7021_SCL_PIN;
  SI7021_DelayUs(1);
  SI7021_SCL_PORT->BSRRH = SI7021_SCL_PIN;
}

/**
 * @brief IIC发送一个字节
 */
static void SI7021_WriteByte(uint8_t data)
{
  uint8_t i;
  __IO uint32_t j;
  SI7021_Out();
  SI7021_SCL_PORT->BSRRH = SI7021_SCL_PIN;
  for (i = 0; i < 8; i++)
  {
    SI7021_DelayUs(5);
    if ((data & 0x80) >> 7)
      SI7021_SDA_PORT->BSRRL = SI7021_SDA_PIN;
    else
      SI7021_SDA_PORT->BSRRH = SI7021_SDA_PIN;
    data <<= 1;
    SI7021_DelayUs(1);
    SI7021_SCL_PORT->BSRRL = SI7021_SCL_PIN;
    SI7021_DelayUs(1);
    SI7021_SCL_PORT->BSRRH = SI7021_SCL_PIN;
  }
}
/**
 * @brief 读1字节
 * @param ack
 *          1-发送应答
 *          0-发送非应答
 * @return 返回读取的一个字节
 */
static uint8_t SI7021_ReadByte(uint8_t ack)
{
  uint8_t i, receive = 0;
  __IO uint32_t j;
  SI7021_In();
  for (i = 0; i < 8; i++)
  {
    SI7021_DelayUs(5);
    SI7021_SCL_PORT->BSRRH = SI7021_SCL_PIN;
    SI7021_DelayUs(1);
    SI7021_SCL_PORT->BSRRL = SI7021_SCL_PIN;
    receive <<= 1;
    SI7021_DelayUs(1);
    if (SI7021_SDA_PORT->IDR & SI7021_SDA_PIN)
      receive++;
  }
  if (!ack)
    SI7021_NAck();
  else
    SI7021_Ack();
  return receive;
}

void SI7021_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(SI7021_SCL_GPIO_CLK | SI7021_SDA_GPIO_CLK, ENABLE);//使能GPIO时钟
  //SCL, SDA初始化设置
  GPIO_InitStructure.GPIO_Pin = SI7021_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(SI7021_SCL_PORT, &GPIO_InitStructure);//初始化
  GPIO_InitStructure.GPIO_Pin = SI7021_SDA_PIN;
  GPIO_Init(SI7021_SDA_PORT, &GPIO_InitStructure);//初始化
  SI7021_SCL_PORT->BSRRL = SI7021_SCL_PIN;//IIC_SCL=1
  SI7021_SDA_PORT->BSRRL = SI7021_SDA_PIN;//IIC_SDA=1
}

uint8_t SI7021_GetTempature(float* tempature)
{
  uint16_t result;
  SI7021_Start();
  SI7021_WriteByte(SI7021_WRITE_ADDRESS);
  if (SI7021_WaitAck() != 0)
    return 1;
  SI7021_WriteByte(0xF3);
  if (SI7021_WaitAck() != 0)
    return 1;
  
  UTILS_DelayMs(18);
  SI7021_Start();
  SI7021_WriteByte(SI7021_READ_ADDRESS);
  if (SI7021_WaitAck() != 0)
    return 1;
  
  result = (uint16_t)SI7021_ReadByte(1) << 8;
  result |= (uint16_t)SI7021_ReadByte(0);
  SI7021_Stop();

  // Swap the bytes and clear the status bits 
  *tempature = 175.72f * (float)result / 65536.0f - 46.85f;

  return 0;
}

uint8_t SI7021_GetHumidity(uint8_t* humidity)
{
  uint16_t result;
  SI7021_Start();
  SI7021_WriteByte(SI7021_WRITE_ADDRESS);
  if (SI7021_WaitAck() != 0)
    return 1;
  SI7021_WriteByte(0xF5);
  if (SI7021_WaitAck() != 0)
    return 1;
  
  UTILS_DelayMs(18);
  SI7021_Start();
  SI7021_WriteByte(SI7021_READ_ADDRESS);
  if (SI7021_WaitAck() != 0)
    return 1;
  
  result = (uint16_t)SI7021_ReadByte(1) << 8;
  result |= (uint16_t)SI7021_ReadByte(0);
  SI7021_Stop();

  // Swap the bytes and clear the status bits 
  *humidity = (125 * result >> 16) - 6;

  return 0;
}
/**
 * @}
 */
 