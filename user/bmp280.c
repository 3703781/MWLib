/**
 * @file    bmp280.c
 * @author  Miaow
 * @version 0.1.0
 * @date    2019/07/07
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of BMP280:
 *              1. Communicate using I2C
 *              2. Initialization and Configuration
 *              3. Measure and read out temperature and pressure
 * @note     
 *          Minimum version of header file:
 *              0.1.0
 *          
 *          Pin connection(defined in iic.h):
 *          ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 *          ©¦     PB8©À©¤©¤©¤©¤©¤©ÈSCK  CSB©À©¤©¤©¤3V3
 *          ©¦     PB9©À©¤©¤©¤©¤©¤©ÈSDI  SDO©À©¤©¤©¤GND
 *          ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *          STM32F407       bmp280
 *
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 *          Inspired by BoschSensortec's API: https://github.com/BoschSensortec/BMP280_driver
 */
#include "bmp280.h"
#include "bsp_iic.h"

//Chip IDs for samples and mass production parts.
#define BMP280_CHIP_ID1                 0x56
#define BMP280_CHIP_ID2                 0x57
#define BMP280_CHIP_ID3                 0x58

//Calibration parameter register addresses.
#define BMP280_DIG_T1_LSB_ADDR          0x88
#define BMP280_DIG_T1_MSB_ADDR          0x89
#define BMP280_DIG_T2_LSB_ADDR          0x8A
#define BMP280_DIG_T2_MSB_ADDR          0x8B
#define BMP280_DIG_T3_LSB_ADDR          0x8C
#define BMP280_DIG_T3_MSB_ADDR          0x8D
#define BMP280_DIG_P1_LSB_ADDR          0x8E
#define BMP280_DIG_P1_MSB_ADDR          0x8F
#define BMP280_DIG_P2_LSB_ADDR          0x90
#define BMP280_DIG_P2_MSB_ADDR          0x91
#define BMP280_DIG_P3_LSB_ADDR          0x92
#define BMP280_DIG_P3_MSB_ADDR          0x93
#define BMP280_DIG_P4_LSB_ADDR          0x94
#define BMP280_DIG_P4_MSB_ADDR          0x95
#define BMP280_DIG_P5_LSB_ADDR          0x96
#define BMP280_DIG_P5_MSB_ADDR          0x97
#define BMP280_DIG_P6_LSB_ADDR          0x98
#define BMP280_DIG_P6_MSB_ADDR          0x99
#define BMP280_DIG_P7_LSB_ADDR          0x9A
#define BMP280_DIG_P7_MSB_ADDR          0x9B
#define BMP280_DIG_P8_LSB_ADDR          0x9C
#define BMP280_DIG_P8_MSB_ADDR          0x9D
#define BMP280_DIG_P9_LSB_ADDR          0x9E
#define BMP280_DIG_P9_MSB_ADDR          0x9F

//Registers
#define BMP280_CHIP_ID_ADDR             0xD0
#define BMP280_SOFT_RESET_ADDR          0xE0
#define BMP280_STATUS_ADDR              0xF3
#define BMP280_CTRL_MEAS_ADDR           0xF4
#define BMP280_CONFIG_ADDR              0xF5
#define BMP280_PRES_MSB_ADDR            0xF7
#define BMP280_PRES_LSB_ADDR            0xF8
#define BMP280_PRES_XLSB_ADDR           0xF9
#define BMP280_TEMP_MSB_ADDR            0xFA
#define BMP280_TEMP_LSB_ADDR            0xFB
#define BMP280_TEMP_XLSB_ADDR           0xFC

/**
 * @brief ODR options.
 * Normal mode continuosly cycles between an measurement period and an standby period.
 * Standby time defines defined time of each standby period.
 */
typedef enum
{
  BMP280_ODR_0_5_MS = 0, //!< 0.5ms
  BMP280_ODR_62_5_MS = 1, //!< 62.5ms
  BMP280_ODR_125_MS = 2, //!< 125ms
  BMP280_ODR_250_MS = 3, //!< 250ms
  BMP280_ODR_500_MS = 4, //!< 500ms
  BMP280_ODR_1000_MS = 5, //!< 1000ms
  BMP280_ODR_2000_MS = 6, //!< 2000ms
  BMP280_ODR_4000_MS = 7 //!< 4000ms
}BMP280_StandbyTime;

/**
 * @brief Oversampling options.
 * Pressure/temperature measurement can be enabled or skipped. Skipping the measurement could be useful
 * if BMP280 is used as temperature sensor. BMP280_OS_NONE is the option for skipping 
 * ressure/temperature measurement.
 */
typedef enum
{
  BMP280_OS_NONE = 0, //!< Skip the measurement.
  BMP280_OS_1X = 1, //!< Ultra low power.
  BMP280_OS_2X = 2, //!< Low power.
  BMP280_OS_4X = 3, //!< Standard resolution.
  BMP280_OS_8X = 4, //!< High resolution.
  BMP280_OS_16X = 5 //!< Ultra high resolution.
}BMP280_OverSampling;

/**
 * @breif IIR Filter options.
 * The filter suppresses these disturbances in the output data 
 * and reduces the bandwidth of the output signals.
 */
typedef enum
{
  BMP280_FILTER_OFF = 0,
  BMP280_FILTER_COEFF_2 = 1,
  BMP280_FILTER_COEFF_4 = 2,
  BMP280_FILTER_COEFF_8 = 3,
  BMP280_FILTER_COEFF_16 = 4
}BMP280_IirFilter;

/**
 * @brief Power mode options.
 */
typedef enum
{
  BMP280_SLEEP_MODE = 0, //!< No measurements are performed in sleep mode.
  BMP280_FORCED_MODE = 1, //!< A single measurement is performed in forced mode.
  BMP280_NORMAL_MODE = 3 //!< Normal mode comprises an automated perpetual cycling between an active measurement period and an inactive standby period.
}BMP280_Mode;

#if defined(BMP280_APPLICAITION_HANDHELD_DEVICE_LOW_POWER)

#define BMP280_MODE                       BMP280_NORMAL_MODE
#define BMP280_PRESSURE_OVER_SAMPLING     BMP280_OS_16X
#define BMP280_TEMPERATURE_OVER_SAMPLING  BMP280_OS_2X
#define BMP280IIR_FILTER                  BMP280_FILTER_COEFF_4
#define BMP280_STANDBY_TIME               BMP280_ODR_62_5_MS

#elif defined(BMP280_APPLICAITION_HANDHELD_DEVICE_DYNAMIC)

#define BMP280_MODE                       BMP280_NORMAL_MODE
#define BMP280_PRESSURE_OVER_SAMPLING     BMP280_OS_4X
#define BMP280_TEMPERATURE_OVER_SAMPLING  BMP280_OS_1X
#define BMP280IIR_FILTER                  BMP280_FILTER_COEFF_16
#define BMP280_STANDBY_TIME               BMP280_ODR_0_5_MS

#elif defined(BMP280_APPLICATION_WEATHER_MONITORING)

#define BMP280_MODE                       BMP280_FORCED_MODE
#define BMP280_PRESSURE_OVER_SAMPLING     BMP280_OS_1X
#define BMP280_TEMPERATURE_OVER_SAMPLING  BMP280_OS_1X
#define BMP280IIR_FILTER                  BMP280_FILTER_OFF
#define BMP280_STANDBY_TIME               BMP280_ODR_4000_MS

#elif defined(BMP280_APPLICATION_FLOOR_CHANGE_DETECTION)

#define BMP280_MODE                       BMP280_NORMAL_MODE
#define BMP280_PRESSURE_OVER_SAMPLING     BMP280_OS_4X
#define BMP280_TEMPERATURE_OVER_SAMPLING  BMP280_OS_1X
#define BMP280IIR_FILTER                  BMP280_FILTER_COEFF_4
#define BMP280_STANDBY_TIME               BMP280_ODR_125_MS

#elif defined(BMP280_APPLICATION_DROP_DETECTION)

#define BMP280_MODE                       BMP280_NORMAL_MODE
#define BMP280_PRESSURE_OVER_SAMPLING     BMP280_OS_2X
#define BMP280_TEMPERATURE_OVER_SAMPLING  BMP280_OS_1X
#define BMP280IIR_FILTER                  BMP280_FILTER_OFF
#define BMP280_STANDBY_TIME               BMP280_ODR_0_5_MS

#elif defined(BMP280_APPLICATION_INDOOR_NAVIGATION)

#define BMP280_MODE                       BMP280_NORMAL_MODE
#define BMP280_PRESSURE_OVER_SAMPLING     BMP280_OS_16X
#define BMP280_TEMPERATURE_OVER_SAMPLING  BMP280_OS_2X
#define BMP280IIR_FILTER                  BMP280_FILTER_COEFF_16
#define BMP280_STANDBY_TIME               BMP280_ODR_0_5_MS

#endif

/**
 * @brief Calibration parameters' structure
 */
struct
{
  uint16_t dig_t1;
  int16_t dig_t2;
  int16_t dig_t3;
  uint16_t dig_p1;
  int16_t dig_p2;
  int16_t dig_p3;
  int16_t dig_p4;
  int16_t dig_p5;
  int16_t dig_p6;
  int16_t dig_p7;
  int16_t dig_p8;
  int16_t dig_p9;
  int32_t t_fine;
}BMP280_CalibrationParameters;

/**
 * @brief Perform a software reset.
 * @return Result of execution, see @ref DRV8825_return_status .
 */
static inline uint8_t BMP280_SoftReset()
{
  __IO uint32_t i;
  uint8_t result;
  result = IIC_WriteRegByte(BMP280_DEVICE_ADDR, BMP280_SOFT_RESET_ADDR, 0xB6);
  
  //2ms
  i = 50000;
  while (i--);
  return result;
}

/**
 * @brief This function is used to read the calibration parameters used
 * for calculating the compensated data.
 * @return Result of execution, see @ref DRV8825_return_status .
 */
static uint8_t BM280_GetCalibrationParameters()
{
  uint8_t result;
  uint8_t temp[24] = { 0 };

  result = IIC_ReadRegBytes(BMP280_DEVICE_ADDR, BMP280_DIG_T1_LSB_ADDR, 24, temp);
  if (result == BMP280_OK)
  {
    BMP280_CalibrationParameters.dig_t1 = (uint16_t)(((uint16_t)temp[1] << 8) | ((uint16_t)temp[0]));
    BMP280_CalibrationParameters.dig_t2 = (int16_t)(((int16_t)temp[3] << 8) | ((int16_t)temp[2]));
    BMP280_CalibrationParameters.dig_t3 = (int16_t)(((int16_t)temp[5] << 8) | ((int16_t)temp[4]));
    BMP280_CalibrationParameters.dig_p1 = (uint16_t)(((uint16_t)temp[7] << 8) | ((uint16_t)temp[6]));
    BMP280_CalibrationParameters.dig_p2 = (int16_t)(((int16_t)temp[9] << 8) | ((int16_t)temp[8]));
    BMP280_CalibrationParameters.dig_p3 = (int16_t)(((int16_t)temp[11] << 8) | ((int16_t)temp[10]));
    BMP280_CalibrationParameters.dig_p4 = (int16_t)(((int16_t)temp[13] << 8) | ((int16_t)temp[12]));
    BMP280_CalibrationParameters.dig_p5 = (int16_t)(((int16_t)temp[15] << 8) | ((int16_t)temp[14]));
    BMP280_CalibrationParameters.dig_p6 = (int16_t)(((int16_t)temp[17] << 8) | ((int16_t)temp[16]));
    BMP280_CalibrationParameters.dig_p7 = (int16_t)(((int16_t)temp[19] << 8) | ((int16_t)temp[18]));
    BMP280_CalibrationParameters.dig_p8 = (int16_t)(((int16_t)temp[21] << 8) | ((int16_t)temp[20]));
    BMP280_CalibrationParameters.dig_p9 = (int16_t)(((int16_t)temp[23] << 8) | ((int16_t)temp[22]));
  }
  return result;
}

/**
 * @brief This function configures the BMP280.
 * @param temperature Temperature over sampling.
 * @param pressure Pressure over sampling.
 * @param mode Power mode.
 * @param coefficient The coefficient of the pressure IIR filter.
 * @time time Standby time.
 * @return Result of execution, see @ref DRV8825_return_status .
 */
static inline uint8_t BMP280_SetSettings(
  BMP280_OverSampling temperature,
  BMP280_OverSampling pressure,
  BMP280_Mode mode,
  BMP280_IirFilter coefficient,
  BMP280_StandbyTime time)
{
  uint8_t result1;
  uint8_t result2;
  uint8_t data1 = ((uint8_t)temperature << 5) | ((uint8_t)pressure << 2) | (uint8_t)mode;
  uint8_t data2 = ((uint8_t)time << 5) | ((uint8_t)coefficient << 2);

  result1 = IIC_WriteRegByte(BMP280_DEVICE_ADDR, BMP280_CTRL_MEAS_ADDR, data1);
  result2 = IIC_WriteRegByte(BMP280_DEVICE_ADDR, BMP280_CONFIG_ADDR, data2);
  if (result1 == BMP280_OK && result2 == BMP280_OK)
    return BMP280_OK;
  return BMP280_ERROR;
}

/**
 * @brief This function is used to get the compensated temperature from
 * uncompensated temperature.
 * @param uncompensatedTemperature Raw temperature values from the sensor.
 * @return Compensated temperature.
 */
static float BMP280_GetCompensatedTemperature(int32_t uncompensatedTemperature)
{
  float var1, var2;

  var1 = (((float)uncompensatedTemperature) / 16384.0f - ((float)BMP280_CalibrationParameters.dig_t1) / 1024.0f) *
    ((float)BMP280_CalibrationParameters.dig_t2);
  var2 =
    ((((float)uncompensatedTemperature) / 131072.0f - ((float)BMP280_CalibrationParameters.dig_t1) / 8192.0f) *
    (((float)uncompensatedTemperature) / 131072.0f - ((float)BMP280_CalibrationParameters.dig_t1) / 8192.0f)) *
      ((float)BMP280_CalibrationParameters.dig_t3);
  return ((var1 + var2) / 5120.0f);
}

/**
 * @brief This function is used to get the compensated pressure from
 * uncompensated pressure. This API uses floating precision.
 * @param uncompensatedPressure Raw pressure values from the sensor.
 * @return Compensated pressure.
 */
static float BMP280_GetCompensatedPressure(int32_t uncompensatedPressure)
{
  float var1, var2;

  var1 = ((float)BMP280_CalibrationParameters.t_fine / 2.0f) - 64000.0f;
  var2 = var1 * var1 * ((float)BMP280_CalibrationParameters.dig_p6) / 32768.0f;
  var2 = var2 + var1 * ((float)BMP280_CalibrationParameters.dig_p5) * 2.0f;
  var2 = (var2 / 4.0f) + (((float)BMP280_CalibrationParameters.dig_p4) * 65536.0f);
  var1 =
    (((float)BMP280_CalibrationParameters.dig_p3) * var1 * var1 / 524288.0f + ((float)BMP280_CalibrationParameters.dig_p2) * var1) /
    524288.0f;
  var1 = (1.0f + var1 / 32768.0f) * ((float)BMP280_CalibrationParameters.dig_p1);
  uncompensatedPressure = (uint32_t)(1048576.0f - (float)uncompensatedPressure);
  if (var1 < 0 || var1 > 0)
  {
    uncompensatedPressure = (uint32_t)((uncompensatedPressure - (var2 / 4096.0f)) * 6250.0f / var1);
    var1 = ((float)BMP280_CalibrationParameters.dig_p9) * uncompensatedPressure * uncompensatedPressure / 2147483648.0f;
    var2 = uncompensatedPressure * ((float)BMP280_CalibrationParameters.dig_p8) / 32768.0f;
    return uncompensatedPressure + (var1 + var2 + ((float)BMP280_CalibrationParameters.dig_p7)) / 16.0f;
  }
  return 0.0f;
}

/**
 * @brief Initialize the BMP280 with the settings in the header file.
 * @return Result of execution, see @ref DRV8825_return_status .
 */
uint8_t BMP280_Init()
{
  uint8_t chipId;
  uint8_t tryCount = 5;
  uint8_t result;
  __IO uint32_t i;
  IIC_Init();
  while (tryCount--)
  {
    chipId = IIC_ReadRegByte(BMP280_DEVICE_ADDR, BMP280_CHIP_ID_ADDR);
    if (chipId == BMP280_CHIP_ID1 || chipId == BMP280_CHIP_ID2 || chipId == BMP280_CHIP_ID3)
    {
      result = BMP280_SoftReset();
      if (result == BMP280_OK)
        result = BM280_GetCalibrationParameters();
      if (result == BMP280_OK)
        result = BMP280_SetSettings(BMP280_TEMPERATURE_OVER_SAMPLING,
          BMP280_PRESSURE_OVER_SAMPLING,
          BMP280_MODE,
          BMP280IIR_FILTER,
          BMP280_STANDBY_TIME);
      if (result == BMP280_OK)
        break;
    }

    //10ms
    i = 250000;
    while (i--);
  }

  return BMP280_ERROR;
}


/**
 * @brief Start a measurement and wait for completion.
 * @return Result of execution, see @ref DRV8825_return_status .
 * @note This function can only be called in forced mode.
 * @warning The program may be blocked if the BMP280 never 
 * set the flag of completion or the communication failed.
 */
uint8_t BMP280_PerformMeasurement()
{
  uint8_t result;
  uint8_t data = ((uint8_t)BMP280_TEMPERATURE_OVER_SAMPLING << 5) | 
  ((uint8_t)BMP280_PRESSURE_OVER_SAMPLING << 2) | 
  (uint8_t)BMP280_MODE;
  
  result = IIC_WriteRegByte(BMP280_DEVICE_ADDR, BMP280_CTRL_MEAS_ADDR, data);

  if (result == BMP280_OK)
  {
    while (IIC_ReadRegByte(BMP280_DEVICE_ADDR, BMP280_STATUS_ADDR) & 0x08);
    return BMP280_OK;
  }
  return BMP280_ERROR;
}

/**
 * @brief Get temperature in degrees Celsius.
 * @return Temperature.
 * @note This function reads out the data of temperature measured last time.
 * To get the realtime temperature, the chip needs to be set to the force mode 
 * to start and complete a measurement(see @ref BMP280_PerformMeasurement) 
 * before the data is read when the continuous measuring is off.
 */
float BMP280_GetTemperature()
{
  uint8_t result;
  uint8_t temperature[3] = { 0 };
  float tmp;

  result = IIC_ReadRegBytes(BMP280_DEVICE_ADDR, BMP280_TEMP_MSB_ADDR, 3, temperature);
  if (result == BMP280_OK)
  {
    tmp = (int32_t)((((int32_t)(temperature[0])) << 12) | (((int32_t)(temperature[1])) << 4) | (((int32_t)(temperature[2])) >> 4));
    return BMP280_GetCompensatedTemperature(tmp);
  }
  return 999.9f;
}

/**
 * @brief Get pressure in hPa.
 * @return Pressure.
 * @note This function reads out the data of pressure measured last time.
 * To get the realtime pressure, the chip needs to be set to the force mode 
 * to start and complete a measurement(see @ref BMP280_PerformMeasurement) 
 * before the data is read when the continuous measuring is off.
 */
float BMP280_GetPressure()
{
  uint8_t result;
  uint8_t pressure[3] = { 0 };
  float tmp;

  result = IIC_ReadRegBytes(BMP280_DEVICE_ADDR, BMP280_PRES_MSB_ADDR, 3, pressure);
  if (result == BMP280_OK)
  {
    tmp = (int32_t)((((int32_t)(pressure[0])) << 12) | (((int32_t)(pressure[1])) << 4) | (((int32_t)(pressure[2])) >> 4));
    return BMP280_GetCompensatedPressure(tmp);
  }
  return 0.0f;
}

/**
 * @brief Get pressure in hPa and temperature in Celsius degree.
 * @param pressure This pointer outputs pressure in hPa.
 * @param temperature This pointer outputs temperature in Celsius degree.
 * @return Result of execution, see @ref DRV8825_return_status .
 * @note This function reads out the data of pressure and temperature measured last time.
 * Which means the chip needs to be set to the force mode to perform and complete a measurement 
 * before the data is read when the continuous measuring is off.
 */
uint8_t BMP280_GetTemperatureAndPressure(float* pressure, float* temperature)
{
  uint8_t result;
  int32_t tmpTemperature;
  int32_t tmpPressure;
  uint8_t temp[6] = { 0 };

  result = IIC_ReadRegBytes(BMP280_DEVICE_ADDR, BMP280_PRES_MSB_ADDR, 6, temp);
  if (result == BMP280_OK)
  {
    tmpPressure = (int32_t)((((uint32_t)(temp[0])) << 12) | (((uint32_t)(temp[1])) << 4) | ((uint32_t)temp[2] >> 4));
    tmpTemperature = (int32_t)((((int32_t)(temp[3])) << 12) | (((int32_t)(temp[4])) << 4) | (((int32_t)(temp[5])) >> 4));
  }
  *pressure = BMP280_GetCompensatedPressure(tmpPressure);
  *temperature = BMP280_GetCompensatedTemperature(tmpTemperature);
  return result;
}
