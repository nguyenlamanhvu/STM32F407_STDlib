#include "LCD.h"

void OUT_LCD(char *string,uint8_t x,uint8_t y){
	uint8_t data;
	if(x==1)		//x=1:line 1
	{
		data=0x80+y;
		cmdwrite(data);
	}
	else if(x==2)//x=2: line 2
	{
		data=0xC0+y;
		cmdwrite(data);
	}
	int i=0;
	while(*string){
		datawrite(*string++);
	}
}
void CLEAR_LCD(void)
{
	uint8_t data=0x01;
	cmdwrite(data);
}
void init_lcd(){
		GPIO_Configuration();
		TIMER_Configuration();
	
		uint8_t tmp;
	
		Delay_Ms(50);		//Delay 25ms
		tmp=0x30;				//ghi lenh 0x30 lan 1
		cmdwrite(tmp);
		Delay_Ms(5);		//Delay 5ms
		tmp=0x30;				//ghi lenh 0x30 lan 2
		cmdwrite(tmp);
		Delay_Us(200);	//Delay 200us
		tmp=0x30;				//ghi lenh 0x30 lan 3
		cmdwrite(tmp);
		
		tmp=0x38;				//2 dong font 5x8
		cmdwrite(tmp);
		tmp=0x01;				//xoa man hinh
		cmdwrite(tmp);
		tmp=0x0C;				//man hinh on, con tro off
		cmdwrite(tmp);
		tmp=0x06;				//dich phai con tro, d/c DDRAM tang 1 khi ghi data
		cmdwrite(tmp);
}
void cmdwrite(uint8_t data){
		xulyhex(data);
		GPIO_WriteBit(GPIOA,RS,Bit_RESET);
		GPIO_WriteBit(GPIOA,RW,Bit_RESET);
		GPIO_WriteBit(GPIOB,E,Bit_SET);
		Delay_Us(5);
		GPIO_WriteBit(GPIOB,E,Bit_RESET);
		Delay_Ms(2);
}
void datawrite(uint8_t data){
		xulyhex(data);
		GPIO_WriteBit(GPIOA,RS,Bit_SET);
		GPIO_WriteBit(GPIOA,RW,Bit_RESET);
		GPIO_WriteBit(GPIOB,E,Bit_SET);
		Delay_Us(5);
		GPIO_WriteBit(GPIOB,E,Bit_RESET);
		Delay_Ms(2);
}
void xulyhex(uint8_t data){
		GPIO_WriteBit(GPIOE,GPIO_Pin_1,(data>>7)&1);		//D7
		GPIO_WriteBit(GPIOB,GPIO_Pin_4,(data>>6)&1);		//D6
		GPIO_WriteBit(GPIOB,GPIO_Pin_5,(data>>5)&1);		//D5
		GPIO_WriteBit(GPIOE,GPIO_Pin_0,(data>>4)&1);		//D4
		GPIO_WriteBit(GPIOB,GPIO_Pin_3,(data>>3)&1);		//D3
		GPIO_WriteBit(GPIOC,GPIO_Pin_11,(data>>2)&1);		//D2
		GPIO_WriteBit(GPIOC,GPIO_Pin_10,(data>>1)&1);		//D1
		GPIO_WriteBit(GPIOC,GPIO_Pin_12,(data>>0)&1);		//D0
}
