/**
 * @file    gp2y0e03.c
 * @author  Miaow
 * @version 0.1.1
 * @date    2018/10/19
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of GP2Y0E03, the active optical distance measurement sensor:
 *              1. Initialization
 *              2. Configurations
 *              3. distance Measurement
 * @note
 *          Minimum version of header file:
 *              0.1.1
 *          Pin connection:
 *          ┌────────┐     ┌────────┐
 *          │    PE12├─────┤SCL     │
 *          │    PE15├─────┤SDA     │
 *          └────────┘     └────────┘
 *          STM32F407       GP2Y0E03
 *          
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#include "gp2y0e03.h"

uint8_t GP2Y0E03_maxOutputDistance[4] = {GP2Y0E03_Mod_64cm, GP2Y0E03_Mod_64cm, GP2Y0E03_Mod_64cm, GP2Y0E03_Mod_64cm};
/**
 * @brief 不准的微秒级延时函数.
 */
void GP2Y0E03_DelayUs(volatile uint32_t us)
{
    us <<= 4;
    while(us--);
}

/**
 * @brief 初始化GP2Y0E03_1的IIC.
 */
static inline void GP2Y0E03_1_IIC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(GP2Y0E03_1_SCL_GPIO_CLK | GP2Y0E03_1_SDA_GPIO_CLK, ENABLE);//使能GPIO时钟
    //SCL, SDA初始化设置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_1_SCL_PIN;
    GPIO_Init(GP2Y0E03_1_SCL_PORT, &GPIO_InitStructure);//初始化
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_1_SDA_PIN;
    GPIO_Init(GP2Y0E03_1_SDA_PORT, &GPIO_InitStructure);//初始化
    GP2Y0E03_1_SCL_PORT->BSRRL = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=1
    GP2Y0E03_1_SDA_PORT->BSRRL = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=1
}

/**
 * @brief 初始化GP2Y0E03_2的IIC.
 */
static inline void GP2Y0E03_2_IIC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(GP2Y0E03_2_SCL_GPIO_CLK | GP2Y0E03_2_SDA_GPIO_CLK, ENABLE);//使能GPIO时钟
    //SCL, SDA初始化设置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_2_SCL_PIN;
    GPIO_Init(GP2Y0E03_2_SCL_PORT, &GPIO_InitStructure);//初始化
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_2_SDA_PIN;
    GPIO_Init(GP2Y0E03_2_SDA_PORT, &GPIO_InitStructure);//初始化
    GP2Y0E03_2_SCL_PORT->BSRRL = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=1
    GP2Y0E03_2_SDA_PORT->BSRRL = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=1
}

/**
 * @brief 初始化GP2Y0E03_3的IIC.
 */
static inline void GP2Y0E03_3_IIC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(GP2Y0E03_3_SCL_GPIO_CLK | GP2Y0E03_3_SDA_GPIO_CLK, ENABLE);//使能GPIO时钟
    //SCL, SDA初始化设置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_3_SCL_PIN;
    GPIO_Init(GP2Y0E03_3_SCL_PORT, &GPIO_InitStructure);//初始化
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_3_SDA_PIN;
    GPIO_Init(GP2Y0E03_3_SDA_PORT, &GPIO_InitStructure);//初始化
    GP2Y0E03_3_SCL_PORT->BSRRL = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=1
    GP2Y0E03_3_SDA_PORT->BSRRL = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=1
}

/**
 * @brief 初始化GP2Y0E03_4的IIC.
 */
static inline void GP2Y0E03_4_IIC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(GP2Y0E03_4_SCL_GPIO_CLK | GP2Y0E03_4_SDA_GPIO_CLK, ENABLE);//使能GPIO时钟
    //SCL, SDA初始化设置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_4_SCL_PIN;
    GPIO_Init(GP2Y0E03_4_SCL_PORT, &GPIO_InitStructure);//初始化
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_4_SDA_PIN;
    GPIO_Init(GP2Y0E03_4_SDA_PORT, &GPIO_InitStructure);//初始化
    GP2Y0E03_4_SCL_PORT->BSRRL = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=1
    GP2Y0E03_4_SDA_PORT->BSRRL = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=1
}

/**
 * @brief 产生GP2Y0E03_1的IIC起始信号.
 */
static inline void GP2Y0E03_1_IIC_Start()
{

    GP2Y0E03_1_Out();//sda线输出
    GP2Y0E03_1_SDA_PORT->BSRRL = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=1      
    GP2Y0E03_1_SCL_PORT->BSRRL = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_1_SDA_PORT->BSRRH = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=0 START:when CLK is high,DATA change form high to low 
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//钳住I2C总线，准备发送或接收数据 
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_2的IIC起始信号.
 */
static inline void GP2Y0E03_2_IIC_Start()
{
    GP2Y0E03_2_Out();//sda线输出
    GP2Y0E03_2_SDA_PORT->BSRRL = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=1      
    GP2Y0E03_2_SCL_PORT->BSRRL = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_2_SDA_PORT->BSRRH = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=0 START:when CLK is high,DATA change form high to low 
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//钳住I2C总线，准备发送或接收数据 
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_3的IIC起始信号.
 */
static inline void GP2Y0E03_3_IIC_Start()
{
    GP2Y0E03_3_Out();//sda线输出
    GP2Y0E03_3_SDA_PORT->BSRRL = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=1      
    GP2Y0E03_3_SCL_PORT->BSRRL = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_3_SDA_PORT->BSRRH = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=0 START:when CLK is high,DATA change form high to low 
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//钳住I2C总线，准备发送或接收数据 
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_4的IIC起始信号.
 */
static inline void GP2Y0E03_4_IIC_Start()
{
    GP2Y0E03_4_Out();//sda线输出
    GP2Y0E03_4_SDA_PORT->BSRRL = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=1      
    GP2Y0E03_4_SCL_PORT->BSRRL = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_4_SDA_PORT->BSRRH = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=0 START:when CLK is high,DATA change form high to low 
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//钳住I2C总线，准备发送或接收数据 
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_1的IIC停止信号.
 */
static inline void GP2Y0E03_1_IIC_Stop()
{
    GP2Y0E03_1_Out();//sda线输出
    GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=0
    GP2Y0E03_1_SDA_PORT->BSRRH = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=0 STOP:when CLK is high DATA change form low to high
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_1_SCL_PORT->BSRRL = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_1_SDA_PORT->BSRRL = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=1 发送I2C总线结束信号
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_2的IIC停止信号.
 */
static inline void GP2Y0E03_2_IIC_Stop()
{
    GP2Y0E03_2_Out();//sda线输出
    GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=0
    GP2Y0E03_2_SDA_PORT->BSRRH = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=0 STOP:when CLK is high DATA change form low to high
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_2_SCL_PORT->BSRRL = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_2_SDA_PORT->BSRRL = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=1 发送I2C总线结束信号
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_3的IIC停止信号.
 */
static inline void GP2Y0E03_3_IIC_Stop()
{
    GP2Y0E03_3_Out();//sda线输出
    GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=0
    GP2Y0E03_3_SDA_PORT->BSRRH = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=0 STOP:when CLK is high DATA change form low to high
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_3_SCL_PORT->BSRRL = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_3_SDA_PORT->BSRRL = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=1 发送I2C总线结束信号
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_4的IIC停止信号.
 */
static inline void GP2Y0E03_4_IIC_Stop()
{
    GP2Y0E03_4_Out();//sda线输出
    GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=0
    GP2Y0E03_4_SDA_PORT->BSRRH = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=0 STOP:when CLK is high DATA change form low to high
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_4_SCL_PORT->BSRRL = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_4_SDA_PORT->BSRRL = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=1 发送I2C总线结束信号
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 等待GP2Y0E03_1_IIC_WaitAck的IIC应答信号.
 * @return 1-接收应答失败; 0-接收应答成功.
 */
static inline uint8_t GP2Y0E03_1_IIC_WaitAck()
{
    uint8_t ucErrTime = 0;
    GP2Y0E03_1_In();//SDA设置为输入  
    GP2Y0E03_1_SDA_PORT->BSRRL = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=1
    GP2Y0E03_DelayUs(1);       
    GP2Y0E03_1_SCL_PORT->BSRRL = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=1
    GP2Y0E03_DelayUs(1);     
    while(GP2Y0E03_1_SDA_PORT->IDR & GP2Y0E03_1_SDA_PIN)
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            GP2Y0E03_1_IIC_Stop();
            return 1;
        }
    }
    GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=0 时钟输出0        
    return 0;  
} 

/**
 * @brief 等待GP2Y0E03_2_IIC_WaitAck的IIC应答信号.
 * @return 1-接收应答失败; 0-接收应答成功.
 */
static inline uint8_t GP2Y0E03_2_IIC_WaitAck()
{
    uint8_t ucErrTime = 0;
    GP2Y0E03_2_In();//SDA设置为输入  
    GP2Y0E03_2_SDA_PORT->BSRRL = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=1
    GP2Y0E03_DelayUs(1);       
    GP2Y0E03_2_SCL_PORT->BSRRL = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=1
    GP2Y0E03_DelayUs(1);     
    while(GP2Y0E03_2_SDA_PORT->IDR & GP2Y0E03_2_SDA_PIN)
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            GP2Y0E03_2_IIC_Stop();
            return 1;
        }
    }
    GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=0 时钟输出0        
    return 0;  
}

/**
 * @brief 等待GP2Y0E03_3_IIC_WaitAck的IIC应答信号.
 * @return 1-接收应答失败; 0-接收应答成功.
 */
static inline uint8_t GP2Y0E03_3_IIC_WaitAck()
{
    uint8_t ucErrTime = 0;
    GP2Y0E03_3_In();//SDA设置为输入  
    GP2Y0E03_3_SDA_PORT->BSRRL = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=1
    GP2Y0E03_DelayUs(1);       
    GP2Y0E03_3_SCL_PORT->BSRRL = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=1
    GP2Y0E03_DelayUs(1);     
    while(GP2Y0E03_3_SDA_PORT->IDR & GP2Y0E03_3_SDA_PIN)
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            GP2Y0E03_3_IIC_Stop();
            return 1;
        }
    }
    GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=0 时钟输出0        
    return 0;  
}

/**
 * @brief 等待GP2Y0E03_4_IIC_WaitAck的IIC应答信号.
 * @return 1-接收应答失败; 0-接收应答成功.
 */
static inline uint8_t GP2Y0E03_4_IIC_WaitAck()
{
    uint8_t ucErrTime = 0;
    GP2Y0E03_4_In();//SDA设置为输入  
    GP2Y0E03_4_SDA_PORT->BSRRL = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=1
    GP2Y0E03_DelayUs(1);       
    GP2Y0E03_4_SCL_PORT->BSRRL = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=1
    GP2Y0E03_DelayUs(1);     
    while(GP2Y0E03_4_SDA_PORT->IDR & GP2Y0E03_4_SDA_PIN)
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            GP2Y0E03_4_IIC_Stop();
            return 1;
        }
    }
    GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=0 时钟输出0        
    return 0;  
}

/**
 * @brief 产生GP2Y0E03_1的IIC应答信号.
 */
static inline void GP2Y0E03_1_IIC_Ack()
{
    GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=0
    GP2Y0E03_1_Out();
    GP2Y0E03_1_SDA_PORT->BSRRH = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=0
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_1_SCL_PORT->BSRRL = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=0
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_2的IIC应答信号.
 */
static inline void GP2Y0E03_2_IIC_Ack()
{
    GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=0
    GP2Y0E03_2_Out();
    GP2Y0E03_2_SDA_PORT->BSRRH = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=0
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_2_SCL_PORT->BSRRL = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=0
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_3的IIC应答信号.
 */
static inline void GP2Y0E03_3_IIC_Ack()
{
    GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=0
    GP2Y0E03_3_Out();
    GP2Y0E03_3_SDA_PORT->BSRRH = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=0
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_3_SCL_PORT->BSRRL = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=0
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_4的IIC应答信号.
 */
static inline void GP2Y0E03_4_IIC_Ack()
{
    GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=0
    GP2Y0E03_4_Out();
    GP2Y0E03_4_SDA_PORT->BSRRH = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=0
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_4_SCL_PORT->BSRRL = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=0
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_1的IIC非应答信号.
 */
static inline void GP2Y0E03_1_IIC_NAck()
{
    GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=0
    GP2Y0E03_1_Out();
    GP2Y0E03_1_SDA_PORT->BSRRL = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_1_SCL_PORT->BSRRL = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=0
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_2的IIC非应答信号.
 */
static inline void GP2Y0E03_2_IIC_NAck()
{
    GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=0
    GP2Y0E03_2_Out();
    GP2Y0E03_2_SDA_PORT->BSRRL = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_2_SCL_PORT->BSRRL = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=0
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_3的IIC非应答信号.
 */
static inline void GP2Y0E03_3_IIC_NAck()
{
    GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=0
    GP2Y0E03_3_Out();
    GP2Y0E03_3_SDA_PORT->BSRRL = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_3_SCL_PORT->BSRRL = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=0
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief 产生GP2Y0E03_4的IIC非应答信号.
 */
static inline void GP2Y0E03_4_IIC_NAck()
{
    GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=0
    GP2Y0E03_4_Out();
    GP2Y0E03_4_SDA_PORT->BSRRL = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_4_SCL_PORT->BSRRL = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=0
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief GP2Y0E03_1的IIC发送一个字节.
 * @param data 要发送的字节.
 */
static inline void GP2Y0E03_1_IIC_WriteByte(uint8_t data)
{                        
    uint8_t i;
    GP2Y0E03_1_Out();
    GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=0 拉低时钟开始数据传输
    for(i = 0; i < 8; i++)
    {
        if((data & 0x80) >> 7)
            GP2Y0E03_1_SDA_PORT->BSRRL = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=1
        else
            GP2Y0E03_1_SDA_PORT->BSRRH = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=0
        data <<= 1;
        GP2Y0E03_DelayUs(1);
        GP2Y0E03_1_SCL_PORT->BSRRL = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=1
        GP2Y0E03_DelayUs(1);
        GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=0
        GP2Y0E03_DelayUs(1);
    }
}

/**
 * @brief GP2Y0E03_2的IIC发送一个字节.
 * @param data 要发送的字节.
 */
static inline void GP2Y0E03_2_IIC_WriteByte(uint8_t data)
{                        
    uint8_t i;
    GP2Y0E03_2_Out();
    GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=0 拉低时钟开始数据传输
    for(i = 0; i < 8; i++)
    {
        if((data & 0x80) >> 7)
            GP2Y0E03_2_SDA_PORT->BSRRL = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=1
        else
            GP2Y0E03_2_SDA_PORT->BSRRH = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=0
        data <<= 1;
        GP2Y0E03_DelayUs(1);
        GP2Y0E03_2_SCL_PORT->BSRRL = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=1
        GP2Y0E03_DelayUs(1);
        GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=0
        GP2Y0E03_DelayUs(1);
    }
}

/**
 * @brief GP2Y0E03_3的IIC发送一个字节.
 * @param data 要发送的字节.
 */
static inline void GP2Y0E03_3_IIC_WriteByte(uint8_t data)
{                        
    uint8_t i;
    GP2Y0E03_3_Out();
    GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=0 拉低时钟开始数据传输
    for(i = 0; i < 8; i++)
    {
        if((data & 0x80) >> 7)
            GP2Y0E03_3_SDA_PORT->BSRRL = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=1
        else
            GP2Y0E03_3_SDA_PORT->BSRRH = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=0
        data <<= 1;
        GP2Y0E03_DelayUs(1);
        GP2Y0E03_3_SCL_PORT->BSRRL = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=1
        GP2Y0E03_DelayUs(1);
        GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=0
        GP2Y0E03_DelayUs(1);
    }
}

/**
 * @brief GP2Y0E03_4的IIC发送一个字节.
 * @param data 要发送的字节.
 */
static inline void GP2Y0E03_4_IIC_WriteByte(uint8_t data)
{                        
    uint8_t i;
    GP2Y0E03_4_Out();
    GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=0 拉低时钟开始数据传输
    for(i = 0; i < 8; i++)
    {
        if((data & 0x80) >> 7)
            GP2Y0E03_4_SDA_PORT->BSRRL = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=1
        else
            GP2Y0E03_4_SDA_PORT->BSRRH = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=0
        data <<= 1;
        GP2Y0E03_DelayUs(1);
        GP2Y0E03_4_SCL_PORT->BSRRL = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=1
        GP2Y0E03_DelayUs(1);
        GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=0
        GP2Y0E03_DelayUs(1);
    }
}
/**
 * @brief GP2Y0E03_1的IIC读1字节.
 * @param ack 1-发送应答; 0-发送非应答.
 * @return 返回读取的一个字节.
 */
static inline uint8_t GP2Y0E03_1_IIC_ReadByte(uint8_t ack)
{
    uint8_t i, receive = 0;
    GP2Y0E03_1_In();//SDA设置为输入
    for(i = 0; i < 8; i++)
    {
        GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=0 
        GP2Y0E03_DelayUs(2);
        GP2Y0E03_1_SCL_PORT->BSRRL = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=1
        receive <<= 1;
        if(GP2Y0E03_1_SDA_PORT->IDR & GP2Y0E03_1_SDA_PIN)
            receive++;
        GP2Y0E03_DelayUs(2);
    } 
    if (!ack)
        GP2Y0E03_1_IIC_NAck();//发送nACK
    else
        GP2Y0E03_1_IIC_Ack(); //发送ACK   
    return receive;
}

/**
 * @brief GP2Y0E03_2的IIC读1字节.
 * @param ack 1-发送应答; 0-发送非应答.
 * @return 返回读取的一个字节.
 */
static inline uint8_t GP2Y0E03_2_IIC_ReadByte(uint8_t ack)
{
    uint8_t i, receive = 0;
    GP2Y0E03_2_In();//SDA设置为输入
    for(i = 0; i < 8; i++)
    {
        GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=0 
        GP2Y0E03_DelayUs(2);
        GP2Y0E03_2_SCL_PORT->BSRRL = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=1
        receive <<= 1;
        if(GP2Y0E03_2_SDA_PORT->IDR & GP2Y0E03_2_SDA_PIN)
            receive++;
        GP2Y0E03_DelayUs(2);
    } 
    if (!ack)
        GP2Y0E03_2_IIC_NAck();//发送nACK
    else
        GP2Y0E03_2_IIC_Ack(); //发送ACK   
    return receive;
}

/**
 * @brief GP2Y0E03_3的IIC读1字节.
 * @param ack 1-发送应答; 0-发送非应答.
 * @return 返回读取的一个字节.
 */
static inline uint8_t GP2Y0E03_3_IIC_ReadByte(uint8_t ack)
{
    uint8_t i, receive = 0;
    GP2Y0E03_3_In();//SDA设置为输入
    for(i = 0; i < 8; i++)
    {
        GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=0 
        GP2Y0E03_DelayUs(2);
        GP2Y0E03_3_SCL_PORT->BSRRL = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=1
        receive <<= 1;
        if(GP2Y0E03_3_SDA_PORT->IDR & GP2Y0E03_3_SDA_PIN)
            receive++;
        GP2Y0E03_DelayUs(2);
    } 
    if (!ack)
        GP2Y0E03_3_IIC_NAck();//发送nACK
    else
        GP2Y0E03_3_IIC_Ack(); //发送ACK   
    return receive;
}

/**
 * @brief GP2Y0E03_4的IIC读1字节.
 * @param ack 1-发送应答; 0-发送非应答.
 * @return 返回读取的一个字节.
 */
static inline uint8_t GP2Y0E03_4_IIC_ReadByte(uint8_t ack)
{
    uint8_t i, receive = 0;
    GP2Y0E03_4_In();//SDA设置为输入
    for(i = 0; i < 8; i++)
    {
        GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=0 
        GP2Y0E03_DelayUs(2);
        GP2Y0E03_4_SCL_PORT->BSRRL = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=1
        receive <<= 1;
        if(GP2Y0E03_4_SDA_PORT->IDR & GP2Y0E03_4_SDA_PIN)
            receive++;
        GP2Y0E03_DelayUs(2);
    } 
    if (!ack)
        GP2Y0E03_4_IIC_NAck();//发送nACK
    else
        GP2Y0E03_4_IIC_Ack(); //发送ACK   
    return receive;
}


/**
 * @brief IIC写1字节到寄存器.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @param reg 寄存器地址.
 * @param data 要写入的数据.
 * @return 0-正常; 1-出错.
 */
uint8_t GP2Y0E03_IIC_WriteRegByte(uint8_t sensorX, uint8_t reg, uint8_t data)
{
    switch(sensorX)
    {
        case GP2Y0E03_1:
            GP2Y0E03_1_IIC_Start();
            GP2Y0E03_1_IIC_WriteByte(GP2Y0E03_1_IIC_ADDRESS << 1);//发送器件地址+写命令
            if(GP2Y0E03_1_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_1_IIC_Stop();
                return 1;
            }
            GP2Y0E03_1_IIC_WriteByte(reg);//写寄存器地址
            if(GP2Y0E03_1_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_1_IIC_Stop();
                return 1;
            }
            GP2Y0E03_1_IIC_WriteByte(data);//发送数据
            if(GP2Y0E03_1_IIC_WaitAck())//等待ACK
            {
                GP2Y0E03_1_IIC_Stop();
                return 1;
            }		 
            GP2Y0E03_1_IIC_Stop();
            break;
        case GP2Y0E03_2:
            GP2Y0E03_2_IIC_Start();
            GP2Y0E03_2_IIC_WriteByte(GP2Y0E03_2_IIC_ADDRESS << 1);//发送器件地址+写命令
            if(GP2Y0E03_2_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_2_IIC_Stop();
                return 1;
            }
            GP2Y0E03_2_IIC_WriteByte(reg);//写寄存器地址
            if(GP2Y0E03_2_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_2_IIC_Stop();
                return 1;
            }
            GP2Y0E03_2_IIC_WriteByte(data);//发送数据
            if(GP2Y0E03_2_IIC_WaitAck())//等待ACK
            {
                GP2Y0E03_2_IIC_Stop();
                return 1;
            }		 
            GP2Y0E03_2_IIC_Stop();
            break;
        case GP2Y0E03_3:
            GP2Y0E03_3_IIC_Start();
            GP2Y0E03_3_IIC_WriteByte(GP2Y0E03_3_IIC_ADDRESS << 1);//发送器件地址+写命令
            if(GP2Y0E03_3_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_3_IIC_Stop();
                return 1;
            }
            GP2Y0E03_3_IIC_WriteByte(reg);//写寄存器地址
            if(GP2Y0E03_3_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_3_IIC_Stop();
                return 1;
            }
            GP2Y0E03_3_IIC_WriteByte(data);//发送数据
            if(GP2Y0E03_3_IIC_WaitAck())//等待ACK
            {
                GP2Y0E03_3_IIC_Stop();
                return 1;
            }		 
            GP2Y0E03_3_IIC_Stop();
            break;
        case GP2Y0E03_4:
            GP2Y0E03_4_IIC_Start();
            GP2Y0E03_4_IIC_WriteByte(GP2Y0E03_4_IIC_ADDRESS << 1);//发送器件地址+写命令
            if(GP2Y0E03_4_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_4_IIC_Stop();
                return 1;
            }
            GP2Y0E03_4_IIC_WriteByte(reg);//写寄存器地址
            if(GP2Y0E03_4_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_4_IIC_Stop();
                return 1;
            }
            GP2Y0E03_4_IIC_WriteByte(data);//发送数据
            if(GP2Y0E03_4_IIC_WaitAck())//等待ACK
            {
                GP2Y0E03_4_IIC_Stop();
                return 1;
            }		 
            GP2Y0E03_4_IIC_Stop();
            break;
    }
    return 0;
}

/**
 * @brief IIC读1字节到寄存器.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @param reg 寄存器地址.
 * @return 要读取的数据.
 */
uint8_t GP2Y0E03_IIC_ReadRegByte(uint8_t sensorX, uint8_t reg)
{
	uint8_t res;
    switch(sensorX)
    {
        case GP2Y0E03_1:
            GP2Y0E03_1_IIC_Start(); 
            GP2Y0E03_1_IIC_WriteByte(GP2Y0E03_1_IIC_ADDRESS << 1);//发送器件地址+写命令	
            GP2Y0E03_1_IIC_WaitAck();//等待应答
            GP2Y0E03_1_IIC_WriteByte(reg);//写寄存器地址
            GP2Y0E03_1_IIC_WaitAck();//等待应答
            GP2Y0E03_1_IIC_Stop();
            GP2Y0E03_1_IIC_Start();
            GP2Y0E03_1_IIC_WriteByte((GP2Y0E03_2_IIC_ADDRESS << 1) | 1);//发送器件地址+读命令	
            GP2Y0E03_1_IIC_WaitAck();//等待应答
            res = GP2Y0E03_1_IIC_ReadByte(0);//读取数据,发送nACK 
            GP2Y0E03_1_IIC_Stop();//产生一个停止条件
            break;
        
        case GP2Y0E03_2:
            GP2Y0E03_2_IIC_Start(); 
            GP2Y0E03_2_IIC_WriteByte(GP2Y0E03_2_IIC_ADDRESS << 1);//发送器件地址+写命令	
            GP2Y0E03_2_IIC_WaitAck();//等待应答
            GP2Y0E03_2_IIC_WriteByte(reg);//写寄存器地址
            GP2Y0E03_2_IIC_WaitAck();//等待应答
            GP2Y0E03_2_IIC_Stop();
            GP2Y0E03_2_IIC_Start();
            GP2Y0E03_2_IIC_WriteByte((GP2Y0E03_2_IIC_ADDRESS << 1) | 1);//发送器件地址+读命令	
            GP2Y0E03_2_IIC_WaitAck();//等待应答
            res = GP2Y0E03_2_IIC_ReadByte(0);//读取数据,发送nACK 
            GP2Y0E03_2_IIC_Stop();//产生一个停止条件
            break;
        
        case GP2Y0E03_3:
            GP2Y0E03_3_IIC_Start(); 
            GP2Y0E03_3_IIC_WriteByte(GP2Y0E03_3_IIC_ADDRESS << 1);//发送器件地址+写命令	
            GP2Y0E03_3_IIC_WaitAck();//等待应答
            GP2Y0E03_3_IIC_WriteByte(reg);//写寄存器地址
            GP2Y0E03_3_IIC_WaitAck();//等待应答
            GP2Y0E03_3_IIC_Stop();
            GP2Y0E03_3_IIC_Start();
            GP2Y0E03_3_IIC_WriteByte((GP2Y0E03_3_IIC_ADDRESS << 1) | 1);//发送器件地址+读命令	
            GP2Y0E03_3_IIC_WaitAck();//等待应答
            res = GP2Y0E03_3_IIC_ReadByte(0);//读取数据,发送nACK 
            GP2Y0E03_3_IIC_Stop();//产生一个停止条件
            break;
        
        case GP2Y0E03_4:
            GP2Y0E03_4_IIC_Start(); 
            GP2Y0E03_4_IIC_WriteByte(GP2Y0E03_4_IIC_ADDRESS << 1);//发送器件地址+写命令	
            GP2Y0E03_4_IIC_WaitAck();//等待应答
            GP2Y0E03_4_IIC_WriteByte(reg);//写寄存器地址
            GP2Y0E03_4_IIC_WaitAck();//等待应答
            GP2Y0E03_4_IIC_Stop();
            GP2Y0E03_4_IIC_Start();
            GP2Y0E03_4_IIC_WriteByte((GP2Y0E03_4_IIC_ADDRESS << 1) | 1);//发送器件地址+读命令	
            GP2Y0E03_4_IIC_WaitAck();//等待应答
            res = GP2Y0E03_4_IIC_ReadByte(0);//读取数据,发送nACK 
            GP2Y0E03_4_IIC_Stop();//产生一个停止条件
            break;
    }
	return res;		
}

/**
 * @brief IIC连续读.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @param reg 寄存器地址.
 * @param len 读取字节数.
 * @param data 要读取的数据.
 * @return 0-正常; 1-出错.
 */
uint8_t GP2Y0E03_IIC_ReadRegBytes(uint8_t sensorX, uint8_t reg, uint8_t len, uint8_t *data)
{
    switch(sensorX)
    {
        case GP2Y0E03_1:
            GP2Y0E03_1_IIC_Start();
            GP2Y0E03_1_IIC_WriteByte(GP2Y0E03_1_IIC_ADDRESS << 1);//发送器件地址+写命令
            if(GP2Y0E03_1_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_1_IIC_Stop();
                return 1;
            }
            GP2Y0E03_1_IIC_WriteByte(reg);//写寄存器地址
            if(GP2Y0E03_1_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_1_IIC_Stop();
                return 1;
            }
            GP2Y0E03_1_IIC_Start();
            GP2Y0E03_1_IIC_WriteByte((GP2Y0E03_1_IIC_ADDRESS << 1) | 1);//发送器件地址+读命令
            if(GP2Y0E03_1_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_1_IIC_Stop();
                return 1;
            }
            while(len)
            {
                if(len == 1)
                    *data=GP2Y0E03_1_IIC_ReadByte(0);//读数据,发送nACK
                else
                    *data=GP2Y0E03_1_IIC_ReadByte(1);//读数据,发送ACK
                len--;
                data++;
            }
            GP2Y0E03_1_IIC_Stop();//产生一个停止条件
            break;
            
        case GP2Y0E03_2:
            GP2Y0E03_2_IIC_Start();
            GP2Y0E03_2_IIC_WriteByte(GP2Y0E03_2_IIC_ADDRESS << 1);//发送器件地址+写命令
            if(GP2Y0E03_2_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_2_IIC_Stop();
                return 1;
            }
            GP2Y0E03_2_IIC_WriteByte(reg);//写寄存器地址
            if(GP2Y0E03_2_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_2_IIC_Stop();
                return 1;
            }
            GP2Y0E03_2_IIC_Start();
            GP2Y0E03_2_IIC_WriteByte((GP2Y0E03_2_IIC_ADDRESS << 1) | 1);//发送器件地址+读命令
            if(GP2Y0E03_2_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_2_IIC_Stop();
                return 1;
            }
            while(len)
            {
                if(len == 1)
                    *data=GP2Y0E03_2_IIC_ReadByte(0);//读数据,发送nACK
                else
                    *data=GP2Y0E03_2_IIC_ReadByte(1);//读数据,发送ACK
                len--;
                data++;
            }
            GP2Y0E03_2_IIC_Stop();//产生一个停止条件
            break;
            
        case GP2Y0E03_3:
            GP2Y0E03_3_IIC_Start();
            GP2Y0E03_3_IIC_WriteByte(GP2Y0E03_3_IIC_ADDRESS << 1);//发送器件地址+写命令
            if(GP2Y0E03_3_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_3_IIC_Stop();
                return 1;
            }
            GP2Y0E03_3_IIC_WriteByte(reg);//写寄存器地址
            if(GP2Y0E03_3_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_3_IIC_Stop();
                return 1;
            }
            GP2Y0E03_3_IIC_Start();
            GP2Y0E03_3_IIC_WriteByte((GP2Y0E03_3_IIC_ADDRESS << 1) | 1);//发送器件地址+读命令
            if(GP2Y0E03_3_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_3_IIC_Stop();
                return 1;
            }
            while(len)
            {
                if(len == 1)
                    *data=GP2Y0E03_3_IIC_ReadByte(0);//读数据,发送nACK
                else
                    *data=GP2Y0E03_3_IIC_ReadByte(1);//读数据,发送ACK
                len--;
                data++;
            }
            GP2Y0E03_3_IIC_Stop();//产生一个停止条件
            break;
            
        case GP2Y0E03_4:
            GP2Y0E03_4_IIC_Start();
            GP2Y0E03_4_IIC_WriteByte(GP2Y0E03_4_IIC_ADDRESS << 1);//发送器件地址+写命令
            if(GP2Y0E03_4_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_4_IIC_Stop();
                return 1;
            }
            GP2Y0E03_4_IIC_WriteByte(reg);//写寄存器地址
            if(GP2Y0E03_4_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_4_IIC_Stop();
                return 1;
            }
            GP2Y0E03_4_IIC_Start();
            GP2Y0E03_4_IIC_WriteByte((GP2Y0E03_4_IIC_ADDRESS << 1) | 1);//发送器件地址+读命令
            if(GP2Y0E03_4_IIC_WaitAck())//等待应答
            {
                GP2Y0E03_4_IIC_Stop();
                return 1;
            }
            while(len)
            {
                if(len == 1)
                    *data=GP2Y0E03_4_IIC_ReadByte(0);//读数据,发送nACK
                else
                    *data=GP2Y0E03_4_IIC_ReadByte(1);//读数据,发送ACK
                len--;
                data++;
            }
            GP2Y0E03_4_IIC_Stop();//产生一个停止条件
            break;
    }

    return 0;
}

/**
 * @brief 设置最大发射脉宽.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @param maxPulseWidth 最大发射脉宽 @ref GP2Y0E03_MaxPulseWidthTypedef.
 * @note 减小最大发射脉宽能够减小消耗的平均电流以降低功耗, 但会降低远距离或低反射率物品的测距稳定性. 
 *       改变最大发射脉宽不会改变响应时间.
 * @see 应用手册英文版P19
 */
inline void GP2Y0E03_SetMaxPulseWidth(uint8_t sensorX, GP2Y0E03_MaxPulseWidthTypedef maxPulseWidth)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x13, maxPulseWidth);
}

/**
 * @brief 设置信号累积次数.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @param signalAccumulation 设置信号累积数 @ref GP2Y0E03_SignalAccumulationTypedef.
 * @note GP2Y0E03通过累积一定信号次数后算出平均数确定光电位置的, 减小信号累积次数可以减小响应时间, 但会降低远距离或低反射率物品的测距稳定性.
 * @see 应用手册英文版P19.
 */
inline void GP2Y0E03_SetSignalAccumulation(uint8_t sensorX, GP2Y0E03_SignalAccumulationTypedef signalAccumulation)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xA8, signalAccumulation);
}

/**
 * @brief 设置中值滤波器.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @param medianFilter 滤波值 @ref GP2Y0E03_MedianFilterTypedef.
 * @note GP2Y0E03可以使用前几次距离测量的值来计算输出平均值.
 *       比如将滤波值设置为m, 第n次测量后输出第(n-m+1)到第n次的平均. 第1次到第m-1次的输出值为未知值.
 *       (不就是个低通滤波嘛, 秀~)
 * @see 应用手册英文版P19.
 */
inline void GP2Y0E03_SetMedianFilter(uint8_t sensorX, GP2Y0E03_MedianFilterTypedef medianFilter)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x3F, medianFilter);
}

/**
 * @brief 设置信号强度错误判断.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @param command 是否开启这个功能, 默认开启
 * @param signalIntensityThreshold 信号强度阈值(0~255) 默认已设置在E-Fuse里了.
 * @note 当反射光强低于信号强度阈值, 认为无法判断距离, 输出64cm(最大输出距离).

 * @see 应用手册英文版P23. 判断流程见P26的图36. 慎重开启, 原因见P25图34和图35.
 */
inline void GP2Y0E03_SetSignalIntensityJudement(uint8_t sensorX, FunctionalState command, uint8_t signalIntensityThreshold)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xBC, command == DISABLE);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x2F, signalIntensityThreshold);
}

/**
 * @brief 设置最小光斑错误判断.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @param command 是否开启这个功能, 默认开启
 * @param minSpotSizeThreshold 光斑尺寸阈值(0~255), 默认已设置在E-Fuse里了.
 * @note 当反射光斑小于光斑尺寸阈值, 认为无法判断距离, 输出64cm(最大输出距离).
 *       可以从寄存器读出测得的光斑尺寸值 = 0xF8[7:0] - 0xF9[7:0].
 * @see 应用手册英文版P23. 判断流程见P26的图36. 慎重开启, 原因见P25图34和图35.
 */
inline void GP2Y0E03_SetMinSpotSizeJudement(uint8_t sensorX, FunctionalState command, uint8_t minSpotSizeThreshold)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xBD, command == DISABLE);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x34, minSpotSizeThreshold);
}

/**
 * @brief 设置最大光斑错误判断.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @param command 是否开启这个功能, 默认关闭
 * @param maxSpotSizeThreshold 光斑尺寸阈值(0~0x7F), 默认0x7F.
 * @note 当反射光斑大于光斑尺寸阈值, 认为无法判断距离, 输出64cm(最大输出距离).
 *       光斑很大可能由射出不连续的光束或测量复杂反射面等导致.
 *       可以从寄存器读出测得的光斑尺寸值 = 0xF8[7:0] - 0xF9[7:0].
 * @see 应用手册英文版P24. 判断流程见P26的图36. 慎重开启, 原因见P25图34和图35.
 */
inline void GP2Y0E03_SetMaxSpotSizeJudement(uint8_t sensorX, FunctionalState command, uint8_t maxSpotSizeThreshold)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xBE, command == DISABLE);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x33, maxSpotSizeThreshold);
}

/**
 * @brief 设置光斑形状对称性错误判断.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @param command 是否开启这个功能, 默认开启
 * @param spotSymmetryThreshold 光斑对称性阈值(0~31), 默认14.
 * @note 当反射光斑形状不对称达到阈值时, 认为无法判断距离, 输出64cm(最大输出距离).
 *       可以从寄存器读出测得的光斑对称性 = |(0xF8[7:0]＋0xF9[7:0]) - 2 * 0xFA[7:0])|. 0xF8[7:0]为右边缘坐标(正值), -0xF9[7:0]为左边缘坐标,  0xFA[7:0]为峰值坐标
 * @see 应用手册英文版P24. 判断流程见P26的图36. 慎重开启, 原因见P25图34和图35.
 */
inline void GP2Y0E03_SetSpotSymmetryJudement(uint8_t sensorX, FunctionalState command, uint8_t spotSymmetryThreshold)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xBF, command == DISABLE);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x1C, spotSymmetryThreshold);
}

/**
 * @brief 设置最大输出距离.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @param maxOutputDistance 最大输出距离.
 * @note 超过50cm的输出值可能不准.
 * @see 应用手册英文版P26.
 */
inline void GP2Y0E03_SetMaxOutputDistance(uint8_t sensorX, GP2Y0E03_MaxOutputDistanceTypedef maxOutputDistance)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x35, maxOutputDistance);
    switch(sensorX)
    {
        case GP2Y0E03_1:
            GP2Y0E03_maxOutputDistance[0] = maxOutputDistance;
            break;
        case GP2Y0E03_2:
            GP2Y0E03_maxOutputDistance[1] = maxOutputDistance;
            break;
        case GP2Y0E03_3:
            GP2Y0E03_maxOutputDistance[2] = maxOutputDistance;
        break;
        case GP2Y0E03_4:
            GP2Y0E03_maxOutputDistance[3] = maxOutputDistance;
            break;
    }
}

/**
 * @brief 设置有效或待机状态.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @param setActive ENABLE-有效状态(默认值), DISABLE-待机状态.
 * @note GPIO1输入高电平时才可使用此函数.
 * @see 应用手册英文版P26.
 */
inline void GP2Y0E03_StateControlState(uint8_t sensorX, FunctionalState setActive)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xE8, setActive == DISABLE);
    GP2Y0E03_DelayUs(600);
}

/**
 * @brief 执行软件复位.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @see 应用手册英文版P27.
 */
inline void GP2Y0E03_SoftwareReset(uint8_t sensorX)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x00, 0xEF);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xEF, 0xEC);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x06, 0xEE);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x7F, 0xEC);
}

/**
 * @brief 读取光斑强度值.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @return 光斑强度值.
 */
uint32_t GP2Y0E03_ReadOutIntensity(uint8_t sensorX)
{
    uint32_t lowLevel, middleLevel, highLevel;
    
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x90, 0x10);
    lowLevel = (uint32_t)GP2Y0E03_IIC_ReadRegByte(sensorX, 0x90);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x90, 0x11);
    middleLevel = (uint32_t)GP2Y0E03_IIC_ReadRegByte(sensorX, 0x90);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x90, 0x12);
    highLevel = (uint32_t)GP2Y0E03_IIC_ReadRegByte(sensorX, 0x90);
    return (highLevel << 16) | (middleLevel << 8) | lowLevel;
}

/**
 * @brief 读取测距距离.
 * @param sensorX 指定的传感器, 除了GP2Y0E03_ALL在 @ref GP2Y0E03_select 里选一个.
 * @return 测距距离(cm).
 */
float GP2Y0E03_ReadOutDistance(uint8_t sensorX)
{
    int32_t high, low, res;
    high = (uint32_t)GP2Y0E03_IIC_ReadRegByte(sensorX, 0x5E);
    low = (uint32_t)GP2Y0E03_IIC_ReadRegByte(sensorX, 0x5F);
    res = (high << 4) | low;
    if(GP2Y0E03_maxOutputDistance[0] == GP2Y0E03_Mod_128cm)
        return (float)res / 128.0f;
    else
        return (float)res / 64.0f;
}

/**
 * @brief 初始化.
 * @param sensorX 指定的传感器, 可以是任意 @ref GP2Y0E03_select 的组合.
 */
void GP2Y0E03_Init(uint8_t sensorX)
{
    if(sensorX & GP2Y0E03_1)
    {
        GP2Y0E03_1_IIC_Init();
        GP2Y0E03_SetSignalIntensityJudement(GP2Y0E03_1,DISABLE, 0);
        GP2Y0E03_SetMinSpotSizeJudement(GP2Y0E03_1, DISABLE, 0);
        GP2Y0E03_SetMaxSpotSizeJudement(GP2Y0E03_1, DISABLE, 0);
        GP2Y0E03_SetSpotSymmetryJudement(GP2Y0E03_1, DISABLE, 14);
    }
    
    if(sensorX & GP2Y0E03_2)
    {
        GP2Y0E03_2_IIC_Init();
//        GP2Y0E03_SetSignalIntensityJudement(GP2Y0E03_2,DISABLE, 0);
//        GP2Y0E03_SetMinSpotSizeJudement(GP2Y0E03_2, DISABLE, 0);
//        GP2Y0E03_SetMaxSpotSizeJudement(GP2Y0E03_2, DISABLE, 0);
//        GP2Y0E03_SetSpotSymmetryJudement(GP2Y0E03_2, DISABLE, 14);
    }
    
    if(sensorX & GP2Y0E03_3)
    {
        GP2Y0E03_3_IIC_Init();
        GP2Y0E03_SetSignalIntensityJudement(GP2Y0E03_3,DISABLE, 0);
        GP2Y0E03_SetMinSpotSizeJudement(GP2Y0E03_3, DISABLE, 0);
        GP2Y0E03_SetMaxSpotSizeJudement(GP2Y0E03_3, DISABLE, 0);
        GP2Y0E03_SetSpotSymmetryJudement(GP2Y0E03_3, DISABLE, 14);
    }
    
    if(sensorX & GP2Y0E03_4)
    {
        GP2Y0E03_4_IIC_Init();
        GP2Y0E03_SetSignalIntensityJudement(GP2Y0E03_4,DISABLE, 0);
        GP2Y0E03_SetMinSpotSizeJudement(GP2Y0E03_4, DISABLE, 0);
        GP2Y0E03_SetMaxSpotSizeJudement(GP2Y0E03_4, DISABLE, 0);
        GP2Y0E03_SetSpotSymmetryJudement(GP2Y0E03_4, DISABLE, 14);
    }
}
