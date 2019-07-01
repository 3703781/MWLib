/**
 * @file    mpu9250.c
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
 *          Minimum version of header file:
 *              0.1.1
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

#include "mpu9250.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "math.h"
#include "stdio.h"
/**
 * @brief 寄存器定义
 */
#define MPU9250_REG_SELF_TEST_X_GYRO        0X00    //陀螺仪自检寄存器X
#define MPU9250_REG_SELF_TEST_Y_GYRO        0X01    //陀螺仪自检寄存器Y
#define MPU9250_REG_SELF_TEST_Z_GYRO        0X02    //陀螺仪自检寄存器Z
#define MPU9250_REG_ACCEL_OFFS		        0X06	//accel_offs寄存器,可读取版本号,寄存器手册未提到
#define MPU9250_REG_PROD_ID			        0X0C	//prod id寄存器,在寄存器手册未提到
#define MPU9250_REG_SELF_TEST_X_ACCEL		0X0D	//加速度计自检寄存器X
#define MPU9250_REG_SELF_TEST_Y_ACCEL	    0X0E	//加速度计自检寄存器Y
#define MPU9250_REG_SELF_TEST_Z_ACCEL       0X0F	//加速度计自检寄存器Z
#define MPU9250_REG_XG_OFFSETH              0X13    //陀螺仪校准寄存器X
#define MPU9250_REG_XG_OFFSETL              0X14    //陀螺仪校准寄存器X
#define MPU9250_REG_YG_OFFSETH              0X15    //陀螺仪校准寄存器y
#define MPU9250_REG_YG_OFFSETL              0X16    //陀螺仪校准寄存器y
#define MPU9250_REG_ZG_OFFSETH              0X17    //陀螺仪校准寄存器z
#define MPU9250_REG_ZG_OFFSETL              0X18    //陀螺仪校准寄存器z
#define MPU9250_REG_SAMPLE_DIV	            0X19	//采样频率分频器
#define MPU9250_REG_CFG			            0X1A	//配置寄存器
#define MPU9250_REG_GYRO_CFG	            0X1B	//陀螺仪配置寄存器
#define MPU9250_REG_ACCEL_CFG	            0X1C	//加速度计配置寄存器
#define MPU9250_REG_ACCEL_CFG2	            0X1D	//加速度计配置寄存器2
#define MPU9250_REG_LP_ACCEL_ODR	        0X1E
#define MPU9250_REG_WOM_THR 	            0X1F
#define MPU9250_REG_FIFO_EN		            0X23	//FIFO使能寄存器
#define MPU9250_REG_I2CMST_CTRL	            0X24	//IIC主机控制寄存器
#define MPU9250_REG_I2CSLV0_ADDR            0X25	//IIC从机0器件地址寄存器
#define MPU9250_REG_I2CSLV0		            0X26	//IIC从机0数据地址寄存器
#define MPU9250_REG_I2CSLV0_CTRL            0X27	//IIC从机0控制寄存器
#define MPU9250_REG_I2CSLV1_ADDR            0X28	//IIC从机1器件地址寄存器
#define MPU9250_REG_I2CSLV1		            0X29	//IIC从机1数据地址寄存器
#define MPU9250_REG_I2CSLV1_CTRL            0X2A	//IIC从机1控制寄存器
#define MPU9250_REG_I2CSLV2_ADDR            0X2B	//IIC从机2器件地址寄存器
#define MPU9250_REG_I2CSLV2		            0X2C	//IIC从机2数据地址寄存器
#define MPU9250_REG_I2CSLV2_CTRL            0X2D	//IIC从机2控制寄存器
#define MPU9250_REG_I2CSLV3_ADDR            0X2E	//IIC从机3器件地址寄存器
#define MPU9250_REG_I2CSLV3		            0X2F	//IIC从机3数据地址寄存器
#define MPU9250_REG_I2CSLV3_CTRL            0X30	//IIC从机3控制寄存器
#define MPU9250_REG_I2CSLV4_ADDR            0X31	//IIC从机4器件地址寄存器
#define MPU9250_REG_I2CSLV4		            0X32	//IIC从机4数据地址寄存器
#define MPU9250_REG_I2CSLV4_DO	            0X33	//IIC从机4写数据寄存器
#define MPU9250_REG_I2CSLV4_CTRL            0X34	//IIC从机4控制寄存器
#define MPU9250_REG_I2CSLV4_DI	            0X35	//IIC从机4读数据寄存器
#define MPU9250_REG_I2CMST_STA	            0X36	//IIC主机状态寄存器
#define MPU9250_REG_INTPIN_CFG	            0X37
#define MPU9250_REG_INT_EN		            0X38	//中断使能寄存器
#define MPU9250_REG_INT_STA		            0X3A	//中断状态寄存器
#define MPU9250_REG_ACCEL_XOUTH	            0X3B	//加速度值,X轴高8位寄存器
#define MPU9250_REG_ACCEL_XOUTL	            0X3C	//加速度值,X轴低8位寄存器
#define MPU9250_REG_ACCEL_YOUTH	            0X3D	//加速度值,Y轴高8位寄存器
#define MPU9250_REG_ACCEL_YOUTL	            0X3E	//加速度值,Y轴低8位寄存器
#define MPU9250_REG_ACCEL_ZOUTH	            0X3F	//加速度值,Z轴高8位寄存器
#define MPU9250_REG_ACCEL_ZOUTL	            0X40	//加速度值,Z轴低8位寄存器
#define MPU9250_REG_TEMP_OUTH	            0X41	//温度值高八位寄存器
#define MPU9250_REG_TEMP_OUTL	            0X42	//温度值低8位寄存器
#define MPU9250_REG_GYRO_XOUTH	            0X43	//陀螺仪值,X轴高8位寄存器
#define MPU9250_REG_GYRO_XOUTL	            0X44	//陀螺仪值,X轴低8位寄存器
#define MPU9250_REG_GYRO_YOUTH	            0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU9250_REG_GYRO_YOUTL	            0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU9250_REG_GYRO_ZOUTH	            0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU9250_REG_GYRO_ZOUTL	            0X48	//陀螺仪值,Z轴低8位寄存器
#define MPU9250_REG_EXT_SENS_DATA00         0x49
#define MPU9250_REG_EXT_SENS_DATA01         0x4A
#define MPU9250_REG_EXT_SENS_DATA02         0x4B
#define MPU9250_REG_EXT_SENS_DATA03         0x4C
#define MPU9250_REG_EXT_SENS_DATA04         0x4D
#define MPU9250_REG_EXT_SENS_DATA05         0x4E
#define MPU9250_REG_EXT_SENS_DATA06         0x4F
#define MPU9250_REG_EXT_SENS_DATA07         0x50
#define MPU9250_REG_EXT_SENS_DATA08         0x51
#define MPU9250_REG_EXT_SENS_DATA09         0x52
#define MPU9250_REG_EXT_SENS_DATA10         0x53
#define MPU9250_REG_EXT_SENS_DATA11         0x54
#define MPU9250_REG_EXT_SENS_DATA12         0x55
#define MPU9250_REG_EXT_SENS_DATA13         0x56
#define MPU9250_REG_EXT_SENS_DATA14         0x57
#define MPU9250_REG_EXT_SENS_DATA15         0x58
#define MPU9250_REG_EXT_SENS_DATA16         0x59
#define MPU9250_REG_EXT_SENS_DATA17         0x5A
#define MPU9250_REG_EXT_SENS_DATA18         0x5B
#define MPU9250_REG_EXT_SENS_DATA19         0x5C
#define MPU9250_REG_EXT_SENS_DATA20         0x5D
#define MPU9250_REG_EXT_SENS_DATA21         0x5E
#define MPU9250_REG_EXT_SENS_DATA22         0x5F
#define MPU9250_REG_EXT_SENS_DATA23         0x60
#define MPU9250_REG_I2CSLV0_DO	            0X63	//IIC从机0数据寄存器
#define MPU9250_REG_I2CSLV1_DO	            0X64	//IIC从机1数据寄存器
#define MPU9250_REG_I2CSLV2_DO	            0X65	//IIC从机2数据寄存器
#define MPU9250_REG_I2CSLV3_DO	            0X66	//IIC从机3数据寄存器
#define MPU9250_REG_I2CMST_DELAY            0X67	//IIC主机延时管理寄存器
#define MPU9250_REG_SIGPATH_RST	            0X68	//信号通道复位寄存器
#define MPU9250_REG_MDETECT_CTRL            0X69	//运动检测控制寄存器
#define MPU9250_REG_USER_CTRL	            0X6A	//用户控制寄存器
#define MPU9250_REG_PWR_MGMT1	            0X6B	//电源管理寄存器1
#define MPU9250_REG_PWR_MGMT2	            0X6C	//电源管理寄存器2 
#define MPU9250_REG_FIFO_CNTH	            0X72	//FIFO计数寄存器高八位
#define MPU9250_REG_FIFO_CNTL	            0X73	//FIFO计数寄存器低八位
#define MPU9250_REG_FIFO_RW		            0X74	//FIFO读写寄存器
#define MPU9250_REG_DEVICE_ID	            0X75	//器件ID寄存器
#define MPU9250_REG_XA_OFFSETH              0X77
#define MPU9250_REG_XA_OFFSETL              0X78
#define MPU9250_REG_YA_OFFSETH              0X7A
#define MPU9250_REG_YA_OFFSETL              0X7B
#define MPU9250_REG_ZA_OFFSETH              0X7D
#define MPU9250_REG_ZA_OFFSETL              0X7E



//q30格式
#define Q30  1073741824.0f
//q16格式
#define Q16  65536.0f

/**
 * @brief 陀螺仪方向设置
 */
static int8_t MPU9250_GyroOrientation[9] = { 1, 0, 0,
                                             0, 1, 0,
                                             0, 0, 1};

static void MPU9250_InitExti(void (* irqHandler)(void));
void (* MPU9250_IrqHandler)(void);//外部中断回调函数
                                             
/**
 * @brief 设置陀螺仪量程
 * @param fsr MPU9250_FSR_XXXXDPS(见MPU9250_GyroFsrTypedef)
 * @return 0-成功; -1-失败
 */
int8_t MPU9250_SetGyroFsr(MPU9250_GyroFsrTypedef fsr)
{
    switch(fsr)
    {
        case MPU9250_FSR_250DPS:
            return (int8_t)mpu_set_gyro_fsr(250);
         case MPU9250_FSR_500DPS:
            return (int8_t)mpu_set_gyro_fsr(500);
        case MPU9250_FSR_1000DPS:
            return (int8_t)mpu_set_gyro_fsr(1000);
        case MPU9250_FSR_2000DPS:
            return (int8_t)mpu_set_gyro_fsr(2000);
        default:
            return -1;
    }
}
/**
 * @brief 设置加速度量程
 * @param fsr MPU9250_FSR_XXXXG(MPU9250_AccelFsrTypedef)
 * @return 0-成功; -1-失败
 */
int8_t MPU9250_SetAccelFsr(MPU9250_AccelFsrTypedef fsr)
{
    switch(fsr)
    {
        case MPU9250_FSR_2G:
            return (int8_t)mpu_set_accel_fsr(2);
         case MPU9250_FSR_4G:
            return (int8_t)mpu_set_accel_fsr(4);
        case MPU9250_FSR_8G:
            return (int8_t)mpu_set_accel_fsr(8);
        case MPU9250_FSR_16G:
            return (int8_t)mpu_set_accel_fsr(16);
        default:
            return -1;
    }
}
/**
 * @brief 设置采样率(假定Fs=1KHz)
 * @param rate 4~1000Hz
 * @return 0-成功; -1-失败
 */
inline int8_t MPU9250_SetSampleRate(uint16_t rate)
{
    return (int8_t)mpu_set_sample_rate(rate);
}
/**
 * @brief 数字低通滤波器
 * @param lpf MPU9250_FILTER_XXXHZ(见MPU9250_LpfTypedef)
 * @return 0-成功; -1-失败
 */
int8_t MPU9250_SetLPF(MPU9250_LpfTypedef lpf)
{
    switch(lpf)
    {
        case MPU9250_FILTER_188HZ:
            return (int8_t)mpu_set_lpf(188);
         case MPU9250_FILTER_98HZ:
            return (int8_t)mpu_set_lpf(98);
        case MPU9250_FILTER_42HZ:
            return (int8_t)mpu_set_lpf(42);
        case MPU9250_FILTER_20HZ:
            return (int8_t)mpu_set_lpf(20);
        case MPU9250_FILTER_10HZ:
            return (int8_t)mpu_set_lpf(10);
        case MPU9250_FILTER_5HZ:
            return (int8_t)mpu_set_lpf(5);
        default:
            return -1;
    }
}

/**
 * @brief 初始化外部中断
 * @param irqHandler 外部中断回调函数
 */
static inline void MPU9250_InitExti(void (* irqHandler)(void))
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    //GPIO
    RCC_AHB1PeriphClockCmd(MPU9250_INT_GPIO_CLK, ENABLE);//使能GPIO时钟
    GPIO_InitStructure.GPIO_Pin = MPU9250_INT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(MPU9250_INT_PORT, &GPIO_InitStructure);//初始化
    //SYSCFG
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(MPU9250_EXTI_PORT_SOURCE, MPU9250_EXTI_PIN_SOURCE);
    //EXTI
    EXTI_InitStructure.EXTI_Line = MPU9250_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    //NVIC
    NVIC_InitStructure.NVIC_IRQChannel = MPU9250_NVIC_IRQCHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
    //IRQ
    MPU9250_IrqHandler = irqHandler;
}

/**
 * @brief 初始化
 * @param irqHandler 外部中断回调函数
 * @return 0-成功; -1-失败
 * @note 陀螺仪±2000dps, 加速度传±2g, 采样率100Hz, 
 *       中断关, I2C主模式关, FIFO关, INT低有效, 陀螺仪X轴时钟
 */
inline int8_t MPU9250_Init(void (* irqHandler)(void))
{ 
    int8_t result;
    IIC_Init();//初始化IIC总线
    result = (int8_t)(mpu_init() | 
    mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS) | 
    mpu_set_sample_rate(MPU9250_SAMPLE_RATE));//设置采样率
    if(!result)
        return 0;
    else
        return -1;
}

/**
 * @brief 读取温度
 * @return 返回摄氏温度值
 */
float MPU9250_GetTemperature()
{
    long data = 0L;
    unsigned long timestamp;
    mpu_get_temperature(&data, &timestamp);
    return (float)data / Q16;
}
/**
 * @brief 读取陀螺仪
 * @param gx x轴原始读数(带符号)
 * @param gy y轴原始读数(带符号)
 * @param gz z轴原始读数(带符号)
 * @return 0-成功; -1-失败
 */
int8_t MPU9250_GetGyroscope(int16_t *gx, int16_t *gy, int16_t *gz)
{
    int16_t data[3];
    unsigned long timestamp;
    int8_t result = (int8_t)mpu_get_gyro_reg(data, &timestamp);
    *gx = data[0];
    *gy = data[1];
    *gz = data[2];
    return result;
}

/**
 * @brief 读取电子罗盘
 * @param mx x轴原始读数(带符号)
 * @param my y轴原始读数(带符号)
 * @param mz z轴原始读数(带符号)
 * @return 0-成功; -1-失败
 */
int8_t MPU9250_GetCompass(int16_t *mx, int16_t *my, int16_t *mz)
{
    int16_t data[3];
    unsigned long timestamp;
    int8_t result = (int8_t)mpu_get_compass_reg(data, &timestamp);
    *mx = data[0];
    *my = data[1];
    *mz = data[2];
    return result;
}
/**
 * @brief 读取加速度计
 * @param gx x轴原始读数(带符号)
 * @param gy y轴原始读数(带符号)
 * @param gz z轴原始读数(带符号)
 * @return 0-成功; -1-失败
 */
int8_t MPU9250_GetAccelerometer(int16_t *ax, int16_t *ay, int16_t *az)
{
    int16_t data[3];
    unsigned long timestamp;
    int8_t result = (int8_t)mpu_get_accel_reg(data, &timestamp);
    *ax = data[0];
    *ay = data[1];
    *az = data[2];
    return result;
}

int8_t MPU9250_GetEulerFromCompass(float *yaw)
{
    int16_t mx, my, mz;
    int8_t result = MPU9250_GetCompass(&mx, &my, &mz);
    *yaw = atan((float)my /(float)mx) * 57.29578f;
    return result;
}

/**
 * @brief MPU9250自测试
 * @return 0-成功; -1-失败
 */
static inline int8_t MPU9250_RunSelfTest()
{
	int32_t result;
	long gyro[3], accel[3]; 
	result = mpu_run_self_test(gyro, accel);
	if ((result & 0x07) == 0x07) 
	{
//		float sens;
//		uint16_t accel_sens;
//		mpu_get_gyro_sens(&sens);
//		gyro[0] = (long)(gyro[0] * sens);
//		gyro[1] = (long)(gyro[1] * sens);
//		gyro[2] = (long)(gyro[2] * sens);
//		dmp_set_gyro_bias(gyro);
//		mpu_get_accel_sens(&accel_sens);
//		accel[0] *= accel_sens;
//		accel[1] *= accel_sens;
//		accel[2] *= accel_sens;
//		dmp_set_accel_bias(accel);
		return 0;
	}
    return -1;
}

/**
 * @brief 方向转换
 */
static inline int16_t MPU9250_Row2Scale(const int8_t *row)
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
 * @return 0-成功; 其他-失败
 */
static inline uint16_t MPU9250_OrientationMatrix2Scalar(const int8_t *matrix)
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
    scalar = MPU9250_Row2Scale(matrix);
    scalar |= MPU9250_Row2Scale(matrix + 3) << 3;
    scalar |= MPU9250_Row2Scale(matrix + 6) << 6;
    return scalar;
}

/**
 * @brief 连着dmp一起初始化
 * @param irqHandler 外部中断回调函数
 * @return 0-成功; 其他-失败
 */
int8_t MPU9250_InitWithDmp(void (* irqHandler)(void))
{
	IIC_Init();//初始化IIC总线
    
    MPU9250_InitExti(irqHandler);
	if(!mpu_init())//初始化MPU9250
	{
        //设置所需要的传感器
		if(mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS))
            return 1;
        //设置FIFO
		if(mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
            return 2;
        //加载dmp固件
		if(dmp_load_motion_driver_firmware())
            return 4;
		//设置陀螺仪方向
		if(dmp_set_orientation(MPU9250_OrientationMatrix2Scalar(MPU9250_GyroOrientation)))
            return 5;
		//设置dmp功能
		if(dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP | DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL))
            return 6;
		//设置DMP输出速率(最大不超过200Hz)
		if(dmp_set_fifo_rate(MPU9250_FIFO_RATE))
            return 7;
		//自检
		if(MPU9250_RunSelfTest())
            return 8;
		//使能DMP
		if(mpu_set_dmp_state(1))
            return 9;
	}
	return 0;
}

/**
 * @brief 得到dmp处理后的数据
 * @param pitch 俯仰角, 精度:0.1°, 范围 -90°~90°
 * @param pitch 横滚角, 精度:0.1°, 范围 -180°~180°
 * @param pitch 航向角, 精度:0.1°, 范围 -180°~180°
 * @return 0-成功; 其他-失败
 */
int8_t MPU9250_GetDmpData(float *pitch, float *roll, float *yaw)
{
	float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
	unsigned long sensor_timestamp;
	int16_t gyro[3], accel[3], sensors;
	uint8_t more;
	long quat[4]; 
	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more))
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
	if(!(sensors & INV_WXYZ_QUAT))
        return 2;
    q0 = quat[0] / Q30;	//q30格式转换为浮点数
    q1 = quat[1] / Q30;
    q2 = quat[2] / Q30;
    q3 = quat[3] / Q30; 
    //计算得到俯仰角/横滚角/航向角
    *pitch = asin(2 * q0 * q2 - 2 * q1 * q3) * 57.3;// pitch
    *roll = atan2(2 * q2 * q3 + 2 * q0 * q1, 1 - 2 * q1 * q1 - 2 * q2* q2) * 57.3;// roll
    *yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3;//yaw
	return 0;
}

/**
 * @brief MPU9250的外部中断服务函数
 */
void MPU9250_EXTI_IRQHANDLER()
{
    if(EXTI_GetITStatus(MPU9250_EXTI_LINE) != RESET)
    {
        MPU9250_IrqHandler();
        EXTI_ClearITPendingBit(MPU9250_EXTI_LINE);
    }
}
