/**
 * @file    bsp_iic.c
 * @author  Miaow
 * @version 0.1.0
 * @date    2018/08/30
 * @brief
 *          This file provides functions to manage the following
 *          functionalities of IIC:
 *              1. Initialization
 *              2. Write and read one byte
 *              3. Write and read registers of slave
 * @note
 *          Minimum version of header file:
 *              0.1.0
 *          Pin connection:
 *          ┌────────┐     ┌────────┐
 *          │     PB8├─────┤SCL     │
 *          │     PB9├─────┤SDA     │
 *          └────────┘     └────────┘
 *          STM32F407        slave
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#include "bsp_iic.h"
#include "utils.h"

 /**
  * @brief 如果没有接收到应答信号，则返回1
  */
#define IIC_IF_NOT_ACK_RETURN_1     if(IIC_WaitAck()) {IIC_Stop(); return 1;}

static void IIC_DelayUs(uint32_t us);
static uint8_t isInitialized = 0;

/**
 * @brief 不准的微秒级延时函数
 */
static inline void IIC_DelayUs(uint32_t us)
{
  __IO uint32_t tmp = us;
  tmp <<= 2;
  while (tmp--);
}
/**
 * @brief 初始化IIC
 */
void IIC_Init()
{
  if (isInitialized)
    return;
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(IIC_SCL_GPIO_CLK | IIC_SDA_GPIO_CLK, ENABLE);//使能GPIO时钟
  //SCL, SDA初始化设置
  GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(IIC_SCL_PORT, &GPIO_InitStructure);//初始化
  GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
  GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);//初始化
  IIC_SCL_PORT->BSRRL = IIC_SCL_PIN;//IIC_SCL=1
  IIC_SDA_PORT->BSRRL = IIC_SDA_PIN;//IIC_SDA=1
  isInitialized = 1;
}
/**
 * @brief 产生起始信号
 */
void IIC_Start()
{
  IIC_Out();//sda线输出
  IIC_SDA_PORT->BSRRL = IIC_SDA_PIN;//IIC_SDA=1      
  IIC_SCL_PORT->BSRRL = IIC_SCL_PIN;//IIC_SCL=1
  IIC_DelayUs(2);
  IIC_SDA_PORT->BSRRH = IIC_SDA_PIN;//IIC_SDA=0 START:when CLK is high,DATA change form high to low 
  IIC_DelayUs(2);
  IIC_SCL_PORT->BSRRH = IIC_SCL_PIN;//钳住I2C总线，准备发送或接收数据 
}
/**
 * @brief 产生停止信号
 */
void IIC_Stop()
{
  IIC_Out();//sda线输出
  IIC_SCL_PORT->BSRRH = IIC_SCL_PIN;//IIC_SCL=0
  IIC_SDA_PORT->BSRRH = IIC_SDA_PIN;//IIC_SDA=0 STOP:when CLK is high DATA change form low to high
  IIC_DelayUs(2);
  IIC_SCL_PORT->BSRRL = IIC_SCL_PIN;//IIC_SCL=1
  IIC_SDA_PORT->BSRRL = IIC_SDA_PIN;//IIC_SDA=1 发送I2C总线结束信号
  IIC_DelayUs(2);
}
/**
 * @brief 等待应答信号
 * @return 1-接收应答失败; 0-接收应答成功
 */
uint8_t IIC_WaitAck()
{
  uint8_t ucErrTime = 0;
  IIC_In();//SDA设置为输入  
  IIC_SDA_PORT->BSRRL = IIC_SDA_PIN;//IIC_SDA=1
  IIC_DelayUs(1);
  IIC_SCL_PORT->BSRRL = IIC_SCL_PIN;//IIC_SCL=1
  IIC_DelayUs(1);
  while (IIC_SDA_PORT->IDR & IIC_SDA_PIN)
  {
    ucErrTime++;
    if (ucErrTime > 250)
    {
      IIC_Stop();
      return 1;
    }
  }
  IIC_SCL_PORT->BSRRH = IIC_SCL_PIN;//IIC_SCL=0 时钟输出0        
  return 0;
}
/**
 * @brief 产生应答信号
 */
void IIC_Ack()
{
  IIC_SCL_PORT->BSRRH = IIC_SCL_PIN;//IIC_SCL=0
  IIC_Out();
  IIC_SDA_PORT->BSRRH = IIC_SDA_PIN;//IIC_SDA=0
  IIC_DelayUs(1);
  IIC_SCL_PORT->BSRRL = IIC_SCL_PIN;//IIC_SCL=1
  IIC_DelayUs(1);
  IIC_SCL_PORT->BSRRH = IIC_SCL_PIN;//IIC_SCL=0
}
/**
 * @brief 产生非应答信号
 */
void IIC_NAck()
{
  IIC_SCL_PORT->BSRRH = IIC_SCL_PIN;//IIC_SCL=0
  IIC_Out();
  IIC_SDA_PORT->BSRRL = IIC_SDA_PIN;//IIC_SDA=1
  IIC_DelayUs(1);
  IIC_SCL_PORT->BSRRL = IIC_SCL_PIN;//IIC_SCL=1
  IIC_SCL_PORT->BSRRH = IIC_SCL_PIN;//IIC_SCL=0
}
/**
 * @brief IIC发送一个字节
 */
void IIC_WriteByte(uint8_t data)
{
  uint8_t i;
  __IO uint32_t j;
  IIC_Out();
  IIC_SCL_PORT->BSRRH = IIC_SCL_PIN;//IIC_SCL=0 拉低时钟开始数据传输
  for (i = 0; i < 8; i++)
  {
    if ((data & 0x80) >> 7)
      IIC_SDA_PORT->BSRRL = IIC_SDA_PIN;//IIC_SDA=1
    else
      IIC_SDA_PORT->BSRRH = IIC_SDA_PIN;//IIC_SDA=0
    data <<= 1;
    j = 1;
    while (j--);
    IIC_SCL_PORT->BSRRL = IIC_SCL_PIN;//IIC_SCL=1
    j = 1;
    while (j--);
    IIC_SCL_PORT->BSRRH = IIC_SCL_PIN;//IIC_SCL=0
  }
}
/**
 * @brief 读1字节
 * @param ack
 *          1-发送应答
 *          0-发送非应答
 * @return 返回读取的一个字节
 */
uint8_t IIC_ReadByte(uint8_t ack)
{
  uint8_t i, receive = 0;
  __IO uint32_t j;
  IIC_In();//SDA设置为输入
  for (i = 0; i < 8; i++)
  {
    IIC_DelayUs(1);
    IIC_SCL_PORT->BSRRH = IIC_SCL_PIN;//IIC_SCL=0
    j = 10;
    while (j--);
    IIC_SCL_PORT->BSRRL = IIC_SCL_PIN;//IIC_SCL=1
    receive <<= 1;
    if (IIC_SDA_PORT->IDR & IIC_SDA_PIN)
      receive++;
  }
  if (!ack)
    IIC_NAck();//发送nACK
  else
    IIC_Ack(); //发送ACK   
  return receive;
}
/**
 * @brief IIC写1字节到寄存器
 * @param addr 器件地址
 * @param reg 寄存器地址
 * @param data 要写入的数据
 * @return 0-正常; 1-出错
 */
uint8_t IIC_WriteRegByte(uint8_t addr, uint8_t reg, uint8_t data)
{
  IIC_Start();
  IIC_WriteByte(addr << 1);//发送器件地址+写命令	
  if (IIC_WaitAck())	//等待应答
  {
    IIC_Stop();
    return 1;
  }
  IIC_WriteByte(reg);	//写寄存器地址
  IIC_WaitAck();		//等待应答 
  IIC_WriteByte(data);//发送数据
  if (IIC_WaitAck())	//等待ACK
  {
    IIC_Stop();
    return 1;
  }
  IIC_Stop();
  return 0;
}
/**
 * @brief IIC读1字节到寄存器
 * @param addr 器件地址
 * @param reg 寄存器地址
 * @return 要读取的数据
 */
uint8_t IIC_ReadRegByte(uint8_t addr, uint8_t reg)
{
  uint8_t res;
  IIC_Start();
  IIC_WriteByte(addr << 1);//发送器件地址+写命令	
  IIC_WaitAck();//等待应答 
  IIC_WriteByte(reg);//写寄存器地址
  IIC_WaitAck();//等待应答
  IIC_Start();
  IIC_WriteByte((addr << 1) | 1);//发送器件地址+读命令	
  IIC_WaitAck();//等待应答 
  res = IIC_ReadByte(0);//读取数据,发送nACK 
  IIC_Stop();//产生一个停止条件 
  return res;
}

/**
 * @brief IIC连续写
 * @param addr 器件地址
 * @param reg 寄存器地址
 * @param len 写入字节数
 * @param data 要写入的数据
 * @return 0-正常; 1-出错
 */
uint8_t IIC_WriteRegBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
  uint8_t i;
  IIC_Start();
  IIC_WriteByte(addr << 1);//发送器件地址+写命令    
  if (IIC_WaitAck())//等待应答
  {
    IIC_Stop();
    return 1;
  }
  IIC_WriteByte(reg);//写寄存器地址
  IIC_WaitAck();//等待应答
  for (i = 0; i < len; i++)
  {
    IIC_WriteByte(data[i]);//发送数据
    if (IIC_WaitAck())//等待ACK
    {
      IIC_Stop();
      return 1;
    }
  }
  IIC_Stop();
  return 0;
}
/**
 * @brief IIC连续读
 * @param addr 器件地址
 * @param reg 寄存器地址
 * @param len 读取字节数
 * @param data 要读取的数据
 * @return 0-正常; 1-出错
 */
uint8_t IIC_ReadRegBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
  IIC_Start();
  IIC_WriteByte(addr << 1);//发送器件地址+写命令    
  if (IIC_WaitAck())//等待应答
  {
    IIC_Stop();
    return 1;
  }
  IIC_WriteByte(reg);//写寄存器地址
  IIC_WaitAck();//等待应答
  IIC_Start();
  IIC_WriteByte((addr << 1) | 1);//发送器件地址+读命令    
  IIC_WaitAck();//等待应答 
  while (len)
  {
    if (len == 1)
      * data = IIC_ReadByte(0);//读数据,发送nACK 
    else
      *data = IIC_ReadByte(1);//读数据,发送ACK  
    len--;
    data++;
  }
  IIC_Stop();//产生一个停止条件 
  return 0;
}
