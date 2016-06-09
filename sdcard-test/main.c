#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_spi.h"
#include "misc.h"
#include "fatfs/ff.h"
#include "fatfs/spi_sd.h"
#include "delay/delay.h"
#include "lib_cm4/fpu.h"
#include "mp3.h"
#include "disp.h"
#include "sensor.h"

void Timmer4_conf(float czas){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitTypeDef Timmer;
	Timmer.TIM_Period = (84000000)/(10000/czas) -1; //czas seconds
	Timmer.TIM_Prescaler = 9999;
	Timmer.TIM_ClockDivision = TIM_CKD_DIV1;
	Timmer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &Timmer);

	TIM_Cmd(TIM4, ENABLE);
}

void Przerwania4_conf(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// ustawienie trybu pracy priorytetow przerwan

	NVIC_InitTypeDef przerwania;
	przerwania.NVIC_IRQChannel = TIM4_IRQn;
	przerwania.NVIC_IRQChannelPreemptionPriority = 0x00;
	przerwania.NVIC_IRQChannelSubPriority = 0x00;
	przerwania.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&przerwania);

	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
}

void TIM4_IRQHandler(void){
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET){
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		play_mp3("new.mp3");
	}
}

void LED_GPIO_conf(){
		/* GPIOD Periph clock enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		GPIO_InitTypeDef  GPIO_InitStructure;
		/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
}

unsigned int odleglosc;
int i;

void parking_assistant(){
	for(i=0;i<20000000;i++);
	odleglosc=sensor_pomiar();
	display(odleglosc);/*
	if(odleglosc<20){
		Timmer4_conf(0.5);
		Przerwania4_conf();
	}*/
}

int main(void)
{
	SystemInit();
	LED_GPIO_conf();
	//start_sensor();
	start_sd();
	start_disp();
	int i;
	i=otwarcie_pliku();
	switch(i){
	case 1: GPIO_SetBits(GPIOD,GPIO_Pin_12);
		break;
	case 2: GPIO_SetBits(GPIOD,GPIO_Pin_13);
		break;
	case 3: GPIO_SetBits(GPIOD,GPIO_Pin_14);
		break;
	case 4: GPIO_SetBits(GPIOD,GPIO_Pin_15);
		break;
	}

	//play_mp3("new.mp3");
    while(1)
    {
		parking_assistant();
    }
}

