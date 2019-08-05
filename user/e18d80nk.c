#include "e18d80nk.h"
#include "utils.h"

void (*E18D80NK_IrqHandler)(uint8_t state);

void E18D80NK_Init(void (*irqHandler)(uint8_t state))
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    //GPIO
    RCC_AHB1PeriphClockCmd(E18D80NK_INT_GPIO_CLK, ENABLE);//使能GPIO时钟
    GPIO_InitStructure.GPIO_Pin = E18D80NK_INT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(E18D80NK_INT_PORT, &GPIO_InitStructure);//初始化
    //SYSCFG
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(E18D80NK_EXTI_PORT_SOURCE, E18D80NK_EXTI_PIN_SOURCE);
    //EXTI
    EXTI_InitStructure.EXTI_Line = E18D80NK_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    //IRQ
    E18D80NK_IrqHandler = irqHandler;
}

void E18D80NK_BeginReceive()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //NVIC
    NVIC_InitStructure.NVIC_IRQChannel = E18D80NK_NVIC_IRQCHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}

void E18D80NK_StopReceive()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //NVIC
    NVIC_InitStructure.NVIC_IRQChannel = E18D80NK_NVIC_IRQCHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}

void E18D80NK_EXTI_IRQHANDLER()
{
    if((EXTI->PR & E18D80NK_EXTI_LINE) != (uint32_t)RESET)
    {
        E18D80NK_IrqHandler(!(E18D80NK_INT_PORT->IDR & E18D80NK_INT_PIN));
        EXTI->PR = E18D80NK_EXTI_LINE;
    }
}
