#include "stm32f4xx_conf.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
//#include <stdio.h>

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

unsigned int counter, odleglosc;

int main(void)
{
	Timmer2_conf();
	Timmer3_conf();
	Przerwania2_conf();
	Przerwania3_conf();
	TRIG_GPIO_conf();
	ECHO_GPIO_conf();
	SystemInit();
	GPIOD->BSRRH = GPIO_Pin_1;
    while(1)
    {
    	ile=0;
    	GPIOD->BSRRL=GPIO_Pin_1;
    	TIM_Cmd(TIM2,ENABLE);
    	while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)==0);
    	TIM_Cmd(TIM3,ENABLE);
    	while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)==1);
    	TIM_Cmd(TIM3,DISABLE);
    	odleglosc=ile/58;
    	//printf("%i",odleglosc);
    }
}
