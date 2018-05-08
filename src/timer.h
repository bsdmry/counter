/*
 * timer.h
 *
 *  Created on: 27 апр. 2018 г.
 *      Author: igor
 */

#ifndef TIMER_H_
#define TIMER_H_
#ifdef STM32F10X
#include "stm32f10x_tim.h"
#endif

#define  TIMER_PRESCALER 8000 - 1
#define  TIMER_PERIOD 8000 - 1

void tim4IRQ(void);

TIM_TimeBaseInitTypeDef timer;
NVIC_InitTypeDef nvic_tim4;

void hw_timer_init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseStructInit(&timer);
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Prescaler = TIMER_PRESCALER;
	timer.TIM_Period = TIMER_PERIOD;
	TIM_TimeBaseInit(TIM4, &timer);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);

	nvic_tim4.NVIC_IRQChannel = TIM4_IRQn;
	nvic_tim4.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_tim4.NVIC_IRQChannelSubPriority = 0;
	nvic_tim4.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_tim4);
	//NVIC_EnableIRQ(TIM4_IRQn);


}

void TIM4_IRQHandler()
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        tim4IRQ();
    }
}
#endif /* TIMER_H_ */
