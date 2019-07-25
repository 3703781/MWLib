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
 *          ��������������������     ��������������������
 *          ��    PE12��������������SCL     ��
 *          ��    PE15��������������SDA     ��
 *          ��������������������     ��������������������
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
 * @brief ��׼��΢�뼶��ʱ����.
 */
void GP2Y0E03_DelayUs(volatile uint32_t us)
{
    us <<= 4;
    while(us--);
}

/**
 * @brief ��ʼ��GP2Y0E03_1��IIC.
 */
static inline void GP2Y0E03_1_IIC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(GP2Y0E03_1_SCL_GPIO_CLK | GP2Y0E03_1_SDA_GPIO_CLK, ENABLE);//ʹ��GPIOʱ��
    //SCL, SDA��ʼ������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_1_SCL_PIN;
    GPIO_Init(GP2Y0E03_1_SCL_PORT, &GPIO_InitStructure);//��ʼ��
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_1_SDA_PIN;
    GPIO_Init(GP2Y0E03_1_SDA_PORT, &GPIO_InitStructure);//��ʼ��
    GP2Y0E03_1_SCL_PORT->BSRRL = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=1
    GP2Y0E03_1_SDA_PORT->BSRRL = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=1
}

/**
 * @brief ��ʼ��GP2Y0E03_2��IIC.
 */
static inline void GP2Y0E03_2_IIC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(GP2Y0E03_2_SCL_GPIO_CLK | GP2Y0E03_2_SDA_GPIO_CLK, ENABLE);//ʹ��GPIOʱ��
    //SCL, SDA��ʼ������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_2_SCL_PIN;
    GPIO_Init(GP2Y0E03_2_SCL_PORT, &GPIO_InitStructure);//��ʼ��
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_2_SDA_PIN;
    GPIO_Init(GP2Y0E03_2_SDA_PORT, &GPIO_InitStructure);//��ʼ��
    GP2Y0E03_2_SCL_PORT->BSRRL = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=1
    GP2Y0E03_2_SDA_PORT->BSRRL = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=1
}

/**
 * @brief ��ʼ��GP2Y0E03_3��IIC.
 */
static inline void GP2Y0E03_3_IIC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(GP2Y0E03_3_SCL_GPIO_CLK | GP2Y0E03_3_SDA_GPIO_CLK, ENABLE);//ʹ��GPIOʱ��
    //SCL, SDA��ʼ������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_3_SCL_PIN;
    GPIO_Init(GP2Y0E03_3_SCL_PORT, &GPIO_InitStructure);//��ʼ��
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_3_SDA_PIN;
    GPIO_Init(GP2Y0E03_3_SDA_PORT, &GPIO_InitStructure);//��ʼ��
    GP2Y0E03_3_SCL_PORT->BSRRL = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=1
    GP2Y0E03_3_SDA_PORT->BSRRL = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=1
}

/**
 * @brief ��ʼ��GP2Y0E03_4��IIC.
 */
static inline void GP2Y0E03_4_IIC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(GP2Y0E03_4_SCL_GPIO_CLK | GP2Y0E03_4_SDA_GPIO_CLK, ENABLE);//ʹ��GPIOʱ��
    //SCL, SDA��ʼ������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_4_SCL_PIN;
    GPIO_Init(GP2Y0E03_4_SCL_PORT, &GPIO_InitStructure);//��ʼ��
    GPIO_InitStructure.GPIO_Pin = GP2Y0E03_4_SDA_PIN;
    GPIO_Init(GP2Y0E03_4_SDA_PORT, &GPIO_InitStructure);//��ʼ��
    GP2Y0E03_4_SCL_PORT->BSRRL = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=1
    GP2Y0E03_4_SDA_PORT->BSRRL = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=1
}

/**
 * @brief ����GP2Y0E03_1��IIC��ʼ�ź�.
 */
static inline void GP2Y0E03_1_IIC_Start()
{

    GP2Y0E03_1_Out();//sda�����
    GP2Y0E03_1_SDA_PORT->BSRRL = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=1      
    GP2Y0E03_1_SCL_PORT->BSRRL = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_1_SDA_PORT->BSRRH = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=0 START:when CLK is high,DATA change form high to low 
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//ǯסI2C���ߣ�׼�����ͻ�������� 
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief ����GP2Y0E03_2��IIC��ʼ�ź�.
 */
static inline void GP2Y0E03_2_IIC_Start()
{
    GP2Y0E03_2_Out();//sda�����
    GP2Y0E03_2_SDA_PORT->BSRRL = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=1      
    GP2Y0E03_2_SCL_PORT->BSRRL = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_2_SDA_PORT->BSRRH = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=0 START:when CLK is high,DATA change form high to low 
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//ǯסI2C���ߣ�׼�����ͻ�������� 
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief ����GP2Y0E03_3��IIC��ʼ�ź�.
 */
static inline void GP2Y0E03_3_IIC_Start()
{
    GP2Y0E03_3_Out();//sda�����
    GP2Y0E03_3_SDA_PORT->BSRRL = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=1      
    GP2Y0E03_3_SCL_PORT->BSRRL = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_3_SDA_PORT->BSRRH = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=0 START:when CLK is high,DATA change form high to low 
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//ǯסI2C���ߣ�׼�����ͻ�������� 
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief ����GP2Y0E03_4��IIC��ʼ�ź�.
 */
static inline void GP2Y0E03_4_IIC_Start()
{
    GP2Y0E03_4_Out();//sda�����
    GP2Y0E03_4_SDA_PORT->BSRRL = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=1      
    GP2Y0E03_4_SCL_PORT->BSRRL = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_4_SDA_PORT->BSRRH = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=0 START:when CLK is high,DATA change form high to low 
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//ǯסI2C���ߣ�׼�����ͻ�������� 
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief ����GP2Y0E03_1��IICֹͣ�ź�.
 */
static inline void GP2Y0E03_1_IIC_Stop()
{
    GP2Y0E03_1_Out();//sda�����
    GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=0
    GP2Y0E03_1_SDA_PORT->BSRRH = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=0 STOP:when CLK is high DATA change form low to high
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_1_SCL_PORT->BSRRL = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_1_SDA_PORT->BSRRL = GP2Y0E03_1_SDA_PIN;//GP2Y0E03_1_SDA=1 ����I2C���߽����ź�
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief ����GP2Y0E03_2��IICֹͣ�ź�.
 */
static inline void GP2Y0E03_2_IIC_Stop()
{
    GP2Y0E03_2_Out();//sda�����
    GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=0
    GP2Y0E03_2_SDA_PORT->BSRRH = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=0 STOP:when CLK is high DATA change form low to high
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_2_SCL_PORT->BSRRL = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_2_SDA_PORT->BSRRL = GP2Y0E03_2_SDA_PIN;//GP2Y0E03_2_SDA=1 ����I2C���߽����ź�
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief ����GP2Y0E03_3��IICֹͣ�ź�.
 */
static inline void GP2Y0E03_3_IIC_Stop()
{
    GP2Y0E03_3_Out();//sda�����
    GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=0
    GP2Y0E03_3_SDA_PORT->BSRRH = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=0 STOP:when CLK is high DATA change form low to high
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_3_SCL_PORT->BSRRL = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_3_SDA_PORT->BSRRL = GP2Y0E03_3_SDA_PIN;//GP2Y0E03_3_SDA=1 ����I2C���߽����ź�
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief ����GP2Y0E03_4��IICֹͣ�ź�.
 */
static inline void GP2Y0E03_4_IIC_Stop()
{
    GP2Y0E03_4_Out();//sda�����
    GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=0
    GP2Y0E03_4_SDA_PORT->BSRRH = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=0 STOP:when CLK is high DATA change form low to high
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_4_SCL_PORT->BSRRL = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=1
    GP2Y0E03_DelayUs(2);
    GP2Y0E03_4_SDA_PORT->BSRRL = GP2Y0E03_4_SDA_PIN;//GP2Y0E03_4_SDA=1 ����I2C���߽����ź�
    GP2Y0E03_DelayUs(2);
}

/**
 * @brief �ȴ�GP2Y0E03_1_IIC_WaitAck��IICӦ���ź�.
 * @return 1-����Ӧ��ʧ��; 0-����Ӧ��ɹ�.
 */
static inline uint8_t GP2Y0E03_1_IIC_WaitAck()
{
    uint8_t ucErrTime = 0;
    GP2Y0E03_1_In();//SDA����Ϊ����  
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
    GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=0 ʱ�����0        
    return 0;  
} 

/**
 * @brief �ȴ�GP2Y0E03_2_IIC_WaitAck��IICӦ���ź�.
 * @return 1-����Ӧ��ʧ��; 0-����Ӧ��ɹ�.
 */
static inline uint8_t GP2Y0E03_2_IIC_WaitAck()
{
    uint8_t ucErrTime = 0;
    GP2Y0E03_2_In();//SDA����Ϊ����  
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
    GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=0 ʱ�����0        
    return 0;  
}

/**
 * @brief �ȴ�GP2Y0E03_3_IIC_WaitAck��IICӦ���ź�.
 * @return 1-����Ӧ��ʧ��; 0-����Ӧ��ɹ�.
 */
static inline uint8_t GP2Y0E03_3_IIC_WaitAck()
{
    uint8_t ucErrTime = 0;
    GP2Y0E03_3_In();//SDA����Ϊ����  
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
    GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=0 ʱ�����0        
    return 0;  
}

/**
 * @brief �ȴ�GP2Y0E03_4_IIC_WaitAck��IICӦ���ź�.
 * @return 1-����Ӧ��ʧ��; 0-����Ӧ��ɹ�.
 */
static inline uint8_t GP2Y0E03_4_IIC_WaitAck()
{
    uint8_t ucErrTime = 0;
    GP2Y0E03_4_In();//SDA����Ϊ����  
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
    GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=0 ʱ�����0        
    return 0;  
}

/**
 * @brief ����GP2Y0E03_1��IICӦ���ź�.
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
 * @brief ����GP2Y0E03_2��IICӦ���ź�.
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
 * @brief ����GP2Y0E03_3��IICӦ���ź�.
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
 * @brief ����GP2Y0E03_4��IICӦ���ź�.
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
 * @brief ����GP2Y0E03_1��IIC��Ӧ���ź�.
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
 * @brief ����GP2Y0E03_2��IIC��Ӧ���ź�.
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
 * @brief ����GP2Y0E03_3��IIC��Ӧ���ź�.
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
 * @brief ����GP2Y0E03_4��IIC��Ӧ���ź�.
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
 * @brief GP2Y0E03_1��IIC����һ���ֽ�.
 * @param data Ҫ���͵��ֽ�.
 */
static inline void GP2Y0E03_1_IIC_WriteByte(uint8_t data)
{                        
    uint8_t i;
    GP2Y0E03_1_Out();
    GP2Y0E03_1_SCL_PORT->BSRRH = GP2Y0E03_1_SCL_PIN;//GP2Y0E03_1_SCL=0 ����ʱ�ӿ�ʼ���ݴ���
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
 * @brief GP2Y0E03_2��IIC����һ���ֽ�.
 * @param data Ҫ���͵��ֽ�.
 */
static inline void GP2Y0E03_2_IIC_WriteByte(uint8_t data)
{                        
    uint8_t i;
    GP2Y0E03_2_Out();
    GP2Y0E03_2_SCL_PORT->BSRRH = GP2Y0E03_2_SCL_PIN;//GP2Y0E03_2_SCL=0 ����ʱ�ӿ�ʼ���ݴ���
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
 * @brief GP2Y0E03_3��IIC����һ���ֽ�.
 * @param data Ҫ���͵��ֽ�.
 */
static inline void GP2Y0E03_3_IIC_WriteByte(uint8_t data)
{                        
    uint8_t i;
    GP2Y0E03_3_Out();
    GP2Y0E03_3_SCL_PORT->BSRRH = GP2Y0E03_3_SCL_PIN;//GP2Y0E03_3_SCL=0 ����ʱ�ӿ�ʼ���ݴ���
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
 * @brief GP2Y0E03_4��IIC����һ���ֽ�.
 * @param data Ҫ���͵��ֽ�.
 */
static inline void GP2Y0E03_4_IIC_WriteByte(uint8_t data)
{                        
    uint8_t i;
    GP2Y0E03_4_Out();
    GP2Y0E03_4_SCL_PORT->BSRRH = GP2Y0E03_4_SCL_PIN;//GP2Y0E03_4_SCL=0 ����ʱ�ӿ�ʼ���ݴ���
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
 * @brief GP2Y0E03_1��IIC��1�ֽ�.
 * @param ack 1-����Ӧ��; 0-���ͷ�Ӧ��.
 * @return ���ض�ȡ��һ���ֽ�.
 */
static inline uint8_t GP2Y0E03_1_IIC_ReadByte(uint8_t ack)
{
    uint8_t i, receive = 0;
    GP2Y0E03_1_In();//SDA����Ϊ����
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
        GP2Y0E03_1_IIC_NAck();//����nACK
    else
        GP2Y0E03_1_IIC_Ack(); //����ACK   
    return receive;
}

/**
 * @brief GP2Y0E03_2��IIC��1�ֽ�.
 * @param ack 1-����Ӧ��; 0-���ͷ�Ӧ��.
 * @return ���ض�ȡ��һ���ֽ�.
 */
static inline uint8_t GP2Y0E03_2_IIC_ReadByte(uint8_t ack)
{
    uint8_t i, receive = 0;
    GP2Y0E03_2_In();//SDA����Ϊ����
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
        GP2Y0E03_2_IIC_NAck();//����nACK
    else
        GP2Y0E03_2_IIC_Ack(); //����ACK   
    return receive;
}

/**
 * @brief GP2Y0E03_3��IIC��1�ֽ�.
 * @param ack 1-����Ӧ��; 0-���ͷ�Ӧ��.
 * @return ���ض�ȡ��һ���ֽ�.
 */
static inline uint8_t GP2Y0E03_3_IIC_ReadByte(uint8_t ack)
{
    uint8_t i, receive = 0;
    GP2Y0E03_3_In();//SDA����Ϊ����
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
        GP2Y0E03_3_IIC_NAck();//����nACK
    else
        GP2Y0E03_3_IIC_Ack(); //����ACK   
    return receive;
}

/**
 * @brief GP2Y0E03_4��IIC��1�ֽ�.
 * @param ack 1-����Ӧ��; 0-���ͷ�Ӧ��.
 * @return ���ض�ȡ��һ���ֽ�.
 */
static inline uint8_t GP2Y0E03_4_IIC_ReadByte(uint8_t ack)
{
    uint8_t i, receive = 0;
    GP2Y0E03_4_In();//SDA����Ϊ����
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
        GP2Y0E03_4_IIC_NAck();//����nACK
    else
        GP2Y0E03_4_IIC_Ack(); //����ACK   
    return receive;
}


/**
 * @brief IICд1�ֽڵ��Ĵ���.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @param reg �Ĵ�����ַ.
 * @param data Ҫд�������.
 * @return 0-����; 1-����.
 */
uint8_t GP2Y0E03_IIC_WriteRegByte(uint8_t sensorX, uint8_t reg, uint8_t data)
{
    switch(sensorX)
    {
        case GP2Y0E03_1:
            GP2Y0E03_1_IIC_Start();
            GP2Y0E03_1_IIC_WriteByte(GP2Y0E03_1_IIC_ADDRESS << 1);//����������ַ+д����
            if(GP2Y0E03_1_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_1_IIC_Stop();
                return 1;
            }
            GP2Y0E03_1_IIC_WriteByte(reg);//д�Ĵ�����ַ
            if(GP2Y0E03_1_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_1_IIC_Stop();
                return 1;
            }
            GP2Y0E03_1_IIC_WriteByte(data);//��������
            if(GP2Y0E03_1_IIC_WaitAck())//�ȴ�ACK
            {
                GP2Y0E03_1_IIC_Stop();
                return 1;
            }		 
            GP2Y0E03_1_IIC_Stop();
            break;
        case GP2Y0E03_2:
            GP2Y0E03_2_IIC_Start();
            GP2Y0E03_2_IIC_WriteByte(GP2Y0E03_2_IIC_ADDRESS << 1);//����������ַ+д����
            if(GP2Y0E03_2_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_2_IIC_Stop();
                return 1;
            }
            GP2Y0E03_2_IIC_WriteByte(reg);//д�Ĵ�����ַ
            if(GP2Y0E03_2_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_2_IIC_Stop();
                return 1;
            }
            GP2Y0E03_2_IIC_WriteByte(data);//��������
            if(GP2Y0E03_2_IIC_WaitAck())//�ȴ�ACK
            {
                GP2Y0E03_2_IIC_Stop();
                return 1;
            }		 
            GP2Y0E03_2_IIC_Stop();
            break;
        case GP2Y0E03_3:
            GP2Y0E03_3_IIC_Start();
            GP2Y0E03_3_IIC_WriteByte(GP2Y0E03_3_IIC_ADDRESS << 1);//����������ַ+д����
            if(GP2Y0E03_3_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_3_IIC_Stop();
                return 1;
            }
            GP2Y0E03_3_IIC_WriteByte(reg);//д�Ĵ�����ַ
            if(GP2Y0E03_3_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_3_IIC_Stop();
                return 1;
            }
            GP2Y0E03_3_IIC_WriteByte(data);//��������
            if(GP2Y0E03_3_IIC_WaitAck())//�ȴ�ACK
            {
                GP2Y0E03_3_IIC_Stop();
                return 1;
            }		 
            GP2Y0E03_3_IIC_Stop();
            break;
        case GP2Y0E03_4:
            GP2Y0E03_4_IIC_Start();
            GP2Y0E03_4_IIC_WriteByte(GP2Y0E03_4_IIC_ADDRESS << 1);//����������ַ+д����
            if(GP2Y0E03_4_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_4_IIC_Stop();
                return 1;
            }
            GP2Y0E03_4_IIC_WriteByte(reg);//д�Ĵ�����ַ
            if(GP2Y0E03_4_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_4_IIC_Stop();
                return 1;
            }
            GP2Y0E03_4_IIC_WriteByte(data);//��������
            if(GP2Y0E03_4_IIC_WaitAck())//�ȴ�ACK
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
 * @brief IIC��1�ֽڵ��Ĵ���.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @param reg �Ĵ�����ַ.
 * @return Ҫ��ȡ������.
 */
uint8_t GP2Y0E03_IIC_ReadRegByte(uint8_t sensorX, uint8_t reg)
{
	uint8_t res;
    switch(sensorX)
    {
        case GP2Y0E03_1:
            GP2Y0E03_1_IIC_Start(); 
            GP2Y0E03_1_IIC_WriteByte(GP2Y0E03_1_IIC_ADDRESS << 1);//����������ַ+д����	
            GP2Y0E03_1_IIC_WaitAck();//�ȴ�Ӧ��
            GP2Y0E03_1_IIC_WriteByte(reg);//д�Ĵ�����ַ
            GP2Y0E03_1_IIC_WaitAck();//�ȴ�Ӧ��
            GP2Y0E03_1_IIC_Stop();
            GP2Y0E03_1_IIC_Start();
            GP2Y0E03_1_IIC_WriteByte((GP2Y0E03_2_IIC_ADDRESS << 1) | 1);//����������ַ+������	
            GP2Y0E03_1_IIC_WaitAck();//�ȴ�Ӧ��
            res = GP2Y0E03_1_IIC_ReadByte(0);//��ȡ����,����nACK 
            GP2Y0E03_1_IIC_Stop();//����һ��ֹͣ����
            break;
        
        case GP2Y0E03_2:
            GP2Y0E03_2_IIC_Start(); 
            GP2Y0E03_2_IIC_WriteByte(GP2Y0E03_2_IIC_ADDRESS << 1);//����������ַ+д����	
            GP2Y0E03_2_IIC_WaitAck();//�ȴ�Ӧ��
            GP2Y0E03_2_IIC_WriteByte(reg);//д�Ĵ�����ַ
            GP2Y0E03_2_IIC_WaitAck();//�ȴ�Ӧ��
            GP2Y0E03_2_IIC_Stop();
            GP2Y0E03_2_IIC_Start();
            GP2Y0E03_2_IIC_WriteByte((GP2Y0E03_2_IIC_ADDRESS << 1) | 1);//����������ַ+������	
            GP2Y0E03_2_IIC_WaitAck();//�ȴ�Ӧ��
            res = GP2Y0E03_2_IIC_ReadByte(0);//��ȡ����,����nACK 
            GP2Y0E03_2_IIC_Stop();//����һ��ֹͣ����
            break;
        
        case GP2Y0E03_3:
            GP2Y0E03_3_IIC_Start(); 
            GP2Y0E03_3_IIC_WriteByte(GP2Y0E03_3_IIC_ADDRESS << 1);//����������ַ+д����	
            GP2Y0E03_3_IIC_WaitAck();//�ȴ�Ӧ��
            GP2Y0E03_3_IIC_WriteByte(reg);//д�Ĵ�����ַ
            GP2Y0E03_3_IIC_WaitAck();//�ȴ�Ӧ��
            GP2Y0E03_3_IIC_Stop();
            GP2Y0E03_3_IIC_Start();
            GP2Y0E03_3_IIC_WriteByte((GP2Y0E03_3_IIC_ADDRESS << 1) | 1);//����������ַ+������	
            GP2Y0E03_3_IIC_WaitAck();//�ȴ�Ӧ��
            res = GP2Y0E03_3_IIC_ReadByte(0);//��ȡ����,����nACK 
            GP2Y0E03_3_IIC_Stop();//����һ��ֹͣ����
            break;
        
        case GP2Y0E03_4:
            GP2Y0E03_4_IIC_Start(); 
            GP2Y0E03_4_IIC_WriteByte(GP2Y0E03_4_IIC_ADDRESS << 1);//����������ַ+д����	
            GP2Y0E03_4_IIC_WaitAck();//�ȴ�Ӧ��
            GP2Y0E03_4_IIC_WriteByte(reg);//д�Ĵ�����ַ
            GP2Y0E03_4_IIC_WaitAck();//�ȴ�Ӧ��
            GP2Y0E03_4_IIC_Stop();
            GP2Y0E03_4_IIC_Start();
            GP2Y0E03_4_IIC_WriteByte((GP2Y0E03_4_IIC_ADDRESS << 1) | 1);//����������ַ+������	
            GP2Y0E03_4_IIC_WaitAck();//�ȴ�Ӧ��
            res = GP2Y0E03_4_IIC_ReadByte(0);//��ȡ����,����nACK 
            GP2Y0E03_4_IIC_Stop();//����һ��ֹͣ����
            break;
    }
	return res;		
}

/**
 * @brief IIC������.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @param reg �Ĵ�����ַ.
 * @param len ��ȡ�ֽ���.
 * @param data Ҫ��ȡ������.
 * @return 0-����; 1-����.
 */
uint8_t GP2Y0E03_IIC_ReadRegBytes(uint8_t sensorX, uint8_t reg, uint8_t len, uint8_t *data)
{
    switch(sensorX)
    {
        case GP2Y0E03_1:
            GP2Y0E03_1_IIC_Start();
            GP2Y0E03_1_IIC_WriteByte(GP2Y0E03_1_IIC_ADDRESS << 1);//����������ַ+д����
            if(GP2Y0E03_1_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_1_IIC_Stop();
                return 1;
            }
            GP2Y0E03_1_IIC_WriteByte(reg);//д�Ĵ�����ַ
            if(GP2Y0E03_1_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_1_IIC_Stop();
                return 1;
            }
            GP2Y0E03_1_IIC_Start();
            GP2Y0E03_1_IIC_WriteByte((GP2Y0E03_1_IIC_ADDRESS << 1) | 1);//����������ַ+������
            if(GP2Y0E03_1_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_1_IIC_Stop();
                return 1;
            }
            while(len)
            {
                if(len == 1)
                    *data=GP2Y0E03_1_IIC_ReadByte(0);//������,����nACK
                else
                    *data=GP2Y0E03_1_IIC_ReadByte(1);//������,����ACK
                len--;
                data++;
            }
            GP2Y0E03_1_IIC_Stop();//����һ��ֹͣ����
            break;
            
        case GP2Y0E03_2:
            GP2Y0E03_2_IIC_Start();
            GP2Y0E03_2_IIC_WriteByte(GP2Y0E03_2_IIC_ADDRESS << 1);//����������ַ+д����
            if(GP2Y0E03_2_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_2_IIC_Stop();
                return 1;
            }
            GP2Y0E03_2_IIC_WriteByte(reg);//д�Ĵ�����ַ
            if(GP2Y0E03_2_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_2_IIC_Stop();
                return 1;
            }
            GP2Y0E03_2_IIC_Start();
            GP2Y0E03_2_IIC_WriteByte((GP2Y0E03_2_IIC_ADDRESS << 1) | 1);//����������ַ+������
            if(GP2Y0E03_2_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_2_IIC_Stop();
                return 1;
            }
            while(len)
            {
                if(len == 1)
                    *data=GP2Y0E03_2_IIC_ReadByte(0);//������,����nACK
                else
                    *data=GP2Y0E03_2_IIC_ReadByte(1);//������,����ACK
                len--;
                data++;
            }
            GP2Y0E03_2_IIC_Stop();//����һ��ֹͣ����
            break;
            
        case GP2Y0E03_3:
            GP2Y0E03_3_IIC_Start();
            GP2Y0E03_3_IIC_WriteByte(GP2Y0E03_3_IIC_ADDRESS << 1);//����������ַ+д����
            if(GP2Y0E03_3_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_3_IIC_Stop();
                return 1;
            }
            GP2Y0E03_3_IIC_WriteByte(reg);//д�Ĵ�����ַ
            if(GP2Y0E03_3_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_3_IIC_Stop();
                return 1;
            }
            GP2Y0E03_3_IIC_Start();
            GP2Y0E03_3_IIC_WriteByte((GP2Y0E03_3_IIC_ADDRESS << 1) | 1);//����������ַ+������
            if(GP2Y0E03_3_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_3_IIC_Stop();
                return 1;
            }
            while(len)
            {
                if(len == 1)
                    *data=GP2Y0E03_3_IIC_ReadByte(0);//������,����nACK
                else
                    *data=GP2Y0E03_3_IIC_ReadByte(1);//������,����ACK
                len--;
                data++;
            }
            GP2Y0E03_3_IIC_Stop();//����һ��ֹͣ����
            break;
            
        case GP2Y0E03_4:
            GP2Y0E03_4_IIC_Start();
            GP2Y0E03_4_IIC_WriteByte(GP2Y0E03_4_IIC_ADDRESS << 1);//����������ַ+д����
            if(GP2Y0E03_4_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_4_IIC_Stop();
                return 1;
            }
            GP2Y0E03_4_IIC_WriteByte(reg);//д�Ĵ�����ַ
            if(GP2Y0E03_4_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_4_IIC_Stop();
                return 1;
            }
            GP2Y0E03_4_IIC_Start();
            GP2Y0E03_4_IIC_WriteByte((GP2Y0E03_4_IIC_ADDRESS << 1) | 1);//����������ַ+������
            if(GP2Y0E03_4_IIC_WaitAck())//�ȴ�Ӧ��
            {
                GP2Y0E03_4_IIC_Stop();
                return 1;
            }
            while(len)
            {
                if(len == 1)
                    *data=GP2Y0E03_4_IIC_ReadByte(0);//������,����nACK
                else
                    *data=GP2Y0E03_4_IIC_ReadByte(1);//������,����ACK
                len--;
                data++;
            }
            GP2Y0E03_4_IIC_Stop();//����һ��ֹͣ����
            break;
    }

    return 0;
}

/**
 * @brief �������������.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @param maxPulseWidth ��������� @ref GP2Y0E03_MaxPulseWidthTypedef.
 * @note ��С����������ܹ���С���ĵ�ƽ�������Խ��͹���, ���ή��Զ�����ͷ�������Ʒ�Ĳ���ȶ���. 
 *       �ı������������ı���Ӧʱ��.
 * @see Ӧ���ֲ�Ӣ�İ�P19
 */
inline void GP2Y0E03_SetMaxPulseWidth(uint8_t sensorX, GP2Y0E03_MaxPulseWidthTypedef maxPulseWidth)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x13, maxPulseWidth);
}

/**
 * @brief �����ź��ۻ�����.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @param signalAccumulation �����ź��ۻ��� @ref GP2Y0E03_SignalAccumulationTypedef.
 * @note GP2Y0E03ͨ���ۻ�һ���źŴ��������ƽ����ȷ�����λ�õ�, ��С�ź��ۻ��������Լ�С��Ӧʱ��, ���ή��Զ�����ͷ�������Ʒ�Ĳ���ȶ���.
 * @see Ӧ���ֲ�Ӣ�İ�P19.
 */
inline void GP2Y0E03_SetSignalAccumulation(uint8_t sensorX, GP2Y0E03_SignalAccumulationTypedef signalAccumulation)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xA8, signalAccumulation);
}

/**
 * @brief ������ֵ�˲���.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @param medianFilter �˲�ֵ @ref GP2Y0E03_MedianFilterTypedef.
 * @note GP2Y0E03����ʹ��ǰ���ξ��������ֵ���������ƽ��ֵ.
 *       ���罫�˲�ֵ����Ϊm, ��n�β����������(n-m+1)����n�ε�ƽ��. ��1�ε���m-1�ε����ֵΪδֵ֪.
 *       (�����Ǹ���ͨ�˲���, ��~)
 * @see Ӧ���ֲ�Ӣ�İ�P19.
 */
inline void GP2Y0E03_SetMedianFilter(uint8_t sensorX, GP2Y0E03_MedianFilterTypedef medianFilter)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x3F, medianFilter);
}

/**
 * @brief �����ź�ǿ�ȴ����ж�.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @param command �Ƿ����������, Ĭ�Ͽ���
 * @param signalIntensityThreshold �ź�ǿ����ֵ(0~255) Ĭ����������E-Fuse����.
 * @note �������ǿ�����ź�ǿ����ֵ, ��Ϊ�޷��жϾ���, ���64cm(����������).

 * @see Ӧ���ֲ�Ӣ�İ�P23. �ж����̼�P26��ͼ36. ���ؿ���, ԭ���P25ͼ34��ͼ35.
 */
inline void GP2Y0E03_SetSignalIntensityJudement(uint8_t sensorX, FunctionalState command, uint8_t signalIntensityThreshold)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xBC, command == DISABLE);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x2F, signalIntensityThreshold);
}

/**
 * @brief ������С��ߴ����ж�.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @param command �Ƿ����������, Ĭ�Ͽ���
 * @param minSpotSizeThreshold ��߳ߴ���ֵ(0~255), Ĭ����������E-Fuse����.
 * @note ��������С�ڹ�߳ߴ���ֵ, ��Ϊ�޷��жϾ���, ���64cm(����������).
 *       ���ԴӼĴ���������õĹ�߳ߴ�ֵ = 0xF8[7:0] - 0xF9[7:0].
 * @see Ӧ���ֲ�Ӣ�İ�P23. �ж����̼�P26��ͼ36. ���ؿ���, ԭ���P25ͼ34��ͼ35.
 */
inline void GP2Y0E03_SetMinSpotSizeJudement(uint8_t sensorX, FunctionalState command, uint8_t minSpotSizeThreshold)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xBD, command == DISABLE);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x34, minSpotSizeThreshold);
}

/**
 * @brief ��������ߴ����ж�.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @param command �Ƿ����������, Ĭ�Ϲر�
 * @param maxSpotSizeThreshold ��߳ߴ���ֵ(0~0x7F), Ĭ��0x7F.
 * @note �������ߴ��ڹ�߳ߴ���ֵ, ��Ϊ�޷��жϾ���, ���64cm(����������).
 *       ��ߺܴ����������������Ĺ�����������ӷ�����ȵ���.
 *       ���ԴӼĴ���������õĹ�߳ߴ�ֵ = 0xF8[7:0] - 0xF9[7:0].
 * @see Ӧ���ֲ�Ӣ�İ�P24. �ж����̼�P26��ͼ36. ���ؿ���, ԭ���P25ͼ34��ͼ35.
 */
inline void GP2Y0E03_SetMaxSpotSizeJudement(uint8_t sensorX, FunctionalState command, uint8_t maxSpotSizeThreshold)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xBE, command == DISABLE);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x33, maxSpotSizeThreshold);
}

/**
 * @brief ���ù����״�Գ��Դ����ж�.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @param command �Ƿ����������, Ĭ�Ͽ���
 * @param spotSymmetryThreshold ��߶Գ�����ֵ(0~31), Ĭ��14.
 * @note ����������״���Գƴﵽ��ֵʱ, ��Ϊ�޷��жϾ���, ���64cm(����������).
 *       ���ԴӼĴ���������õĹ�߶Գ��� = |(0xF8[7:0]��0xF9[7:0]) - 2 * 0xFA[7:0])|. 0xF8[7:0]Ϊ�ұ�Ե����(��ֵ), -0xF9[7:0]Ϊ���Ե����,  0xFA[7:0]Ϊ��ֵ����
 * @see Ӧ���ֲ�Ӣ�İ�P24. �ж����̼�P26��ͼ36. ���ؿ���, ԭ���P25ͼ34��ͼ35.
 */
inline void GP2Y0E03_SetSpotSymmetryJudement(uint8_t sensorX, FunctionalState command, uint8_t spotSymmetryThreshold)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xBF, command == DISABLE);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x1C, spotSymmetryThreshold);
}

/**
 * @brief ��������������.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @param maxOutputDistance ����������.
 * @note ����50cm�����ֵ���ܲ�׼.
 * @see Ӧ���ֲ�Ӣ�İ�P26.
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
 * @brief ������Ч�����״̬.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @param setActive ENABLE-��Ч״̬(Ĭ��ֵ), DISABLE-����״̬.
 * @note GPIO1����ߵ�ƽʱ�ſ�ʹ�ô˺���.
 * @see Ӧ���ֲ�Ӣ�İ�P26.
 */
inline void GP2Y0E03_StateControlState(uint8_t sensorX, FunctionalState setActive)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xE8, setActive == DISABLE);
    GP2Y0E03_DelayUs(600);
}

/**
 * @brief ִ�������λ.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @see Ӧ���ֲ�Ӣ�İ�P27.
 */
inline void GP2Y0E03_SoftwareReset(uint8_t sensorX)
{
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x00, 0xEF);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0xEF, 0xEC);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x06, 0xEE);
    GP2Y0E03_IIC_WriteRegByte(sensorX, 0x7F, 0xEC);
}

/**
 * @brief ��ȡ���ǿ��ֵ.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @return ���ǿ��ֵ.
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
 * @brief ��ȡ������.
 * @param sensorX ָ���Ĵ�����, ����GP2Y0E03_ALL�� @ref GP2Y0E03_select ��ѡһ��.
 * @return ������(cm).
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
 * @brief ��ʼ��.
 * @param sensorX ָ���Ĵ�����, ���������� @ref GP2Y0E03_select �����.
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
