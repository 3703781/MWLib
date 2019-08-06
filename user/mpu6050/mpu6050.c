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

#include "mpu6050.h"
#include "arm_math.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
 /**
  * @brief �Ĵ�������
  */
#define MPU6050_REG_ACCEL_OFFS		    0X06 //!< accel_offs�Ĵ���,�ɶ�ȡ�汾��,�Ĵ����ֲ�δ�ᵽ
#define MPU6050_REG_PROD_ID			      0X0C //!< prod id�Ĵ���,�ڼĴ����ֲ�δ�ᵽ
#define MPU6050_REG_SELF_TESTX		    0X0D //!< �Լ�Ĵ���X
#define MPU6050_REG_SELF_TESTY		    0X0E //!< �Լ�Ĵ���Y
#define MPU6050_REG_SELF_TESTZ		    0X0F //!< �Լ�Ĵ���Z
#define MPU6050_REG_SELF_TESTA		    0X10 //!< �Լ�Ĵ���A
#define MPU6050_REG_SAMPLE_RATE		    0X19 //!< ����Ƶ�ʷ�Ƶ��
#define MPU6050_REG_CFG				        0X1A //!< ���üĴ���
#define MPU6050_REG_GYRO_CFG		      0X1B //!< ���������üĴ���
#define MPU6050_REG_ACCEL_CFG		      0X1C //!< ���ٶȼ����üĴ���
#define MPU6050_REG_MOTION_DET		    0X1F //!< �˶���ֵⷧ���üĴ���
#define MPU6050_REG_FIFO_EN			      0X23 //!< FIFOʹ�ܼĴ���
#define MPU6050_REG_I2CMST_CTRL		    0X24 //!< IIC�������ƼĴ���
#define MPU6050_REG_I2CSLV0_ADDR	    0X25 //!< IIC�ӻ�0������ַ�Ĵ���
#define MPU6050_REG_I2CSLV0			      0X26 //!< IIC�ӻ�0���ݵ�ַ�Ĵ���
#define MPU6050_REG_I2CSLV0_CTRL	    0X27 //!< IIC�ӻ�0���ƼĴ���
#define MPU6050_REG_I2CSLV1_ADDR	    0X28 //!< IIC�ӻ�1������ַ�Ĵ���
#define MPU6050_REG_I2CSLV1			      0X29 //!< IIC�ӻ�1���ݵ�ַ�Ĵ���
#define MPU6050_REG_I2CSLV1_CTRL	    0X2A //!< IIC�ӻ�1���ƼĴ���
#define MPU6050_REG_I2CSLV2_ADDR	    0X2B //!< IIC�ӻ�2������ַ�Ĵ���
#define MPU6050_REG_I2CSLV2			      0X2C //!< IIC�ӻ�2���ݵ�ַ�Ĵ���
#define MPU6050_REG_I2CSLV2_CTRL	    0X2D //!< IIC�ӻ�2���ƼĴ���
#define MPU6050_REG_I2CSLV3_ADDR	    0X2E //!< IIC�ӻ�3������ַ�Ĵ���
#define MPU6050_REG_I2CSLV3			      0X2F //!< IIC�ӻ�3���ݵ�ַ�Ĵ���
#define MPU6050_REG_I2CSLV3_CTRL	    0X30 //!< IIC�ӻ�3���ƼĴ���
#define MPU6050_REG_I2CSLV4_ADDR	    0X31 //!< IIC�ӻ�4������ַ�Ĵ���
#define MPU6050_REG_I2CSLV4			      0X32 //!< IIC�ӻ�4���ݵ�ַ�Ĵ���
#define MPU6050_REG_I2CSLV4_DO		    0X33 //!< IIC�ӻ�4д���ݼĴ���
#define MPU6050_REG_I2CSLV4_CTRL	    0X34 //!< IIC�ӻ�4���ƼĴ���
#define MPU6050_REG_I2CSLV4_DI		    0X35 //!< IIC�ӻ�4�����ݼĴ���
#define MPU6050_REG_I2CMST_STA		    0X36 //!< IIC����״̬�Ĵ���
#define MPU6050_REG_INTBP_CFG		      0X37 //!< �ж�/��·���üĴ���
#define MPU6050_REG_INT_EN			      0X38 //!< �ж�ʹ�ܼĴ���
#define MPU6050_REG_INT_STA			      0X3A //!< �ж�״̬�Ĵ���
#define MPU6050_REG_ACCEL_XOUTH		    0X3B //!< ���ٶ�ֵ,X���8λ�Ĵ���
#define MPU6050_REG_ACCEL_XOUTL		    0X3C //!< ���ٶ�ֵ,X���8λ�Ĵ���
#define MPU6050_REG_ACCEL_YOUTH		    0X3D //!< ���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU6050_REG_ACCEL_YOUTL		    0X3E //!< ���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU6050_REG_ACCEL_ZOUTH		    0X3F //!< ���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU6050_REG_ACCEL_ZOUTL		    0X40 //!< ���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU6050_REG_TEMP_OUTH		      0X41 //!< �¶�ֵ�߰�λ�Ĵ���
#define MPU6050_REG_TEMP_OUTL		      0X42 //!< �¶�ֵ��8λ�Ĵ���
#define MPU6050_REG_GYRO_XOUTH		    0X43 //!< ������ֵ,X���8λ�Ĵ���
#define MPU6050_REG_GYRO_XOUTL		    0X44 //!< ������ֵ,X���8λ�Ĵ���
#define MPU6050_REG_GYRO_YOUTH		    0X45 //!< ������ֵ,Y���8λ�Ĵ���
#define MPU6050_REG_GYRO_YOUTL		    0X46 //!< ������ֵ,Y���8λ�Ĵ���
#define MPU6050_REG_GYRO_ZOUTH		    0X47 //!< ������ֵ,Z���8λ�Ĵ���
#define MPU6050_REG_GYRO_ZOUTL		    0X48 //!< ������ֵ,Z���8λ�Ĵ���
#define MPU6050_REG_I2CSLV0_DO		    0X63 //!< IIC�ӻ�0���ݼĴ���
#define MPU6050_REG_I2CSLV1_DO		    0X64 //!< IIC�ӻ�1���ݼĴ���
#define MPU6050_REG_I2CSLV2_DO		    0X65 //!< IIC�ӻ�2���ݼĴ���
#define MPU6050_REG_I2CSLV3_DO		    0X66 //!< IIC�ӻ�3���ݼĴ���
#define MPU6050_REG_I2CMST_DELAY	    0X67 //!< IIC������ʱ����Ĵ���
#define MPU6050_REG_SIGPATH_RST		    0X68 //!< �ź�ͨ����λ�Ĵ���
#define MPU6050_REG_MDETECT_CTRL	    0X69 //!< �˶������ƼĴ���
#define MPU6050_REG_USER_CTRL		      0X6A //!< �û����ƼĴ���
#define MPU6050_REG_PWR_MGMT1		      0X6B //!< ��Դ����Ĵ���1
#define MPU6050_REG_PWR_MGMT2		      0X6C //!< ��Դ����Ĵ���2 
#define MPU6050_REG_FIFO_CNTH		      0X72 //!< FIFO�����Ĵ����߰�λ
#define MPU6050_REG_FIFO_CNTL		      0X73 //!< FIFO�����Ĵ����Ͱ�λ
#define MPU6050_REG_FIFO_RW			      0X74 //!< FIFO��д�Ĵ���
#define MPU6050_REG_DEVICE_ID		      0X75 //!< ����ID�Ĵ���

#define MPU6050_Q30                   1073741824.0 //!<q30��ʽ
#define MPU6050_Q16                   65536.0 //!< q16��ʽ

  /**
   * @brief �����Ƿ�������
   */
static int8_t MPU6050_GyroOrientation[9] = { 1, 0, 0,
                                             0, 1, 0,
                                             0, 0, 1 };
static MPU6050_DataArrivalHandler __dataArrivalHandler = NULL; //!< �ⲿ�жϻص�����

/**
 * @brief ��������������
 * @param fsr MPU6050_FSR_XXXXDPS(��MPU6050_GyroFsrTypedef)
 * @return 0-�ɹ�; 1-ʧ��
 */
uint8_t MPU6050_SetGyroFsr(MPU6050_GyroFsrTypedef fsr)
{
  return IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_GYRO_CFG, fsr << 3);//���������������̷�Χ  
}

/**
 * @brief ���ü��ٶ�����
 * @param fsr MPU6050_FSR_XXXXG(MPU6050_AccelFsrTypedef)
 * @return 0-�ɹ�; 1-ʧ��
 */
uint8_t MPU6050_SetAccelFsr(MPU6050_AccelFsrTypedef fsr)
{
  return IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_ACCEL_CFG, fsr << 3);//���ü��ٶȴ����������̷�Χ  
}

/**
 * @brief ���ò�����(�ٶ�Fs=1KHz)
 * @param rate 4~1000Hz
 * @return 0-�ɹ�; 1-ʧ��
 */
uint8_t MPU6050_SetSampleRate(uint16_t rate)
{
  uint16_t lpf = rate >> 1;
  MPU6050_LpfTypedef data;
  if (rate > 1000 || rate < 4)
    return 1;
  if (IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_SAMPLE_RATE, 1000 / rate - 1))    //�������ֵ�ͨ�˲���
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
  return MPU6050_SetLPF(data);//����LPFΪ�����ʵ�һ��
}

/**
 * @brief ���ֵ�ͨ�˲���
 * @param lpf MPU6050_FILTER_XXXHZ(��MPU6050_LpfTypedef)
 * @return 0-�ɹ�; 1-ʧ��
 */
uint8_t MPU6050_SetLPF(MPU6050_LpfTypedef lpf)
{
  return IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_CFG, lpf);//�������ֵ�ͨ�˲���  
}

/**
 * @brief ��ʼ��
 * @return 0-�ɹ�; 1-ʧ��
 * @note �����ǡ�2000dps, ���ٶȴ���2g, ������50Hz,
 *       �жϹ�, I2C��ģʽ��, FIFO��, INT����Ч, ������X��ʱ��
 */
uint8_t MPU6050_Init()
{
  IIC_Init();//��ʼ��IIC����
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_PWR_MGMT1, 0X80);//��λMPU6050
  UTILS_DelayMs(100);
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_PWR_MGMT1, 0X00);//����MPU6050 
  MPU6050_SetGyroFsr(MPU6050_FSR_2000DPS);//�����ǡ�2000dps
  MPU6050_SetAccelFsr(MPU6050_FSR_2G);//���ٶȴ���2g
  MPU6050_SetSampleRate(MPU6050_SAMPLE_RATE);//���ò�����
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_INT_EN, 0X00);//�ر������ж�
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_USER_CTRL, 0X00);//I2C��ģʽ�ر�
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_FIFO_EN, 0X00);//�ر�FIFO
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_INTBP_CFG, 0X80);//INT���ŵ͵�ƽ��Ч
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_PWR_MGMT1, 0X01);//����CLKSEL X��PLLΪ�ο�
  IIC_WriteRegByte(MPU6050_ADDR, MPU6050_REG_PWR_MGMT2, 0X00);//���ٶ��������Ƕ�����

  return IIC_ReadRegByte(MPU6050_ADDR, MPU6050_REG_DEVICE_ID) != MPU6050_ADDR;
}

/**
 * @brief ��ȡ�¶�
 * @return ���������¶�ֵ
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
 * @brief ��ȡ������
 * @param gx x��ԭʼ����(������)
 * @param gy y��ԭʼ����(������)
 * @param gz z��ԭʼ����(������)
 * @return 0-�ɹ�; 1-ʧ��
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
 * @brief ��ʼ���ⲿ�ж�
 * @param irqHandler �ⲿ�жϻص�����
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
  GPIO_Init(MPU6050_INT_PORT, &GPIO_InitStructure);//��ʼ��
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
 * @brief �����ⲿ�ж�, ��ʼ����MPU6050������
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
 * @brief ��ȡ���ٶȼ�
 * @param gx x��ԭʼ����(������)
 * @param gy y��ԭʼ����(������)
 * @param gz z��ԭʼ����(������)
 * @return 0-�ɹ�; 1-ʧ��
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
 * @brief MPU6050�Բ���
 * @return 0-�ɹ�; 1-ʧ��
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
 * @brief ����ת��
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
 * @brief �����Ƿ������
 * @param matrix �������
 * @return 0-�ɹ�; 1-ʧ��
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
 * @brief ����dmpһ���ʼ��
 * @return 0-�ɹ�; ����-ʧ��
 */
uint8_t MPU6050_InitWithDmp(MPU6050_DataArrivalHandler dataArrivalHandler)
{
  IIC_Init();//��ʼ��IIC����
  if (!mpu_init())//��ʼ��MPU6050
  {
    //��������Ҫ�Ĵ�����
    if (mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
      return 1;
    //����FIFO
    if (mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
      return 2;
    //����dmp�̼�
    if (dmp_load_motion_driver_firmware())
      return 4;
    //���������Ƿ���
    if (dmp_set_orientation(MPU6050_InvOrientationMatrix2Scalar(MPU6050_GyroOrientation)))
      return 5;
    //����dmp����
    if (dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
      DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
      DMP_FEATURE_GYRO_CAL))
      return 6;
    //����DMP�������(��󲻳���200Hz)
    if (dmp_set_fifo_rate(MPU6050_FIFO_RATE))
      return 7;
    //�Լ�
    if (MPU6050_RunSelfTest())
      return 8;
    //ʹ��DMP
    if (mpu_set_dmp_state(1))
      return 9;
  }
  MPU6050_InitExti(dataArrivalHandler);
  return 0;
}

/**
 * @brief �õ�dmp����������
 * @param pitch ������, ����:0.1��, ��Χ -90��~90��
 * @param pitch �����, ����:0.1��, ��Χ -180��~180��
 * @param pitch �����, ����:0.1��, ��Χ -180��~180��
 * @return 0-�ɹ�; ����-ʧ��
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
  q0 = quat[0] / MPU6050_Q30;	//q30��ʽת��Ϊ������
  q1 = quat[1] / MPU6050_Q30;
  q2 = quat[2] / MPU6050_Q30;
  q3 = quat[3] / MPU6050_Q30;

  *pitch = asinf(2.0f * q0 * q2 - 2.0f * q1 * q3) * 57.29577951308232286464772187173366546630859375f;
  *roll = atan2f(2.0f * q2 * q3 + 2.0f * q0 * q1, 1.0f - 2.0f * q1 * q1 - 2.0f * q2 * q2) * 57.29577951308232286464772187173366546630859375f;
  *yaw = atan2f(2.0f * q1 * q2 + 2.0f * q0 * q3, q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.29577951308232286464772187173366546630859375f;
  return 0;
}

/**
 * @brief MPU6050���ⲿ�жϷ�����
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
