/**
 * @file    bsp_iic.h
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

#ifndef __BSP_IIC_H
#define __BSP_IIC_H
#include "stm32f4xx.h"

//GPIO
#define IIC_SCL_PORT        GPIOB
#define IIC_SCL_PIN         GPIO_Pin_8
#define IIC_SCL_GPIO_CLK    RCC_AHB1Periph_GPIOB
#define IIC_SDA_PORT        GPIOB
#define IIC_SDA_PIN         GPIO_Pin_9
#define IIC_SDA_GPIO_CLK    RCC_AHB1Periph_GPIOB
#define IIC_In()            IIC_SDA_PORT->MODER &= ~0x000C0000;IIC_SDA_PORT->MODER |= 0x00000000//输入模式
#define IIC_Out()           IIC_SDA_PORT->MODER &= ~0x000C0000;IIC_SDA_PORT->MODER |= 0x00040000//输出模式

/**
 * @brief 产生起始信号
 */
void IIC_Start(void);
/**
 * @brief 产生停止信号
 */
void IIC_Stop(void);
/**
 * @brief 等待应答信号
 * @return 1-接收应答失败; 0-接收应答成功
 */
uint8_t IIC_WaitAck(void);
/**
 * @brief 产生应答信号
 */
void IIC_Ack(void);
/**
 * @brief 产生非应答信号
 */  
void IIC_NAck(void);

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
