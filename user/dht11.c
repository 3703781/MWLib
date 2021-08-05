#include "dht11.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//DHT11 ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

//��λDHT11
void DHT11_Rst(void)
{
	DHT11_GPIO_OUT();							   //SET OUTPUT
	DHT11_GPIO_PORT->BSRRH |= DHT11_GPIO_PIN; //����DQ
	UTILS_DelayMs(20);							   //��������18ms
	DHT11_GPIO_IN(); //DQ=1
	UTILS_DelayUs(30);							   //��������20~40us
}
//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
u8 DHT11_Check(void)
{
	u8 retry = 0;
	DHT11_GPIO_IN();					   //SET INPUT
	while ((DHT11_GPIO_PORT->IDR & DHT11_GPIO_PIN) && retry < 200) //DHT11������40~80us
	{
		retry++;
		UTILS_DelayUs(1);
	};
	if (retry >= 200)
		return 1;
	else
		retry = 0;
	while (!(DHT11_GPIO_PORT->IDR & DHT11_GPIO_PIN) && retry < 200) //DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		UTILS_DelayUs(1);
	};
	if (retry >= 200)
		return 1;
	return 0;
}
//��DHT11��ȡһ��λ
//����ֵ��1/0
u8 DHT11_Read_Bit(void)
{
	u8 retry = 0;
	DHT11_GPIO_IN();
	while ((DHT11_GPIO_PORT->IDR & DHT11_GPIO_PIN) && retry < 100) //�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		UTILS_DelayUs(1);
	}
	retry = 0;
	while (!(DHT11_GPIO_PORT->IDR & DHT11_GPIO_PIN) && retry < 100) //�ȴ���ߵ�ƽ
	{
		retry++;
		UTILS_DelayUs(1);
	}
	UTILS_DelayUs(40); //�ȴ�40us
	if (DHT11_GPIO_PORT->IDR & DHT11_GPIO_PIN)
		return 1;
	else
		return 0;
}
//��DHT11��ȡһ���ֽ�
//����ֵ������������
u8 DHT11_Read_Byte(void)
{
	u8 i, dat;
	dat = 0;
	for (i = 0; i < 8; i++)
	{
		dat <<= 1;
		dat |= DHT11_Read_Bit();
	}
	return dat;
}
//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
u8 DHT11_Read_Data(u8 *temp, u8 *humi)
{
	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if (DHT11_Check() == 0)
	{
		for (i = 0; i < 5; i++) //��ȡ40λ����
		{
			buf[i] = DHT11_Read_Byte();
		}
		if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
		{
			*humi = buf[0];
			*temp = buf[2];
		}
	}
	else
		return 1;
	return 0;
}
//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����
u8 DHT11_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(DHT11_GPIO_CLK, ENABLE); //ʹ��GPIOGʱ��

	//GPIOF9,F10��ʼ������
	GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	  //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	  //����
	GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);			  //��ʼ��
	DHT11_Rst();
	return DHT11_Check();
}
