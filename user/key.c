/**
 * @file    key.c
 * @author  Alientek, Miaow
 * @version 0.1.1
 * @date    2019/06/30
 * @brief   
 *          This file provides bsp functions to manage the following 
 *          functionalities of keys:
 *              1. Initialization
 *              2. Scan function
 * @note
 *          Minimum version of header file:
 *              0.1.1
 *          Pin connection:
 *                         ©°©¤©¤©¤©¤©¤©¤©¤©´
 *                KEY0©¤©¤©¤©¤©¤©ÈPE4    ©¦
 *                KEY1©¤©¤©¤©¤©¤©ÈPE3    ©¦
 *                K_UP©¤©¤©¤©¤©¤©ÈPA0    ©¦
 *                         ©¸©¤©¤©¤©¤©¤©¤©¤©¼
 *                        STM32F407  
 *  
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
#include "key.h"
#include "utils.h" 

/** @addtogroup KEY
 * @{
 */

#define KEY_ALL_GPIO_CLK        (KEY_0_GPIO_CLK | KEY_1_GPIO_CLK | KEY_UP_GPIO_CLK)

#if KEY_0_PRESSED_STATUS == KEY_PRESSED_DOWN
#define KEY_0_IDR               ((KEY_0_PORT->IDR & KEY_0_PIN) != (uint32_t)Bit_RESET)
#else
#define KEY_0_IDR               ((KEY_0_PORT->IDR & KEY_0_PIN) == (uint32_t)Bit_RESET)
#endif

#if KEY_1_PRESSED_STATUS == KEY_PRESSED_DOWN
#define KEY_1_IDR               ((KEY_1_PORT->IDR & KEY_1_PIN) != (uint32_t)Bit_RESET)
#else
#define KEY_1_IDR               ((KEY_1_PORT->IDR & KEY_1_PIN) == (uint32_t)Bit_RESET)
#endif

#if KEY_UP_PRESSED_STATUS == KEY_PRESSED_DOWN
#define KEY_UP_IDR              ((KEY_UP_PORT->IDR & KEY_UP_PIN) != (uint32_t)Bit_RESET)
#else
#define KEY_UP_IDR              ((KEY_UP_PORT->IDR & KEY_UP_PIN) == (uint32_t)Bit_RESET)
#endif

/**
 * @brief Initialization.
 *
 */
void KEY_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(KEY_ALL_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;

  GPIO_InitStructure.GPIO_Pin = KEY_0_PIN;
  GPIO_InitStructure.GPIO_PuPd = (GPIOPuPd_TypeDef)KEY_0_PRESSED_STATUS;
  GPIO_Init(KEY_0_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = KEY_1_PIN;
  GPIO_InitStructure.GPIO_PuPd = (GPIOPuPd_TypeDef)KEY_1_PRESSED_STATUS;
  GPIO_Init(KEY_1_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = KEY_UP_PIN;
  GPIO_InitStructure.GPIO_PuPd = (GPIOPuPd_TypeDef)KEY_UP_PRESSED_STATUS;
  GPIO_Init(KEY_UP_PORT, &GPIO_InitStructure);
}

/**
 * @brief Scan for keys.
 * @return Pressed key, see @ref KEY_definition.
 * @note priority - KEY0 > KEY1 > WK_UP.
 */
uint8_t KEY_Scan()
{
  static uint8_t keyUp = 1;
  if (keyUp && (KEY_0_IDR == 0 || KEY_1_IDR == 0 || KEY_UP_IDR == 0))
  {
    UTILS_DelayMs(10);
    keyUp = 0;
    if (KEY_0_IDR == 0)
      return 1;
    else if (KEY_1_IDR == 0)
      return 2;
    else if (KEY_UP_IDR == 0)
      return 3;
  }
  else if (KEY_0_IDR && KEY_1_IDR && KEY_UP_IDR)
  {
    keyUp = 1;
  }
  return 0;
}
/**
 * @}
 */
