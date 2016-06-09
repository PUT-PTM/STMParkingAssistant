#include "stm32f4xx_conf.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

void TRIG_GPIO_conf();
void ECHO_GPIO_conf();
void Timmer2_conf();
void Timmer3_conf();
void Przerwania2_conf();
void Przerwania3_conf();
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);

void start_sensor();
unsigned int sensor_pomiar();
