#define STM32F10X
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "diag/Trace.h"
#include "delay.h"
#include "ext_int.h"
#include "timer.h"
#include "spi.h"
#include "lcd5110.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#define ARR_SIZE 40
uint8_t period = ARR_SIZE;
uint16_t ac1[ARR_SIZE], ac2[ARR_SIZE], ac3[ARR_SIZE] = {0};
uint16_t diff = 0;
volatile uint8_t start_calc = 0;
volatile uint16_t gcnt1, gcnt2, gcnt3 = 0;


int
main(int argc, char* argv[])
{
	blink_init();
	hw_eint_init();
	hw_timer_init();
	timer_start();
	hw_spi_init();
	init_lcd_pins();
	lcd5110_init();
	lcd5110_print("Geiger counter", 14, 0, 0, 0);

  while (1)
    {
      if (start_calc == 1){
              calc_diff();
              show_cnt();
              start_calc = 0;
      }
    }
}

void blink_init(void){
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	  GPIO_ResetBits(GPIOC, GPIO_Pin_13); // Set C13 to Low level ("0")
}

void exti0IRQ(void){
	gcnt1++;

}
void exti1IRQ(void){
	gcnt2++;
}
void exti2IRQ(void){
	gcnt3++;
}

void calc_diff(void) {
	for (int i = 1; i < period; i++) {
		ac1[i - 1] = ac1[i];
		ac2[i - 1] = ac2[i];
		ac3[i - 1] = ac3[i];
	}
	ac1[period - 1] = gcnt1;
	ac2[period - 1] = gcnt2;
	ac3[period - 1] = gcnt3;
	//if (((ac1[period] >= ac1[0]) && (ac2[period] >= ac2[0]))
	//		&& (ac3[period] >= ac3[0])) {
		diff = (ac1[period - 1] - ac1[0]) + (ac2[period - 1] - ac2[0])
				+ (ac3[period - 1] - ac3[0]);
	//} else {
	//	diff = 0;
	//}
}

void show_cnt(void){
        unsigned char buffer[12];
        float urh = 0.0;
        memset(buffer, 0, 12);
        //urh = diff * 1.53;
        urh = diff * 0.3;
        sprintf(buffer, "%6.1f", urh);
        lcd5110_print(buffer, strlen(buffer), 0, 2, 1);
}

void tim4IRQ(void){
	start_calc = 1;
	GPIOC->ODR ^= GPIO_Pin_13; // Invert C13
}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
