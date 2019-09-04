/**
 * @file    pid.c
 * @author  Miaow
 * @version 0.1.0
 * @date    2018/09/03
 * @brief   
 *          This file provides algorithm of PID (Proportion Integration Differentiation):
 *              1. Initialization
 *              2. Calculation
 * @note
 *          Minimum version of header file:
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
 
#include "pid.h"
#include "utils.h"

#define PID_ABS(a)            ((a) < 0 ? -(a) : (a))

/** @addtogroup TB6612FNG
 * @{
 */

/**
 * @brief Initialize the PID structure.
 * @param pidInfo Pointer to the PID structure.
 * @param kp Proportional gain.
 * @param ki Integral gain.
 * @param kd Derivative gain.
 */
void PID_Init(PID_InfoTypeDef* pidInfo, float kp, float ki, float kd)
{
  pidInfo->outputValue = 0.0f;
  pidInfo->error = 0.0f;
  pidInfo->lastError = 0.0f;

  pidInfo->kp = kp;
  pidInfo->ki = ki;
  pidInfo->kd = kd;

  #if PID_USE_INCREMENTAL != 0
  pidInfo->lastLastError = 0.0f;
  #else
  pidInfo->integralValue = 0.0f;
  #endif

  #if PID_USE_ANTI_INTSAT != 0
  pidInfo->maxActualValue = 400.0f;
  pidInfo->minActualValue = -200.0f;
  #endif

  #if PID_USE_INTSEP != 0
  pidInfo->integralSepration = 200.0f;
  #endif

  #if PID_USE_DEADZONE != 0
  pidInfo->errorDeadZone = 2.0f;
  #endif
}

/**
 * @brief Set the three PID parameters.
 * @param pidInfo Pointer to the PID structure.
 * @param kp Proportional gain.
 * @param ki Integral gain.
 * @param kd Derivative gain.
 */
void PID_SetParameters(PID_InfoTypeDef* pidInfo, float kp, float ki, float kd)
{
  pidInfo->kp = kp;
  pidInfo->ki = ki;
  pidInfo->kd = kd;
}

#if PID_USE_INCREMENTAL != 0

/**
 * @brief This funciton Iterates a PID opetation.
 * @param pidInfo Pointer to the PID structure.
 * @param targetValue Output target.
 * @param actualValue Actual output from feedback.
 * @return PID output. Also pidInfo->outputValue.
 * @note This function should be executed regularly.
 */
float PID_Realize(PID_InfoTypeDef* pidInfo, float targetValue, float actualValue)
{
  float increment, beta;

  #if PID_USE_DEADZONE != 0 ||  PID_USE_INTSEP != 0
  float errorAbs;
  #endif

  //Get error
  pidInfo->error = targetValue - actualValue;
  #if PID_USE_DEADZONE != 0 ||  PID_USE_INTSEP != 0
  errorAbs = PID_ABS(pidInfo->error);
  #endif

  #if PID_USE_DEADZONE != 0
  //Dead zone judgment
  if (errorAbs <= pidInfo->errorDeadZone)
    return pidInfo->outputValue;
  #endif

  #if PID_USE_INTSEP != 0
  //Integral separation
  if (errorAbs > pidInfo->integralSepration)
    beta = 0.0f;
  else
  #endif
    beta = 1.0f;
  
  //Calculate increment
  increment = pidInfo->kp * (pidInfo->error - pidInfo->lastError) +
     beta * pidInfo->ki * pidInfo->error +
     pidInfo->kd * (pidInfo->error - 2 * pidInfo->lastError + pidInfo->lastLastError);
  pidInfo->outputValue += increment;

  pidInfo->lastLastError = pidInfo->lastError;
  pidInfo->lastError = pidInfo->error;
  return pidInfo->outputValue;
}

#else
/**
 * @brief This funciton Iterates a PID opetation.
 * @param pidInfo Pointer to the PID structure.
 * @param targetValue Output target.
 * @param actualValue Actual output from feedback.
 * @return PID output. Also pidInfo->outputValue.
 * @note This function should be executed regularly.
 */
float PID_Realize(PID_InfoTypeDef* pidInfo, float targetValue, float actualValue)
{
  float  beta;
  #if PID_USE_DEADZONE != 0 ||  PID_USE_INTSEP != 0
  float errorAbs;
  #endif
  
  //Get error
  pidInfo->error = targetValue - actualValue;
  
  #if PID_USE_DEADZONE != 0 ||  PID_USE_INTSEP != 0
  errorAbs = PID_ABS(pidInfo->error);
  #endif
  
  #if PID_USE_DEADZONE != 0
  //Dead zone judgment
  if (errorAbs <= pidInfo->errorDeadZone)
    return pidInfo->outputValue;
  #endif
  
  #if PID_USE_INTSEP != 0
  //Integral separation
  if (errorAbs > pidInfo->integralSepration)
  {
    beta = 0.0f;
  }
  else
  {
  #endif
    beta = 1.0f;
    #if PID_USE_ANTI_INTSAT != 0
    //Anti windup
    if (actualValue > pidInfo->maxActualValue)
    {
      if (pidInfo->error < 0.0f)
        pidInfo->integralValue += pidInfo->error;
    }
    else if (actualValue < pidInfo->minActualValue)
    {
      if (pidInfo->error > 0.0f)
        pidInfo->integralValue += pidInfo->error;
    }
    else
    {
    #endif
      pidInfo->integralValue += pidInfo->error;
    #if PID_USE_ANTI_INTSAT != 0
    }
    #endif
    
  #if PID_USE_INTSEP != 0
  }
  #endif
  
  //Calculate position
  pidInfo->outputValue = pidInfo->kp * pidInfo->error + 
    beta * pidInfo->ki * pidInfo->integralValue + 
    pidInfo->kd * (pidInfo->error - pidInfo->lastError);
  
  pidInfo->lastError = pidInfo->error;
  return pidInfo->outputValue;
}

/**
 * @}
 */

#endif
