/**
 * @file    hcsr04.c
 * @author  Miaow
 * @version 0.1.0
 * @date    2018/12/07
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of GP2Y0E03, the active optical distance measurement sensor:
 *              1. Initialization
 *              2. Configurations
 *              3. Distance measurement
 * @note
 *          Minimum version of header file:
 *              0.1.0
 *          Pin connection:
 *          ┌────────┐     ┌────────┐     ┌────────┐
 *          │    TRIG├─────┤PE5  PE2├─────┤TRIG    │
 *          │    ECHO├─────┤PE3  PE4├─────┤ECHO    │
 *          └────────┘     └────────┘     └────────┘
 *           HCSR04_1      STM32F407       HCSR04_2
 *          
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#include "hcsr04.h"

static uint32_t period = 0;

/**
 * @brief 初始化两个超声波传感器.
 */
void HCSR04_Init()
{
    GPIO_InitTypeDef GPIO_InitStructer;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;
    NVIC_InitTypeDef NVIC_InitStructer;

    RCC_AHB1PeriphClockCmd(HCSR04_ALL_GPIO_CLK, ENABLE);
    HCSR04_TIMER_CLKFUN(HCSR04_TIMER_CLK, ENABLE);

    GPIO_InitStructer.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructer.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructer.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructer.GPIO_Pin = HCSR04_1_TRIG_GPIO_PIN;
    GPIO_Init(HCSR04_1_TRIG_GPIO_PORT, &GPIO_InitStructer);
    GPIO_InitStructer.GPIO_Pin = HCSR04_2_TRIG_GPIO_PIN;
    GPIO_Init(HCSR04_2_TRIG_GPIO_PORT, &GPIO_InitStructer);
    
    GPIO_InitStructer.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructer.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructer.GPIO_Pin = HCSR04_1_ECHO_GPIO_PIN;
    GPIO_Init(HCSR04_1_ECHO_GPIO_PORT, &GPIO_InitStructer);
    GPIO_InitStructer.GPIO_Pin = HCSR04_2_ECHO_GPIO_PIN;
    GPIO_Init(HCSR04_2_ECHO_GPIO_PORT, &GPIO_InitStructer);
    
    TIM_DeInit(HCSR04_TIMER);
    TIM_TimeBaseInitStructer.TIM_Period = HCSR04_TIMER_MAX_PERIOD;
    TIM_TimeBaseInitStructer.TIM_Prescaler = 168 - 1;
    TIM_TimeBaseInitStructer.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructer.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructer.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(HCSR04_TIMER, &TIM_TimeBaseInitStructer);
    
    period = (uint32_t)(HCSR04_MAX_DISTANCE * 1000.0f / 17.0f);
    
    NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructer.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructer.NVIC_IRQChannel = HCSR04_TIMER_IRQCHANNEL;
    NVIC_InitStructer.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructer);
    TIM_Cmd(HCSR04_TIMER, DISABLE);
}

/**
 * @brief 读取测距距离.
 * @param sensorX 指定的传感器, @ref HCSR04_select 里选一个.
 * @return 测距距离(cm).
 */
float HCSR04_ReadOutDistance(uint8_t sensorX)
{
    
    float length;
    volatile uint32_t i = 400;
    if(sensorX == 0x01)
    {
        HCSR04_1_TRIG_GPIO_PORT -> BSRRL |= HCSR04_1_TRIG_GPIO_PIN;//拉高信号，作为触发信号
        while(--i);
        HCSR04_1_TRIG_GPIO_PORT -> BSRRH |= HCSR04_1_TRIG_GPIO_PIN;//PA11=0
        while((HCSR04_1_ECHO_GPIO_PORT->IDR & HCSR04_1_ECHO_GPIO_PIN) == RESET);
        HCSR04_TIMER->CR1 |= TIM_CR1_CEN;//回响信号到来，开启定时器计数
        while((HCSR04_1_ECHO_GPIO_PORT->IDR & HCSR04_1_ECHO_GPIO_PIN) != RESET && TIM1->CNT < period);
    }
    else if(sensorX == 0x02)
    {
        HCSR04_2_TRIG_GPIO_PORT -> BSRRL |= HCSR04_2_TRIG_GPIO_PIN;//拉高信号，作为触发信号
        while(--i);
        HCSR04_2_TRIG_GPIO_PORT -> BSRRH |= HCSR04_2_TRIG_GPIO_PIN;//PA11=0
        while((HCSR04_1_ECHO_GPIO_PORT->IDR & HCSR04_2_ECHO_GPIO_PIN) == RESET);
        HCSR04_TIMER->CR1 |= TIM_CR1_CEN;//回响信号到来，开启定时器计数
        while((HCSR04_1_ECHO_GPIO_PORT->IDR & HCSR04_2_ECHO_GPIO_PIN) != RESET && TIM1->CNT < period);
    }
    
    HCSR04_TIMER->CR1 &= (uint16_t)~TIM_CR1_CEN;//关闭定时器
    if(HCSR04_TIMER->CNT >= period)
    {
        HCSR04_TIMER->CNT = 0;
        return HCSR04_MAX_DISTANCE;
    }
    
    length = HCSR04_TIMER->CNT * 0.017f;//通过回响信号计算距离
    HCSR04_TIMER->CNT = 0;//将TIM计数寄存器的计数值清零
    return length;//距离作为函数返回值
}
