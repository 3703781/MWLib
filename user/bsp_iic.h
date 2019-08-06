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
#include "stdint.h"

//GPIO
#define IIC_SCL_PORT        GPIOB
#define IIC_SCL_PIN         GPIO_Pin_8
#define IIC_SCL_GPIO_CLK    RCC_AHB1Periph_GPIOB
#define IIC_SDA_PORT        GPIOB
#define IIC_SDA_PIN         GPIO_Pin_9
#define IIC_SDA_GPIO_CLK    RCC_AHB1Periph_GPIOB
#define IIC_In()            IIC_SDA_PORT->MODER &= ~0x000C0000;IIC_SDA_PORT->MODER |= 0x00000000//输入模式
#define IIC_Out()           IIC_SDA_PORT->MODER &= ~0x000C0000;IIC_SDA_PORT->MODER |= 0x00040000//输出模式


void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_WaitAck(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Init(void);
void IIC_WriteByte(uint8_t data);
uint8_t IIC_ReadByte(uint8_t ack);
uint8_t IIC_WriteRegByte(uint8_t addr, uint8_t reg, uint8_t data);
uint8_t IIC_ReadRegByte(uint8_t addr, uint8_t reg);
uint8_t IIC_WriteRegBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t* data);
uint8_t IIC_ReadRegBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t* data);

#endif
