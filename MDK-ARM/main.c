#include "stm32f4xx.h"
#include "gpio.h"
#include "TIMER.h"
#include "LCD.h"
#include "ADC.h"
#include "USART.h"
#include "stdio.h"
#include "math.h"
#include "USART.h"
#include "led7seg.h"
#include "ledmatrix.h"
#include "PWM.h"
#include "I2C.h"
#include "EXTI.h"
extern uint16_t ADC_Value[5];		//dung 5 channel cua ADC
char sAdcValue[50];
uint8_t BCDdata[7];		//du lieu cua RTC
uint8_t DECdata[7];		//du lieu cua RTC
uint8_t motorstate=0;
extern volatile int32_t speed;
extern volatile float position;
extern double Accel_X;
extern double Accel_Y;
extern double Accel_Z;
extern double Gyro_X;
extern double Gyro_Y;
extern double Gyro_Z;
extern uint16_t Accel_data[6];
extern uint16_t Gyro_data[6];
char I2C_Value[50];
extern char receive_usart[BufferSize];
double VAR;					//ADC_Channel_10
double Humidity;		//ADC_Channel_11
double Temperature;	//ADC_Channel_12
double Temperature_C;	//ADC_Channel_12
uint16_t joystickY;		//ADC_Channel_13
uint16_t joystickX;		//ADC_Channel_14
extern volatile uint8_t LCD_code;
extern uint16_t hc05send;
//char password[6]="301103";
//extern char receive_usart[BufferSize];
//extern char receive_buffer[50];
//char correct[10]="correct";
//char incorrect[10]="incorrect";
//char nhapchu[20]="Nhap password:";

int i=0;
void USART_RXNE(void){
	GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
	//static uint8_t buffer_index=0;
	//receive_buffer[buffer_index]=USART2->DR;
	//buffer_index++;
	//if((USART_GetITStatus (USART2,USART_IT_IDLE))==0)	buffer_index=0;
}
uint8_t BCD_DEC(uint8_t data){
	uint8_t tempt1=data/16;
	uint8_t tempt2=data%16;
	return (10*tempt1+tempt2);
}
int main(void){
	GPIO_Configuration();

	DMA1_USART2_init();
 	USART2_Init();
	ledmatrix_init();
	led7seg_init();
	TIMER_Configuration();
	TIM7_SampleTime();
	TIM5_SampleTime();

	ADC1_init();			//khoi tao ADC
	init_lcd();				//khoi tao LCD
	GPIO_TIM_init();
	TIMER_TIM_init();
	PWM_TIM_init();
//Khoi dong Encoder
	TIM3_Config();
	TIM4_SampleTime();

	I2C_init(I2C1);
	//RTC_init();
	I2C_init(I2C2);
	MPU6050_Init(I2C2);
	EXTILine0_Config();
	while(1)
	{
	LCD_code=receive_usart[0];
	VAR=ADC_Value[0]*3.3/4095.0;
	/* Calculate huminity & temperature */
	Humidity=100.0*(double)ADC_Value[1]*3.3/(3.0*4095.0);
	if(Humidity>=100.0) Humidity=100.0; //Limiting value huminity within 100%
	Temperature=-273.15+(1.0/((log((4095.0*5.0/((double)ADC_Value[2]*3.0)-0.85)/10.0)/3435.0)+1.0/(25.0+273.15)));
	Temperature_C=(Temperature-32)*5.0/9.0;
	joystickY=ADC_Value[3];
	joystickX=ADC_Value[4];
//	if(speed==0)	motorstate=0;
//	else if(speed>0 && speed <2400)	motorstate=1;
//	else motorstate=2;
	speed_pwm();
	if(LCD_code==0){
		/*Display LCD*/
		CLEAR_LCD();
		sprintf(I2C_Value, "Thu %d %d/%d/%d", DECdata[3],DECdata[4],DECdata[5],DECdata[6]);
		OUT_LCD(I2C_Value,1,0);		//line 1, vi tri dau tien
		sprintf(I2C_Value, "%d:%d:%d T:%.2lf", DECdata[2],DECdata[1],DECdata[0],Temperature_C);
		OUT_LCD(I2C_Value,2,0);		//line 2, vi tri dau tien
//		if(motorstate==0)	OUT_LCD("STOP",2,10);
//		else if(motorstate==1)	OUT_LCD("LOW",2,10);
//		else OUT_LCD("FAST",2,10);
		Delay_Ms(900);
	}
	else if(LCD_code==1){
		CLEAR_LCD();
		sprintf(I2C_Value, "%.2lf %.2lf %.2lf", Accel_X,Accel_Y,Accel_Z);
		OUT_LCD(I2C_Value,1,0);		//line 1, vi tri dau tien
		sprintf(I2C_Value, "%.2lf %.2lf %.2lf", Gyro_X,Gyro_Y,Gyro_Z);
		OUT_LCD(I2C_Value,2,0);		//line 2, vi tri dau tien
		Delay_Ms(1000);
	}
	else if(LCD_code==2){
		sprintf(sAdcValue, "Speed: %d", speed);
		CLEAR_LCD();
		OUT_LCD(sAdcValue,1,0);		//line 1, vi tri dau tien
		sprintf(sAdcValue, "Position: %.2lf", position);
		OUT_LCD(sAdcValue,2,0);		//line 2, vi tri dau tien
		Delay_Ms(1000);
	}
	else{
		CLEAR_LCD();
		sprintf(sAdcValue, "%.2lf %.2lf %.2lf", VAR,Humidity,Temperature);
		OUT_LCD(sAdcValue,1,0);		//line 1, vi tri dau tien
		sprintf(sAdcValue, "%d %d", joystickY,joystickX);
		OUT_LCD(sAdcValue,2,0);		//line 2, vi tri dau tien
		Delay_Ms(1000);
	}
	if(hc05send>=5){
		sendhc05();
		hc05send=0;
	}
//		CLEAR_LCD();
//		OUT_LCD(nhapchu,1,0);		//line 1, vi tri dau tien
//		uint32_t index=0;
//		while(receive_usart[index]==0){
//			index++;
//			if(index==BufferSize)	index=0;
//		}
//		if(Buffer_compare(password,receive_usart,BufferSize)){	
//			CLEAR_LCD();
//			OUT_LCD(correct,1,0);		//line 1, vi tri dau tien
//			OUT_LCD(receive_usart,2,0);		//line 2, vi tri dau tien
//			GPIO_SetBits(GPIOD,GPIO_Pin_13);
//			Delay_Ms(1000);
//		}
//		else{
//			CLEAR_LCD();
//			OUT_LCD(incorrect,1,0);		//line 1, vi tri dau tien
//			OUT_LCD(receive_usart,2,0);		//line 2, vi tri dau tien
//			GPIO_ResetBits(GPIOD,GPIO_Pin_13);
//			Delay_Ms(1000);
//		}
		MPU6050_Read_Accel(I2C2);
		MPU6050_Read_GYRO(I2C2);
		I2C_Receive(I2C1,RTC_ADDRESS,SEC_REG,BCDdata,7);	//read RTC value
		for(int i=0;i<7;i++){
			DECdata[i]=BCD_DEC(BCDdata[i]);
		}
	}
}