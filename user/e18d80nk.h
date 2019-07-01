#ifndef __E18D80NK_H
#define __E18D80NK_H

#include "stdint.h"

#define E18D80NK_INT_GPIO_CLK				RCC_AHB1Periph_GPIOE
#define E18D80NK_INT_PIN					GPIO_Pin_10
#define E18D80NK_INT_PORT					GPIOE

#define E18D80NK_EXTI_PORT_SOURCE			EXTI_PortSourceGPIOE
#define E18D80NK_EXTI_PIN_SOURCE			EXTI_PinSource10
#define E18D80NK_EXTI_LINE					EXTI_Line10
#define E18D80NK_EXTI_IRQHANDLER			EXTI15_10_IRQHandler

#define E18D80NK_NVIC_IRQCHANNEL			EXTI15_10_IRQn

#define E18D80NK_STATE_NotTriggled          0
#define E18D80NK_STATE_Triggled             !E18D80NK_NotTriggled


void E18D80NK_Init(void (*irqHandler)(uint8_t state));
void E18D80NK_BeginReceive(void);
void E18D80NK_StopReceive(void);

#endif
