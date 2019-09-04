/**
 * @file    pid.h
 * @author  Miaow
 * @version 0.1.0
 * @date    2018/09/03
 * @brief   
 *          This file provides algorithm of PID (Proportion Integration Differentiation):
 *              1. Initialization
 *              2. Calculation
 * @note
 *          Minimum version of source file:
 *              0.1.0
 *             
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#ifndef __PID_H
#define __PID_H
#include "stdint.h"

/** 
 * @defgroup PID
 * @brief PID algorithm modules
 * @{
 */
 
/** 
 * @defgroup PID_config
 * @{
 */

#define PID_USE_INCREMENTAL           1 //!< This option switches incremental algorithm. (1:Incremental algorithm or 0:Position algorithm).
#define PID_USE_DEADZONE              1 //!< This option switches error dead zone control. (1:Enable or 0:Disable).
#define PID_USE_INTSEP                0 //!< This option switches integral separation. (1:Enable or 0:Disable).
#define PID_USE_ANTI_INTSAT           0 //!< This option switches anti-saturation funciton. Only valid in position pid. (1:Enable or 0:Disable).  

/**
 * @}
 */

typedef struct {
  float outputValue; //!< Output of PID algorithm.
  float error; //!< Error of input and output value.
  float kp, ki, kd; //!< PID parameters.
  float lastError; //!< This member stores errors in the last calculation.
  
  #if PID_USE_INCREMENTAL != 0
  float lastLastError;
  #else
  float integralValue;
  #endif
  
  #if PID_USE_ANTI_INTSAT != 0
  float maxActualValue; //!< This member defines upper limit of actual value.
  float minActualValue; //!< This member defines lower limit of actual value.
  #endif
  
  #if PID_USE_INTSEP != 0
  float integralSepration; //!< This member defines integral sepration threshold.
  #endif
  
  #if PID_USE_DEADZONE != 0
  float errorDeadZone;  //!< This member defines dead zone of error.
  #endif
}PID_InfoTypeDef;

void PID_Init(PID_InfoTypeDef* pidInfo, float kp, float ki, float kd);
float PID_Realize(PID_InfoTypeDef* pidInfo, float targetValue, float actualValue);
void PID_SetParameters(PID_InfoTypeDef* pidInfo, float kp, float ki, float kd);

/**
 * @}
 */

#endif
