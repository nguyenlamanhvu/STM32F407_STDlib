#include "TIMER.h"

void TIMER_Configuration(){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct; 
	/* Tim6 Peripheral clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 83;
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);
	TIM_Cmd(TIM6,ENABLE); 
}

void Delay_Ms(uint16_t DelayCounter){				//delay DelayCounter ms
		while(DelayCounter){
				TIM_SetCounter(TIM6,0U);
				while(TIM_GetCounter(TIM6)<1000U);
				DelayCounter--;
		}
}
void Delay_Us(uint16_t DelayCounter){			//delay DelayCounter us
		while(DelayCounter){
				TIM_SetCounter(TIM6,0U);
				while(TIM_GetCounter(TIM6)<1U);
				DelayCounter--;
		}
}
