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
 *          ┌────────┐     ┌────────┐
 *          │     PB8├─────┤SCL  XDA├───X
 *          │     PB9├─────┤SDA  AD0├───GND
 *          │     PD8├─────┤INT  XCL├───X
 *          └────────┘     └────────┘          
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
 * @brief MPU9250的INT引脚和中断相关定义
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
 * @brief MPU9250的IIC器件地址 AD0引脚接地
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
 * @brief 设置陀螺仪量程
 * @param fsr MPU9250_FSR_XXXXDPS(见MPU9250_GyroFsrTypedef)
 * @return 0-成功; -1-失败
 */
int8_t MPU9250_SetGyroFsr(MPU9250_GyroFsrTypedef fsr);
/**
 * @brief 设置加速度量程
 * @param fsr MPU9250_FSR_XXXXG(MPU9250_AccelFsrTypedef)
 * @return 0-成功; -1-失败
 */
int8_t MPU9250_SetAccelFsr(MPU9250_AccelFsrTypedef fsr);
/**
 * @brief 设置采样率(假定Fs=1KHz)
 * @param rate 4~1000Hz
 * @return 0-成功; -1-失败
 */
extern inline int8_t MPU9250_SetSampleRate(uint16_t rate);
/**
 * @brief 数字低通滤波器
 * @param lpf MPU9250_FILTER_XXXHZ(见MPU9250_LpfTypedef)
 * @return 0-成功; -1-失败
 */
int8_t MPU9250_SetLPF(MPU9250_LpfTypedef lpf);
/**
 * @brief 初始化
 * @param irqHandler 外部中断回调函数
 * @return 0-成功; 1-失败
 * @note 陀螺仪±2000dps, 加速度传±2g, 采样率50Hz, 
 *       中断关, I2C主模式关, FIFO关, INT低有效, 陀螺仪X轴时钟
 */
extern inline int8_t MPU9250_Init(void (* irqHandler)(void));
/**
 * @brief 读取温度
 * @return 返回摄氏温度值
 */
float MPU9250_GetTemperature(void);
/**
 * @brief 读取陀螺仪
 * @param gx x轴原始读数(带符号)
 * @param gy y轴原始读数(带符号)
 * @param gz z轴原始读数(带符号)
 * @return 0-成功; -1-失败
 */
int8_t MPU9250_GetGyroscope(int16_t *gx, int16_t *gy, int16_t *gz);
/**
 * @brief 读取电子罗盘
 * @param mx x轴原始读数(带符号)
 * @param my y轴原始读数(带符号)
 * @param mz z轴原始读数(带符号)
 * @return 0-成功; -1-失败
 */
int8_t MPU9250_GetCompass(int16_t *mx, int16_t *my, int16_t *mz);
/**
 * @brief 读取加速度计
 * @param gx x轴原始读数(带符号)
 * @param gy y轴原始读数(带符号)
 * @param gz z轴原始读数(带符号)
 * @return 0-成功; -1-失败
 */
int8_t MPU9250_GetAccelerometer(int16_t *ax, int16_t *ay, int16_t *az);
/**
 * @brief 连着dmp一起初始化
 * @param irqHandler 外部中断回调函数
 * @return 0-成功; 其他-失败
 */
int8_t MPU9250_InitWithDmp(void (* irqHandler)(void));
/**
 * @brief 得到dmp处理后的数据
 * @param pitch 俯仰角, 精度:0.1°, 范围 -90°~90°
 * @param pitch 横滚角, 精度:0.1°, 范围 -180°~180°
 * @param pitch 航向角, 精度:0.1°, 范围 -180°~180°
 * @return 0-成功; 其他-失败
 */
int8_t MPU9250_GetDmpData(float *pitch, float *roll, float *yaw);


int8_t MPU9250_GetEulerFromCompass(float *yaw);

#endif
