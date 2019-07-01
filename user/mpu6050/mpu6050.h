/**
 * @file    mpu6050.c
 * @author  Miaow
 * @version 0.1.1
 * @date    2018/08/31
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of mpu6050:
 *              1. Initialization and setup
 *              2. Get raw data from gyroscope, accelerometer and thermometer
 *              3. DMP operations
 * @note
 *          Pin connection:
 *          ��������������������     ��������������������
 *          ��     PB8��������������SCL  XDA��������X
 *          ��     PB9��������������SDA  AD0��������GND
 *          ��     PD8��������������INT  XCL��������X
 *          ��������������������     �������������������� 
 *          STM32F407       mpu6050
 *          
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#ifndef __MPU6050_H
#define __MPU6050_H

#include "bsp_iic.h"   												  	  
#include "stm32f4xx.h"
#include "utils.h"
#include "math.h"

/**
 * @brief MPU6050��INT���ź��ж���ض���
 */
#define MPU6050_INT_PORT            GPIOD
#define MPU6050_INT_PIN             GPIO_Pin_8
#define MPU6050_INT_GPIO_CLK        RCC_AHB1Periph_GPIOD
#define MPU6050_EXTI_PORT_SOURCE    EXTI_PortSourceGPIOD
#define MPU6050_EXTI_PIN_SOURCE     EXTI_PinSource8
#define MPU6050_EXTI_LINE           EXTI_Line8
#define MPU6050_NVIC_IRQCHANNEL     EXTI9_5_IRQn
#define MPU6050_EXTI_IRQHANDLER     EXTI9_5_IRQHandler

/**
 * @brief MPU6050��IIC������ַ AD0���Žӵ�
 */
#define MPU6050_ADDR				0X68
#define MPU6050_SAMPLE_RATE         200
#define MPU6050_FIFO_RATE           200

typedef enum {
    MPU6050_FSR_250DPS = 0,
    MPU6050_FSR_500DPS,
    MPU6050_FSR_1000DPS,
    MPU6050_FSR_2000DPS
}MPU6050_GyroFsrTypedef;

typedef enum {
    MPU6050_FSR_2G = 0,
    MPU6050_FSR_4G,
    MPU6050_FSR_8G,
    MPU6050_FSR_16G
}MPU6050_AccelFsrTypedef;

typedef enum {
    MPU6050_FILTER_256HZ_NOLPF2 = 0,
    MPU6050_FILTER_188HZ,
    MPU6050_FILTER_98HZ,
    MPU6050_FILTER_42HZ,
    MPU6050_FILTER_20HZ,
    MPU6050_FILTER_10HZ,
    MPU6050_FILTER_5HZ,
    MPU6050_FILTER_2100HZ_NOLPF
}MPU6050_LpfTypedef;

/**
 * @brief ��ʼ��
 * @return 0-�ɹ�; 1-ʧ��
 * @note �����ǡ�2000dps, ���ٶȴ���2g, ������50Hz, 
 *       �жϹ�, I2C��ģʽ��, FIFO��, INT����Ч, ������X��ʱ��
 */
uint8_t MPU6050_Init(void);
/**
 * @brief �����ⲿ�ж�, ��ʼ����MPU6050������
 */
void MPU6050_BeginReceive(void);
/**
 * @brief ��������������
 * @param fsr MPU6050_FSR_XXXXDPS(��MPU6050_GyroFsrTypedef)
 * @return 0-�ɹ�; 1-ʧ��
 */
uint8_t MPU6050_SetGyroFsr(MPU6050_GyroFsrTypedef fsr);
/**
 * @brief ���ü��ٶ�����
 * @param fsr MPU6050_FSR_XXXXG(MPU6050_AccelFsrTypedef)
 * @return 0-�ɹ�; 1-ʧ��
 */
uint8_t MPU6050_SetAccelFsr(MPU6050_AccelFsrTypedef fsr);
/**
 * @brief ���ò�����(�ٶ�Fs=1KHz)
 * @param rate 4~1000Hz
 * @return 0-�ɹ�; 1-ʧ��
 */
uint8_t MPU6050_SetSampleRate(uint16_t rate);
/**
 * @brief ���ֵ�ͨ�˲���
 * @param lpf MPU6050_FILTER_XXXHZ(��MPU6050_LpfTypedef)
 * @return 0-�ɹ�; 1-ʧ��
 */
uint8_t MPU6050_SetLPF(MPU6050_LpfTypedef lpf);
/**
 * @brief ��ȡ�¶�
 * @return ���������¶�ֵ
 */
float MPU6050_GetTemperature(void);
/**
 * @brief ��ȡ������
 * @param gx x��ԭʼ����(������)
 * @param gy y��ԭʼ����(������)
 * @param gz z��ԭʼ����(������)
 * @return 0-�ɹ�; 1-ʧ��
 */
uint8_t MPU6050_GetGyroscope(int16_t *gx, int16_t *gy, int16_t *gz);
/**
 * @brief ��ȡ���ٶȼ�
 * @param gx x��ԭʼ����(������)
 * @param gy y��ԭʼ����(������)
 * @param gz z��ԭʼ����(������)
 * @return 0-�ɹ�; 1-ʧ��
 */
uint8_t MPU6050_GetAccelerometer(int16_t *ax, int16_t *ay, int16_t *az);
/**
 * @brief ����dmpһ���ʼ��
 * @return 0-�ɹ�; ����-ʧ��
 */
uint8_t MPU6050_InitWithDmp(void (* irqHandler)(void));
/**
 * @brief �õ�dmp����������
 * @param pitch ������, ����:0.1��, ��Χ -90��~90��
 * @param pitch �����, ����:0.1��, ��Χ -180��~180��
 * @param pitch �����, ����:0.1��, ��Χ -180��~180��
 * @return 0-�ɹ�; ����-ʧ��
 */
uint8_t MPU6050_GetDmpData(float *pitch, float *roll, float *yaw);

#endif
