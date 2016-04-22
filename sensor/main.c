#include "stm32f4xx_conf.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

void Timmer_conf(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef Timmer2;
	Timmer2.TIM_Period = 999; /*1 milisecond*/
	Timmer2.TIM_Prescaler = 83; /*1 microsecond tick*/
	Timmer2.TIM_ClockDivision = TIM_CKD_DIV1;
	Timmer2.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &Timmer2);

	TIM_Cmd(TIM2, ENABLE);
}

void Przerwania_conf(){
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

void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET){
		GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

unsigned int counter = TIM2->CNT;

int main(void)
{
	Timmer_conf();
	Przerwania_conf();
	SystemInit();
    while(1)
    {
    	counter = TIM2->CNT;
    }
}
