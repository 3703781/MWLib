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
 *          ┌────────┐     ┌────────┐
 *          │     PB8├─────┤SCL  XDA├───X
 *          │     PB9├─────┤SDA  AD0├───GND
 *          │     PD8├─────┤INT  XCL├───X
 *          └────────┘     └────────┘ 
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
 * @brief MPU6050的INT引脚和中断相关定义
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
 * @brief MPU6050的IIC器件地址 AD0引脚接地
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
 * @brief 初始化
 * @return 0-成功; 1-失败
 * @note 陀螺仪±2000dps, 加速度传±2g, 采样率50Hz, 
 *       中断关, I2C主模式关, FIFO关, INT低有效, 陀螺仪X轴时钟
 */
uint8_t MPU6050_Init(void);
/**
 * @brief 开启外部中断, 开始接收MPU6050的数据
 */
void MPU6050_BeginReceive(void);
/**
 * @brief 设置陀螺仪量程
 * @param fsr MPU6050_FSR_XXXXDPS(见MPU6050_GyroFsrTypedef)
 * @return 0-成功; 1-失败
 */
uint8_t MPU6050_SetGyroFsr(MPU6050_GyroFsrTypedef fsr);
/**
 * @brief 设置加速度量程
 * @param fsr MPU6050_FSR_XXXXG(MPU6050_AccelFsrTypedef)
 * @return 0-成功; 1-失败
 */
uint8_t MPU6050_SetAccelFsr(MPU6050_AccelFsrTypedef fsr);
/**
 * @brief 设置采样率(假定Fs=1KHz)
 * @param rate 4~1000Hz
 * @return 0-成功; 1-失败
 */
uint8_t MPU6050_SetSampleRate(uint16_t rate);
/**
 * @brief 数字低通滤波器
 * @param lpf MPU6050_FILTER_XXXHZ(见MPU6050_LpfTypedef)
 * @return 0-成功; 1-失败
 */
uint8_t MPU6050_SetLPF(MPU6050_LpfTypedef lpf);
/**
 * @brief 读取温度
 * @return 返回摄氏温度值
 */
float MPU6050_GetTemperature(void);
/**
 * @brief 读取陀螺仪
 * @param gx x轴原始读数(带符号)
 * @param gy y轴原始读数(带符号)
 * @param gz z轴原始读数(带符号)
 * @return 0-成功; 1-失败
 */
uint8_t MPU6050_GetGyroscope(int16_t *gx, int16_t *gy, int16_t *gz);
/**
 * @brief 读取加速度计
 * @param gx x轴原始读数(带符号)
 * @param gy y轴原始读数(带符号)
 * @param gz z轴原始读数(带符号)
 * @return 0-成功; 1-失败
 */
uint8_t MPU6050_GetAccelerometer(int16_t *ax, int16_t *ay, int16_t *az);
/**
 * @brief 连着dmp一起初始化
 * @return 0-成功; 其他-失败
 */
uint8_t MPU6050_InitWithDmp(void (* irqHandler)(void));
/**
 * @brief 得到dmp处理后的数据
 * @param pitch 俯仰角, 精度:0.1°, 范围 -90°~90°
 * @param pitch 横滚角, 精度:0.1°, 范围 -180°~180°
 * @param pitch 航向角, 精度:0.1°, 范围 -180°~180°
 * @return 0-成功; 其他-失败
 */
uint8_t MPU6050_GetDmpData(float *pitch, float *roll, float *yaw);

#endif
