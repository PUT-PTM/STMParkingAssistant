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

int main(void)
{
	SystemInit();
	LED_GPIO_conf();
	start_sd();
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
    while(1)
    {
    }
}
