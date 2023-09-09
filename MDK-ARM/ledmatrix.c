#include "ledmatrix.h"
#include "TIMER.h"
uint8_t matrix[8]={0x7e,0x99,0x00,0x00,0x00,0x81,0xc3,0x66};
volatile uint8_t currentRow = 0;
void ledmatrix_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* Configure SDA,SCK in output pushpull mode */
  GPIO_InitStruct.GPIO_Pin = sda_pin|sck_pin|lat_pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void hc595_shift_matrix(uint8_t data)
{
	for (int8_t i=7;i>= 0;i--){
		if(data& (1<<i))
			GPIO_SetBits (GPIOB, sda_pin);
		else
			GPIO_ResetBits (GPIOB, sda_pin);
		GPIO_SetBits (sck_port, sck_pin);
		GPIO_ResetBits (sck_port, sck_pin);
	}
GPIO_SetBits (lat_port, lat_pin);
GPIO_ResetBits (lat_port, lat_pin);
}
//void display_matrix(void)
//{
//	for(int8_t i=7;i>=0;i--)
//	{	
//		GPIO_ResetBits(lat_port,lat_pin);
//		hc595_shift_matrix(matrix[i]);	
//		hc595_shift_matrix((1<<i));
//		GPIO_SetBits(lat_port,lat_pin);
//		Delay_Ms(1);
//	}
//}
void TIM5_SampleTime(void)		//sample time=10ms
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	/* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	/*Configure Timer for Encoder */
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 1000-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 83;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	
	// Configure the encoder interrupt
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Start Timer 4
	TIM_Cmd(TIM5, ENABLE);
}
void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) == SET) {
		GPIO_ResetBits(lat_port,lat_pin);
		hc595_shift_matrix(matrix[currentRow]);	
		hc595_shift_matrix((1<<currentRow));
		GPIO_SetBits(lat_port,lat_pin);
		currentRow = (currentRow + 1) % 8;
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	}
}