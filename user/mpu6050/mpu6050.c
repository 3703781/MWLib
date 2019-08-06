/**
 * @file    mpu6050.c
 * @author  Miaow
 * @version 0.1.2
 * @date    2018/08/31
 * @brief
 *          This file provides functions to manage the following
 *          functionalities of mpu6050:
 *              1. Initialization and setup
 *              2. Get raw data from gyroscope, accelerometer and thermometer
 *              3. DMP operations
 * @note
 *          Minimum version of header file:
 *              0.1.0
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

#include "mpu6050.h"
#include "arm_math.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
 /**
  * @brief 寄存器定义
  */
#define MPU6050_REG_ACCEL_OFFS		    0X06 //!< accel_offs寄存器,可读取版本号,寄存器手册未提到
#define MPU6050_REG_PROD_ID			      0X0C //!< prod id寄存器,在寄存器手册未提到
#define MPU6050_REG_SELF_TESTX		    0X0D //!< 自检寄存器X
#define MPU6050_REG_SELF_TESTY		    0X0E //!< 自检寄存器Y
#define MPU6050_REG_SELF_TESTZ		    0X0F //!< 自检寄存器Z
#define MPU6050_REG_SELF_TESTA		    0X10 //!< 自检寄存器A
#define MPU6050_REG_SAMPLE_RATE		    0X19 //!< 采样频率分频器
#define MPU6050_REG_CFG				        0X1A //!< 配置寄存器
#define MPU6050_REG_GYRO_CFG		      0X1B //!< 陀螺仪配置寄存器
#define MPU6050_REG_ACCEL_CFG		      0X1C //!< 加速度计配置寄存器
#define MPU6050_REG_MOTION_DET		    0X1F //!< 运动检测阀值设置寄存器
#define MPU6050_REG_FIFO_EN			      0X23 //!< FIFO使能寄存器
#define MPU6050_REG_I2CMST_CTRL		    0X24 //!< IIC主机控制寄存器
#define MPU6050_REG_I2CSLV0_ADDR	    0X25 //!< IIC从机0器件地址寄存器
#define MPU6050_REG_I2CSLV0			      0X26 //!< IIC从机0数据地址寄存器
#define MPU6050_REG_I2CSLV0_CTRL	    0X27 //!< IIC从机0控制寄存器
#define MPU6050_REG_I2CSLV1_ADDR	    0X28 //!< IIC从机1器件地址寄存器
#define MPU6050_REG_I2CSLV1			      0X29 //!< IIC从机1数据地址寄存器
#define MPU6050_REG_I2CSLV1_CTRL	    0X2A //!< IIC从机1控制寄存器
#define MPU6050_REG_I2CSLV2_ADDR	    0X2B //!< IIC从机2器件地址寄存器
#define MPU6050_REG_I2CSLV2			      0X2C //!< IIC从机2数据地址寄存器
#define MPU6050_REG_I2CSLV2_CTRL	    0X2D //!< IIC从机2控制寄存器
#define MPU6050_REG_I2CSLV3_ADDR	    0X2E //!< IIC从机3器件地址寄存器
#define MPU6050_REG_I2CSLV3			      0X2F //!< IIC从机3数据地址寄存器
#define MPU6050_REG_I2CSLV3_CTRL	    0X30 //!< IIC从机3控制寄存器
#define MPU6050_REG_I2CSLV4_ADDR	    0X31 //!< IIC从机4器件地址寄存器
#define MPU6050_REG_I2CSLV4			      0X32 //!< IIC从机4数据地址寄存器
#define MPU6050_REG_I2CSLV4_DO		    0X33 //!< IIC从机4写数据寄存器
#define MPU6050_REG_I2CSLV4_CTRL	    0X34 //!< IIC从机4控制寄存器
#define MPU6050_REG_I2CSLV4_DI		    0X35 //!< IIC从机4读数据寄存器
#define MPU6050_REG_I2CMST_STA		    0X36 //!< IIC主机状态寄存器
#define MPU6050_REG_INTBP_CFG		      0X37 //!< 中断/旁路设置寄存器
#define MPU6050_REG_INT_EN			      0X38 //!< 中断使能寄存器
#define MPU6050_REG_INT_STA			      0X3A //!< 中断状态寄存器
#define MPU6050_REG_ACCEL_XOUTH		    0X3B //!< 加速度值,X轴高8位寄存器
#define MPU6050_REG_ACCEL_XOUTL		    0X3C //!< 加速度值,X轴低8位寄存器
#define MPU6050_REG_ACCEL_YOUTH		    0X3D //!< 加速度值,Y轴高8位寄存器
#define MPU6050_REG_ACCEL_YOUTL		    0X3E //!< 加速度值,Y轴低8位寄存器
#define MPU6050_REG_ACCEL_ZOUTH		    0X3F //!< 加速度值,Z轴高8位寄存器
#define MPU6050_REG_ACCEL_ZOUTL		    0X40 //!< 加速度值,Z轴低8位寄存器
#define MPU6050_REG_TEMP_OUTH		      0X41 //!< 温度值高八位寄存器
#define MPU6050_REG_TEMP_OUTL		      0X42 //!< 温度值低8位寄存器
#define MPU6050_REG_GYRO_XOUTH		    0X43 //!< 陀螺仪值,X轴高8位寄存器
#define MPU6050_REG_GYRO_XOUTL		    0X44 //!< 陀螺仪值,X轴低8位寄存器
#define MPU6050_REG_GYRO_YOUTH		    0X45 //!< 陀螺仪值,Y轴高8位寄存器
#define MPU6050_REG_GYRO_YOUTL		    0X46 //!< 陀螺仪值,Y轴低8位寄存器
#define MPU6050_REG_GYRO_ZOUTH		    0X47 //!< 陀螺仪值,Z轴高8位寄存器
#define MPU6050_REG_GYRO_ZOUTL		    0X48 //!< 陀螺仪值,Z轴低8位寄存器
#define MPU6050_REG_I2CSLV0_DO		    0X63 //!< IIC从机0数据寄存器
#define MPU6050_REG_I2CSLV1_DO		    0X64 //!< IIC从机1数据寄存器
#define MPU6050_REG_I2CSLV2_DO		    0X65 //!< IIC从机2数据寄存器
#define MPU6050_REG_I2CSLV3_DO		    0X66 //!< IIC从机3数据寄存器
#define MPU6050_REG_I2CMST_DELAY	    0X67 //!< IIC主机延时管理寄存器
#define MPU6050_REG_SIGPATH_RST		    0X68 //!< 信号通道复位寄存器
#define MPU6050_REG_MDETECT_CTRL	    0X69 //!< 运动检测控制寄存器
#define MPU6050_REG_USER_CTRL		      0X6A //!< 用户控制寄存器
#define MPU6050_REG_PWR_MGMT1		      0X6B //!< 电源管理寄存器1
#define MPU6050_REG_PWR_MGMT2		      0X6C //!< 电源管理寄存器2 
#define MPU6050_REG_FIFO_CNTH		      0X72 //!< FIFO计数寄存器高八位
#define MPU6050_REG_FIFO_CNTL		      0X73 //!< FIFO计数寄存器低八位
#define MPU6050_REG_FIFO_RW			      0X74 //!< FIFO读写寄存器
#define MPU6050_REG_DEVICE_ID		      0X75 //!< 器件ID寄存器

#define MPU6050_Q30                   1073741824.0 //!<q30格式
#define MPU6050_Q16                   65536.0 //!< q16格式

  /**
   * @brief 陀螺仪方向设置
   */
static int8_t MPU6050_GyroOrientation[9] = { 1, 0, 0,
                                             0, 1, 0,
                                             0, 0, 1 };
static MPU6050_DataArrivalHandler __dataArrivalHandler = NULL; //!< 外部中断回调函数

/**
 * @brief 设置陀螺仪量程
 * @param fsr MPU6050_FSR_XXXXDPS(见MPU6050_GyroFsrTypedef)
 * @return 0-成功; 1-失败
 */
uint8_t MPU6050_SetGyroFsr(MPU6050_GyroFsrTypedef fsr)
{
  return IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_GYRO_CFG, fsr << 3);//设置陀螺仪满量程范围  
}

/**
 * @brief 设置加速度量程
 * @param fsr MPU6050_FSR_XXXXG(MPU6050_AccelFsrTypedef)
 * @return 0-成功; 1-失败
 */
uint8_t MPU6050_SetAccelFsr(MPU6050_AccelFsrTypedef fsr)
{
  return IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_ACCEL_CFG, fsr << 3);//设置加速度传感器满量程范围  
}

/**
 * @brief 设置采样率(假定Fs=1KHz)
 * @param rate 4~1000Hz
 * @return 0-成功; 1-失败
 */
uint8_t MPU6050_SetSampleRate(uint16_t rate)
{
  uint16_t lpf = rate >> 1;
  MPU6050_LpfTypedef data;
  if (rate > 1000 || rate < 4)
    return 1;
  if (IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_SAMPLE_RATE, 1000 / rate - 1))    //设置数字低通滤波器
    return 1;
  if (lpf >= 188)
    data = MPU6050_FILTER_188HZ;
  else if (lpf >= 98)
    data = MPU6050_FILTER_98HZ;
  else if (lpf >= 42)
    data = MPU6050_FILTER_42HZ;
  else if (lpf >= 20)
    data = MPU6050_FILTER_20HZ;
  else if (lpf >= 10)
    data = MPU6050_FILTER_10HZ;
  else
    data = MPU6050_FILTER_5HZ;
  return MPU6050_SetLPF(data);//设置LPF为采样率的一半
}

/**
 * @brief 数字低通滤波器
 * @param lpf MPU6050_FILTER_XXXHZ(见MPU6050_LpfTypedef)
 * @return 0-成功; 1-失败
 */
uint8_t MPU6050_SetLPF(MPU6050_LpfTypedef lpf)
{
  return IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_CFG, lpf);//设置数字低通滤波器  
}

/**
 * @brief 初始化
 * @return 0-成功; 1-失败
 * @note 陀螺仪±2000dps, 加速度传±2g, 采样率50Hz,
 *       中断关, I2C主模式关, FIFO关, INT低有效, 陀螺仪X轴时钟
 */
uint8_t MPU6050_Init()
{
  IIC_Init();//初始化IIC总线
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_PWR_MGMT1, 0X80);//复位MPU6050
  UTILS_DelayMs(100);
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_PWR_MGMT1, 0X00);//唤醒MPU6050 
  MPU6050_SetGyroFsr(MPU6050_FSR_2000DPS);//陀螺仪±2000dps
  MPU6050_SetAccelFsr(MPU6050_FSR_2G);//加速度传±2g
  MPU6050_SetSampleRate(MPU6050_SAMPLE_RATE);//设置采样率
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_INT_EN, 0X00);//关闭所有中断
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_USER_CTRL, 0X00);//I2C主模式关闭
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_FIFO_EN, 0X00);//关闭FIFO
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_INTBP_CFG, 0X80);//INT引脚低电平有效
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_PWR_MGMT1, 0X01);//设置CLKSEL X轴PLL为参考
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_PWR_MGMT2, 0X00);//加速度与陀螺仪都工作

  return IIC_ReadRegByte(MPU6050_ADDR, MPU6050_REG_DEVICE_ID) != MPU6050_ADDR;
}

/**
 * @brief 读取温度
 * @return 返回摄氏温度值
 */
float MPU6050_GetTemperature(void)
{
  uint8_t data[2];
  int16_t raw;
  IIC_ReadRegBytes(MPU6050_ADDR, MPU6050_REG_TEMP_OUTH, 2, data);
  raw = ((int16_t)data[0] << 8) | data[1];
  return 36.53f + (float)raw / 340.0f;
}

/**
 * @brief 读取陀螺仪
 * @param gx x轴原始读数(带符号)
 * @param gy y轴原始读数(带符号)
 * @param gz z轴原始读数(带符号)
 * @return 0-成功; 1-失败
 */
uint8_t MPU6050_GetGyroscope(int16_t * gx, int16_t * gy, int16_t * gz)
{
  uint8_t data[6], res;
  res = IIC_ReadRegBytes(MPU6050_ADDR, MPU6050_REG_GYRO_XOUTH, 6, data);
  if (!res)
  {
    *gx = ((uint16_t)data[0] << 8) | data[1];
    *gy = ((uint16_t)data[2] << 8) | data[3];
    *gz = ((uint16_t)data[4] << 8) | data[5];
    return 0;
  }
  return 1;
}

/**
 * @brief 初始化外部中断
 * @param irqHandler 外部中断回调函数
 */
static inline void MPU6050_InitExti(MPU6050_DataArrivalHandler dataArrivalHandler)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  //GPIO
  RCC_AHB1PeriphClockCmd(MPU6050_INT_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin = MPU6050_INT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(MPU6050_INT_PORT, &GPIO_InitStructure);//初始化
  //SYSCFG
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig(MPU6050_EXTI_PORT_SOURCE, MPU6050_EXTI_PIN_SOURCE);
  //EXTI
  EXTI_InitStructure.EXTI_Line = MPU6050_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  //IRQ
  __dataArrivalHandler = dataArrivalHandler;
}

/**
 * @brief 开启外部中断, 开始接收MPU6050的数据
 */
void MPU6050_BeginReceive()
{
  NVIC_InitTypeDef NVIC_InitStructure;
  //NVIC
  NVIC_InitStructure.NVIC_IRQChannel = MPU6050_NVIC_IRQCHANNEL;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief 读取加速度计
 * @param gx x轴原始读数(带符号)
 * @param gy y轴原始读数(带符号)
 * @param gz z轴原始读数(带符号)
 * @return 0-成功; 1-失败
 */
uint8_t MPU6050_GetAccelerometer(int16_t * ax, int16_t * ay, int16_t * az)
{
  uint8_t data[6], res;
  res = IIC_ReadRegBytes(MPU6050_ADDR, MPU6050_REG_ACCEL_XOUTH, 6, data);
  if (!res)
  {
    *ax = ((uint16_t)data[0] << 8) | data[1];
    *ay = ((uint16_t)data[2] << 8) | data[3];
    *az = ((uint16_t)data[4] << 8) | data[5];
    return 0;
  }
  return 1;
}

/**
 * @brief MPU6050自测试
 * @return 0-成功; 1-失败
 */
static inline uint8_t MPU6050_RunSelfTest()
{
  int32_t result;
  long gyro[3], accel[3];
  result = mpu_run_self_test(gyro, accel);
  if ((result & 0x03) == 0x03)
  {
    float sens;
    uint16_t accel_sens;
    mpu_get_gyro_sens(&sens);
    gyro[0] = (long)(gyro[0] * sens);
    gyro[1] = (long)(gyro[1] * sens);
    gyro[2] = (long)(gyro[2] * sens);
    dmp_set_gyro_bias(gyro);
    mpu_get_accel_sens(&accel_sens);
    accel[0] *= accel_sens;
    accel[1] *= accel_sens;
    accel[2] *= accel_sens;
    dmp_set_accel_bias(accel);
    return 0;
  }
  else
    return 1;
}

/**
 * @brief 方向转换
 */
static inline int16_t MPU6050_InvRow2Scale(const int8_t * row)
{
  int16_t b;

  if (row[0] > 0)
    b = 0;
  else if (row[0] < 0)
    b = 4;
  else if (row[1] > 0)
    b = 1;
  else if (row[1] < 0)
    b = 5;
  else if (row[2] > 0)
    b = 2;
  else if (row[2] < 0)
    b = 6;
  else
    b = 7;      // error
  return b;
}

/**
 * @brief 陀螺仪方向控制
 * @param matrix 方向矩阵
 * @return 0-成功; 1-失败
 */
static inline uint16_t MPU6050_InvOrientationMatrix2Scalar(const int8_t * matrix)
{
  uint16_t scalar;
  /*
     XYZ  010_001_000 Identity Matrix
     XZY  001_010_000
     YXZ  010_000_001
     YZX  000_010_001
     ZXY  001_000_010
     ZYX  000_001_010
   */
  scalar = MPU6050_InvRow2Scale(matrix);
  scalar |= MPU6050_InvRow2Scale(matrix + 3) << 3;
  scalar |= MPU6050_InvRow2Scale(matrix + 6) << 6;
  return scalar;
}

/**
 * @brief 连着dmp一起初始化
 * @return 0-成功; 其他-失败
 */
uint8_t MPU6050_InitWithDmp(MPU6050_DataArrivalHandler dataArrivalHandler)
{
  IIC_Init();//初始化IIC总线
  if (!mpu_init())//初始化MPU6050
  {
    //设置所需要的传感器
    if (mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
      return 1;
    //设置FIFO
    if (mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
      return 2;
    //加载dmp固件
    if (dmp_load_motion_driver_firmware())
      return 4;
    //设置陀螺仪方向
    if (dmp_set_orientation(MPU6050_InvOrientationMatrix2Scalar(MPU6050_GyroOrientation)))
      return 5;
    //设置dmp功能
    if (dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
      DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
      DMP_FEATURE_GYRO_CAL))
      return 6;
    //设置DMP输出速率(最大不超过200Hz)
    if (dmp_set_fifo_rate(MPU6050_FIFO_RATE))
      return 7;
    //自检
    if (MPU6050_RunSelfTest())
      return 8;
    //使能DMP
    if (mpu_set_dmp_state(1))
      return 9;
  }
  MPU6050_InitExti(dataArrivalHandler);
  return 0;
}

/**
 * @brief 得到dmp处理后的数据
 * @param pitch 俯仰角, 精度:0.1°, 范围 -90°~90°
 * @param pitch 横滚角, 精度:0.1°, 范围 -180°~180°
 * @param pitch 航向角, 精度:0.1°, 范围 -180°~180°
 * @return 0-成功; 其他-失败
 */
uint8_t MPU6050_GetDmpData(float* pitch, float* roll, float* yaw)
{
  float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
  unsigned long sensor_timestamp;
  int16_t gyro[3], accel[3], sensors;
  uint8_t more;
  long quat[4];
  if (dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more))
    return 1;
  /* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
   * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
  **/
  /*if (sensors & INV_XYZ_GYRO )
  send_packet(PACKET_TYPE_GYRO, gyro);
  if (sensors & INV_XYZ_ACCEL)
  send_packet(PACKET_TYPE_ACCEL, accel); */
  /* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
   * The orientation is set by the scalar passed to dmp_set_orientation during initialization.
  **/
  if (!(sensors & INV_WXYZ_QUAT))
    return 2;
  q0 = quat[0] / MPU6050_Q30;	//q30格式转换为浮点数
  q1 = quat[1] / MPU6050_Q30;
  q2 = quat[2] / MPU6050_Q30;
  q3 = quat[3] / MPU6050_Q30;

  *pitch = asinf(2.0f * q0 * q2 - 2.0f * q1 * q3) * 57.29577951308232286464772187173366546630859375f;
  *roll = atan2f(2.0f * q2 * q3 + 2.0f * q0 * q1, 1.0f - 2.0f * q1 * q1 - 2.0f * q2 * q2) * 57.29577951308232286464772187173366546630859375f;
  *yaw = atan2f(2.0f * q1 * q2 + 2.0f * q0 * q3, q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.29577951308232286464772187173366546630859375f;
  return 0;
}

/**
 * @brief MPU6050的外部中断服务函数
 */
void MPU6050_EXTI_IRQHANDLER()
{
  if ((EXTI->PR & MPU6050_EXTI_LINE) != (uint32_t)RESET)
  {
    float pitch, roll, yaw;
    EXTI->PR = MPU6050_EXTI_LINE;
    MPU6050_GetDmpData(&pitch, &roll, &yaw);
    __dataArrivalHandler(pitch, roll, yaw);
  }
}
