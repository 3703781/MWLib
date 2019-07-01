#ifndef __BSP_IIC_H
#define __BSP_IIC_H
#include "sys.h" 
#include "delay.h"

//GPIO
#define IIC_SCL_PORT        GPIOE
#define IIC_SCL_PIN         GPIO_Pin_2
#define IIC_SCL_GPIO_CLK    RCC_AHB1Periph_GPIOE
#define IIC_SDA_PORT        GPIOE
#define IIC_SDA_PIN         GPIO_Pin_3
#define IIC_SDA_GPIO_CLK    RCC_AHB1Periph_GPIOE
#define IIC_In()            IIC_SDA_PORT->MODER &= ~0x000000C0;IIC_SDA_PORT->MODER |= 0x00000000//输入模式
#define IIC_Out()           IIC_SDA_PORT->MODER &= ~0x000000C0;IIC_SDA_PORT->MODER |= 0x00000040//输出模式

 /**
 * @brief 初始化IIC
 */
void IIC_Init(void);
/**
 * @brief IIC发送一个字节
 * @return 1-有Ack; 0-无Ack
 */      
void IIC_WriteByte(uint8_t data);
/**
 * @brief 读1字节
 * @param ack
 *          1-发送应答
 *          0-发送非应答
 * @return 返回读取的一个字节
 */ 
uint8_t IIC_ReadByte(uint8_t ack);
/**
 * @brief IIC写1字节到寄存器
 * @param addr 器件地址
 * @param reg 寄存器地址
 * @param data 要写入的数据
 * @return 0-正常; 1-出错
 */
uint8_t IIC_WriteRegByte(uint8_t addr, uint8_t reg, uint8_t data);
/**
 * @brief IIC读1字节到寄存器
 * @param addr 器件地址
 * @param reg 寄存器地址
 * @return 要读取的数据
 */
uint8_t IIC_ReadRegByte(uint8_t addr, uint8_t reg);
/**
 * @brief IIC连续写
 * @param addr 器件地址
 * @param reg 寄存器地址
 * @param len 写入字节数
 * @param data 要写入的数据
 * @return 0-正常; 1-出错
 */
uint8_t IIC_WriteRegBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
/**
 * @brief IIC连续读
 * @param addr 器件地址
 * @param reg 寄存器地址
 * @param len 读取字节数
 * @param data 要读取的数据
 * @return 0-正常; 1-出错
 */
uint8_t IIC_ReadRegBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);




#endif
