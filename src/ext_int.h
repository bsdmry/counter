/*
 * ext_int.h
 *
 *  Created on: 27 апр. 2018 г.
 *      Author: igor
 */

#ifndef EXT_INT_H_
#define EXT_INT_H_

#ifdef STM32F10X
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#endif

GPIO_InitTypeDef int_pins;
EXTI_InitTypeDef exti;
NVIC_InitTypeDef nvic0, nvic1, nvic2;

void exti0IRQ(void);
void exti1IRQ(void);
void exti2IRQ(void);

void hw_eint_init(void){
	GPIO_StructInit(&int_pins);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	int_pins.GPIO_Mode = GPIO_Mode_IPD;
	int_pins.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	int_pins.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &int_pins);

    /* Add IRQ vector to NVIC */
    /* PA0 is connected to EXTI_Line0, which has EXTI0_IRQn vector*/
	nvic0.NVIC_IRQChannel = EXTI0_IRQn;
	nvic0.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic0.NVIC_IRQChannelSubPriority = 0x00;
	nvic0.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic0);

    /* PA1 is connected to EXTI_Line1, which has EXTI1_IRQn vector*/
	nvic1.NVIC_IRQChannel = EXTI1_IRQn;
	nvic1.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic1.NVIC_IRQChannelSubPriority = 0x00;
	nvic1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic1);

    /* PA2 is connected to EXTI_Line2, which has EXTI2_IRQn vector*/
	nvic2.NVIC_IRQChannel = EXTI2_IRQn;
	nvic2.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic2.NVIC_IRQChannelSubPriority = 0x00;
	nvic2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic2);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0 | GPIO_PinSource1 | GPIO_PinSource2);
    /* PD0 is connected to EXTI_Line0 */
    exti.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2;
    /* Enable interrupt */
    exti.EXTI_LineCmd = ENABLE;
    /* Interrupt mode */
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    /* Triggers on rising edge */
    exti.EXTI_Trigger = EXTI_Trigger_Rising;
    /* Add to EXTI */
    EXTI_Init(&exti);
}

void EXTI0_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
		exti0IRQ();
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
void EXTI1_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
		exti1IRQ();
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
void EXTI2_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
		exti2IRQ();
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

#endif /* EXT_INT_H_ */
