#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "defines.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include "stm32f4xx_conf.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"


void TRIG_GPIO_conf(){
		/* GPIOD Periph clock enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		GPIO_InitTypeDef  GPIO_InitStructure;
		/* Configure PD1 in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void ECHO_GPIO_conf(){
		/* GPIOD Periph clock enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		GPIO_InitTypeDef  GPIO_InitStructure;
		/* Configure PD1 in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void Timmer2_conf(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef Timmer2;
	Timmer2.TIM_Period = 9; /*10 microseconds*/
	Timmer2.TIM_Prescaler = 83; /*1 microsecond tick*/
	Timmer2.TIM_ClockDivision = TIM_CKD_DIV1;
	Timmer2.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &Timmer2);

}

void Timmer3_conf(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInitTypeDef Timmer3;
		Timmer3.TIM_Period = 9; /*999=1 milisecond*/
		Timmer3.TIM_Prescaler = 83; /*1 microsecond tick*/
		Timmer3.TIM_ClockDivision = TIM_CKD_DIV1;
		Timmer3.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &Timmer3);
}

void Przerwania2_conf(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// ustawienie trybu pracy priorytetow przerwan

	NVIC_InitTypeDef przerwania;
	przerwania.NVIC_IRQChannel = TIM2_IRQn;
	przerwania.NVIC_IRQChannelPreemptionPriority = 0x00;
	przerwania.NVIC_IRQChannelSubPriority = 0x00;
	przerwania.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&przerwania);

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void Przerwania3_conf(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// ustawienie trybu pracy priorytetow przerwan

	NVIC_InitTypeDef przerwania;
	przerwania.NVIC_IRQChannel = TIM3_IRQn;
	przerwania.NVIC_IRQChannelPreemptionPriority = 0x00;
	przerwania.NVIC_IRQChannelSubPriority = 0x00;
	przerwania.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&przerwania);

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

int ile=0;

void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET){
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		GPIO_WriteBit(GPIOD,GPIO_Pin_1,0);
		TIM_Cmd(TIM2, DISABLE);
	}
}

void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET){
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		ile=ile+10;
	}
}

void display(unsigned int odleglosc,char str[]){
	if(odleglosc<100) TM_ILI9341_Puts(110, 250, str, &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
	    	else TM_ILI9341_Puts(110, 250, ">100", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
	    	if(odleglosc<10&&odleglosc>0) {
	    		TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    	}
	    	else if(odleglosc<20&&odleglosc>=10) {
	    		TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    		TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    	    }
	    	else if(odleglosc<30&&odleglosc>=20) {
	    	    TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    	    }
	    	else if(odleglosc<40&&odleglosc>=30) {
	    	    TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    	    }
	    	else if(odleglosc<50&&odleglosc>=40) {
	    	    TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_BLACK);
	    		TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_BLACK);
	    		TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    	  	}
	    	else if(odleglosc<60&&odleglosc>=50) {
	    		TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    		TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_BLACK);
	    	   	TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_BLACK);
	    	  	TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_BLACK);
	    	  	TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_BLACK);
	    	 	TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_RED);
	    	   	TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    	 	TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    	  	}
	    	else if(odleglosc<70&&odleglosc>=60) {
	    		TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    		TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_BLACK);
	    	   	TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_BLACK);
	    	  	TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_BLACK);
	    	  	TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_BLACK);
	    	 	TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_BLACK);
	    	   	TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    	 	TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    		}
	    	else if(odleglosc<80&&odleglosc>=70) {
	    		TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    		TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_BLACK);
	    	   	TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_BLACK);
	    	  	TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_BLACK);
	    	  	TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_BLACK);
	    	 	TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_BLACK);
	    	   	TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_BLACK);
	    	 	TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    		}
	    	else if(odleglosc>=80) {
	    		TM_ILI9341_DrawFilledRectangle(30, 30, 210, 225, ILI9341_COLOR_BLACK);
	    	}
}

unsigned int odleglosc;

int main(void) {
    //Initialize system
    SystemInit();

    int i;
    //Initialize ILI9341
    TM_ILI9341_Init();
    //Rotate LCD for 90 degrees
    //TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_2);
    //FIll lcd with color
    TM_ILI9341_Fill(ILI9341_COLOR_BLACK);
    //Draw red filled rectangle
    TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_RED);

    Timmer2_conf();
    Timmer3_conf();
    Przerwania2_conf();
    Przerwania3_conf();
    TRIG_GPIO_conf();
    ECHO_GPIO_conf();
    SystemInit();
    GPIOD->BSRRH = GPIO_Pin_1;

    char str[100];
    while (1) {
    	for(i=0;i<20000000;i++);
    	ile=0;
    	GPIOD->BSRRL=GPIO_Pin_1;
    	TIM_Cmd(TIM2,ENABLE);
    	while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)==0);
    	TIM_Cmd(TIM3,ENABLE);
    	while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)==1);
    	TIM_Cmd(TIM3,DISABLE);
    	odleglosc=ile/58;

    	itoa(odleglosc, str, 10);
    	TM_ILI9341_DrawFilledRectangle(110,250,160,270,ILI9341_COLOR_BLACK);
    	display(odleglosc,str);

    }
}
