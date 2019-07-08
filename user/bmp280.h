/**
 * @file    bmp280.h
 * @author  Miaow
 * @version 0.1.0
 * @date    2019/07/07
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of BMP280:
 *              1. Communicate using I2C
 *              2. Initialization and Configuration
 *              3. Measurie and read out temperature and pressure
 * @note     
 *          Minimum version of source file:
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
 *
 */
#ifndef __BMP280_H
#define __BMP280_H

#include "utils.h"

/** 
 * @defgroup DMP280
 * @brief DMP280 driver
 * @{
 */
 
#define BMP280_DEVICE_ADDR              0x76 //!> I2C address of the chip.

/** 
 * @defgroup DRV8825_configuration
 * @brief Uncomment one of the macro define to use the preset parameters.
 * See the datasheet for details.
 * @{
 */

//#define BMP280_APPLICAITION_HANDHELD_DEVICE_LOW_POWER
//#define BMP280_APPLICAITION_HANDHELD_DEVICE_DYNAMIC
//#define BMP280_APPLICATION_WEATHER_MONITORING
//#define BMP280_APPLICATION_FLOOR_CHANGE_DETECTION
//#define BMP280_APPLICATION_DROP_DETECTION
//#define BMP280_APPLICATION_INDOOR_NAVIGATION
//Set each parameter manually.
#define BMP280_APPLICATION_CUSTOM
/**
 * @}
 */
 
//Custom configuration.
#if defined(BMP280_APPLICATION_CUSTOM)
#define BMP280_MODE                       BMP280_FORCED_MODE //!> Power mode, see @ref BMP280_Mode .
#define BMP280_PRESSURE_OVER_SAMPLING     BMP280_OS_16X //!> Pressure accuracy, see @ref BMP280_OverSampling .
#define BMP280_TEMPERATURE_OVER_SAMPLING  BMP280_OS_16X //!> Temperature accuracy, see @ref BMP280_OverSampling .
#define BMP280IIR_FILTER                  BMP280_FILTER_OFF //!> IIR filter option, see @ref BMP280_IirFilter .
#define BMP280_STANDBY_TIME               BMP280_ODR_125_MS //!> This parameter infulences frequency of measurement, @ref BMP280_StandbyTime .
#endif

/** 
 * @defgroup DRV8825_return_status
 * @{
 */
#define BMP280_OK                       0
#define BMP280_ERROR                    1
/**
 * @}
 */

uint8_t BMP280_Init(void);
float BMP280_GetTemperature(void);
float BMP280_GetPressure(void);
uint8_t BMP280_GetTemperatureAndPressure(float* pressure, float* temperature);
uint8_t BMP280_PerformMeasurement(void);

/**
 * @}
 */

#endif
