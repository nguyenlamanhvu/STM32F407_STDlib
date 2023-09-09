#include "led7seg.h"
#include "TIMER.h"
uint8_t led7seg[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
volatile uint8_t digitIndex = 0;
volatile uint8_t digits[4] = {0, 0, 0, 0};
uint32_t data_7seg=0;
extern volatile int32_t speed;
uint16_t count=0;
uint16_t hc05send = 0;
void led7seg_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	/* Configure SDA,SCK in output pushpull mode */
  GPIO_InitStruct.GPIO_Pin = sda_pin|sck_pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(sda_port, &GPIO_InitStruct);
	
	/* Configure LAT in output pushpull mode */
  GPIO_InitStruct.GPIO_Pin = lat_pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(lat_port, &GPIO_InitStruct);
}
void hc595_shift(uint8_t data)
{
	for (int8_t i=7;i>= 0;i--){
		if(data& (1<<i))
			GPIO_SetBits (GPIOD, sda_pin);
		else
			GPIO_ResetBits (GPIOD, sda_pin);
		GPIO_SetBits (sck_port, sck_pin);
		GPIO_ResetBits (sck_port, sck_pin);
	}
GPIO_SetBits (lat_port, lat_pin);
GPIO_ResetBits (lat_port, lat_pin);
}
void display_7seg(uint32_t data)
{
	uint32_t number=data;
//	uint8_t thousands,hundreds,tens,units;
//	thousands=number/1000;
//	number%=1000;
//	hundreds=number/100;
//	number%=100;
//	tens=number/10;
//	units=number%10;
	
	digits[0]=number/1000;
	number%=1000;
	digits[1]=number/100;
	number%=100;
	digits[2]=number/10;
	digits[3]=number%10;
//	GPIO_ResetBits(lat_port,lat_pin);
//	hc595_shift(0x01);
//	hc595_shift(led7seg[thousands]);
//	GPIO_SetBits(lat_port,lat_pin);
//	Delay_Ms(5);
//	
//	GPIO_ResetBits(lat_port,lat_pin);
//	hc595_shift(0x02);
//	hc595_shift(led7seg[hundreds]);
//	GPIO_SetBits(lat_port,lat_pin);
//	Delay_Ms(5);
//	
//	GPIO_ResetBits(lat_port,lat_pin);
//	hc595_shift(0x04);
//	hc595_shift(led7seg[tens]);
//	GPIO_SetBits(lat_port,lat_pin);
//	Delay_Ms(5);
//	
//	GPIO_ResetBits(lat_port,lat_pin);
//	hc595_shift(0x08);
//	hc595_shift(led7seg[units]);
//	GPIO_SetBits(lat_port,lat_pin);
//	Delay_Ms(5);
}
void TIM7_SampleTime(void)		//sample time=10ms
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	/* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	/*Configure Timer for Encoder */
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 5000-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 83;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	
	// Configure the encoder interrupt
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Start Timer 4
	TIM_Cmd(TIM7, ENABLE);
}
void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET) {
		if(count==200){
			count=0;
			display_7seg(speed);
			hc05send++;
		}
		GPIO_ResetBits(lat_port,lat_pin);
		hc595_shift(1<<digitIndex);
		hc595_shift(led7seg[digits[digitIndex]]);
		GPIO_SetBits(lat_port,lat_pin);
		digitIndex = (digitIndex + 1) % 4;
		count++;
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}