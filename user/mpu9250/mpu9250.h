/**
 * @file    mpu9250.h
 * @author  Miaow
 * @version 0.1.1
 * @date    2018/09/08
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of mpu9250:
 *              1. Initialization and setup
 *              2. Get raw data from gyroscope, accelerometer, magnetometer and thermometer
 *              3. DMP operations
 * @note
 *          Recommanded pin connection:
 *          ��������������������     ��������������������
 *          ��     PB8��������������SCL  XDA��������X
 *          ��     PB9��������������SDA  AD0��������GND
 *          ��     PD8��������������INT  XCL��������X
 *          ��������������������     ��������������������          
 *          STM32F407       mpu9250
 *          
 *          The source code repository is not available on GitHub now:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#ifndef __MPU9250_H
#define __MPU9250_H

#include "bsp_iic.h"   												  	  
#include "sys.h"
#include "delay.h"
#include "math.h"

/**
 * @brief MPU9250��INT���ź��ж���ض���
 */
#define MPU9250_INT_PORT            GPIOD
#define MPU9250_INT_PIN             GPIO_Pin_8
#define MPU9250_INT_GPIO_CLK        RCC_AHB1Periph_GPIOD
#define MPU9250_EXTI_PORT_SOURCE    EXTI_PortSourceGPIOD
#define MPU9250_EXTI_PIN_SOURCE     EXTI_PinSource8
#define MPU9250_EXTI_LINE           EXTI_Line8
#define MPU9250_NVIC_IRQCHANNEL     EXTI9_5_IRQn
#define MPU9250_EXTI_IRQHANDLER     EXTI9_5_IRQHandler

/**
 * @brief MPU9250��IIC������ַ AD0���Žӵ�
 */
#define MPU9250_ADDR				0X68
#define MPU9250_SAMPLE_RATE         200
#define MPU9250_FIFO_RATE           200
typedef enum {
    MPU9250_FSR_250DPS = 0,
    MPU9250_FSR_500DPS,
    MPU9250_FSR_1000DPS,
    MPU9250_FSR_2000DPS
}MPU9250_GyroFsrTypedef;

typedef enum {
    MPU9250_FSR_2G = 0,
    MPU9250_FSR_4G,
    MPU9250_FSR_8G,
    MPU9250_FSR_16G
}MPU9250_AccelFsrTypedef;

typedef enum {
    MPU9250_FILTER_188HZ = 0,
    MPU9250_FILTER_98HZ,
    MPU9250_FILTER_42HZ,
    MPU9250_FILTER_20HZ,
    MPU9250_FILTER_10HZ,
    MPU9250_FILTER_5HZ
}MPU9250_LpfTypedef;

/**
 * @brief ��������������
 * @param fsr MPU9250_FSR_XXXXDPS(��MPU9250_GyroFsrTypedef)
 * @return 0-�ɹ�; -1-ʧ��
 */
int8_t MPU9250_SetGyroFsr(MPU9250_GyroFsrTypedef fsr);
/**
 * @brief ���ü��ٶ�����
 * @param fsr MPU9250_FSR_XXXXG(MPU9250_AccelFsrTypedef)
 * @return 0-�ɹ�; -1-ʧ��
 */
int8_t MPU9250_SetAccelFsr(MPU9250_AccelFsrTypedef fsr);
/**
 * @brief ���ò�����(�ٶ�Fs=1KHz)
 * @param rate 4~1000Hz
 * @return 0-�ɹ�; -1-ʧ��
 */
extern inline int8_t MPU9250_SetSampleRate(uint16_t rate);
/**
 * @brief ���ֵ�ͨ�˲���
 * @param lpf MPU9250_FILTER_XXXHZ(��MPU9250_LpfTypedef)
 * @return 0-�ɹ�; -1-ʧ��
 */
int8_t MPU9250_SetLPF(MPU9250_LpfTypedef lpf);
/**
 * @brief ��ʼ��
 * @param irqHandler �ⲿ�жϻص�����
 * @return 0-�ɹ�; 1-ʧ��
 * @note �����ǡ�2000dps, ���ٶȴ���2g, ������50Hz, 
 *       �жϹ�, I2C��ģʽ��, FIFO��, INT����Ч, ������X��ʱ��
 */
extern inline int8_t MPU9250_Init(void (* irqHandler)(void));
/**
 * @brief ��ȡ�¶�
 * @return ���������¶�ֵ
 */
float MPU9250_GetTemperature(void);
/**
 * @brief ��ȡ������
 * @param gx x��ԭʼ����(������)
 * @param gy y��ԭʼ����(������)
 * @param gz z��ԭʼ����(������)
 * @return 0-�ɹ�; -1-ʧ��
 */
int8_t MPU9250_GetGyroscope(int16_t *gx, int16_t *gy, int16_t *gz);
/**
 * @brief ��ȡ��������
 * @param mx x��ԭʼ����(������)
 * @param my y��ԭʼ����(������)
 * @param mz z��ԭʼ����(������)
 * @return 0-�ɹ�; -1-ʧ��
 */
int8_t MPU9250_GetCompass(int16_t *mx, int16_t *my, int16_t *mz);
/**
 * @brief ��ȡ���ٶȼ�
 * @param gx x��ԭʼ����(������)
 * @param gy y��ԭʼ����(������)
 * @param gz z��ԭʼ����(������)
 * @return 0-�ɹ�; -1-ʧ��
 */
int8_t MPU9250_GetAccelerometer(int16_t *ax, int16_t *ay, int16_t *az);
/**
 * @brief ����dmpһ���ʼ��
 * @param irqHandler �ⲿ�жϻص�����
 * @return 0-�ɹ�; ����-ʧ��
 */
int8_t MPU9250_InitWithDmp(void (* irqHandler)(void));
/**
 * @brief �õ�dmp����������
 * @param pitch ������, ����:0.1��, ��Χ -90��~90��
 * @param pitch �����, ����:0.1��, ��Χ -180��~180��
 * @param pitch �����, ����:0.1��, ��Χ -180��~180��
 * @return 0-�ɹ�; ����-ʧ��
 */
int8_t MPU9250_GetDmpData(float *pitch, float *roll, float *yaw);


int8_t MPU9250_GetEulerFromCompass(float *yaw);

#endif
