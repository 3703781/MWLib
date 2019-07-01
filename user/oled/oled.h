/**
 * @file    oled.h
 * @author  Miaow, Evk123
 * @version 0.1.0
 * @date    2018/10/05
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of 0.96" OLED display:
 *              1. Initialization
 *              2. Display formatted strings, pictures and Chinese characters
 *              3. Turn on/off the screen
 *              4. Show logs
 * @note
 *          Minimum version of source file:
 *              0.1.0
 *          Pin connection:
 *          ��������������������     ��������������������
 *          ��    PC10��������������SCL     ��
 *          ��    PC11��������������SDA     ��
 *          ��������������������     ��������������������
 *          STM32F407      0.96" OLED
 *          
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
 
#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"


#define OLED_SCL_PORT               GPIOC
#define OLED_SCL_PIN                GPIO_Pin_10
#define OLED_SCL_GPIO_CLK           RCC_AHB1Periph_GPIOC
#define OLED_SDA_PORT               GPIOC
#define OLED_SDA_PIN                GPIO_Pin_11
#define OLED_SDA_GPIO_CLK           RCC_AHB1Periph_GPIOC
#define OLED_IIC_In()                   OLED_SDA_PORT->MODER &= ~0x00C00000;OLED_SDA_PORT->MODER |= 0x00000000//����ģʽ
#define OLED_IIC_Out()                  OLED_SDA_PORT->MODER &= ~0x00C00000;OLED_SDA_PORT->MODER |= 0x00400000//���ģʽ

#define OLED_IIC_ADDRESS            0x78
#define	OLED_BRIGHTNESS             255


/**
 * @brief oled���.
 * @note stringX, stringY, stringClear, stringContinuousΪ������.
 */
typedef struct {
    uint8_t stringX;//�ַ�����ʼ��λ��(0~20)
    uint8_t stringY;//�ַ�����ʼ��λ��(0~7)
    FunctionalState stringClear;//�Ƿ�����ϴ��ַ���, ���ޱ���OLED_DisplayFormat����
    FunctionalState stringContinuous;//�����ϴν���λ�ô�ӡ
    uint8_t __stringX;//��ǰ��ӡ�ַ�λ��
    uint8_t __stringY;//��ǰ��ӡ�ַ�λ��
    uint8_t __positionX;//��ǰ��ʾ����
    uint8_t __positionY;//��ǰ��ʾ����
    uint8_t __stringLastBeignX;//�ϴδ�ӡ�ַ�����ʼλ��
    uint8_t __stringLastBeignY;//�ϴδ�ӡ�ַ�����ʼλ��
    uint8_t __stringLastEndX;//�ϴδ�ӡ�ַ�������λ��
    uint8_t __stringLastEndY;//�ϴδ�ӡ�ַ�������λ��
    char __string[100];//ʵ�ʴ�ӡ���ַ���
}OLED_HandleTypedef;


void OLED_Init(OLED_HandleTypedef *oledHandle);
void OLED_TurnOn(void);
void OLED_TurnOff(void);
void OLED_Clear(OLED_HandleTypedef *oledHandle);
void OLED_Blank(void);
void OLED_DisplayFormat(OLED_HandleTypedef *oledHandle, const char *format, ...);
void OLED_DisplayLog(OLED_HandleTypedef *oledHandle, const char *format, ...);

#endif 
