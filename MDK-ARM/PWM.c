#include "PWM.h"

#define PULSES_PER_REVOLUTION 334 // Number of pulses per revolution
#define TICKS_PER_MICROSECOND (SystemCoreClock / 1000000)
extern uint16_t ADC_Value[5];		//dung 5 channel cua ADC

volatile int32_t encoderValue=0;
volatile int32_t rotaroty=0;
volatile float position=0;
volatile uint8_t MotorDir=0;
volatile uint32_t lastCaptureValue = 0;
volatile float angularVelocity = 0;
volatile int32_t speed;
void GPIO_TIM_init(void)
{
	GPIO_InitTypeDef GPIO_TIM;
	/* GPIOE Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
	/* Configure PORTE in output pushpull mode */
  GPIO_TIM.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;
  GPIO_TIM.GPIO_Mode = GPIO_Mode_AF;
  GPIO_TIM.GPIO_OType = GPIO_OType_PP;
  GPIO_TIM.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_TIM.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_TIM);	
	
	/* Configure PORTE in output pushpull mode */
  GPIO_TIM.GPIO_Pin = GPIO_Pin_12;
  GPIO_TIM.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_TIM.GPIO_OType = GPIO_OType_PP;
  GPIO_TIM.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_TIM.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_TIM);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_12);
}
void TIMER_TIM_init(void)	
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct; 
	/* Tim1 Peripheral clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//APB2:168MHz
	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 16800-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
}
void PWM_TIM_init(void)
{
	//Tan so 1kHz
	TIM_OCInitTypeDef PWM;
	PWM.TIM_OCMode = TIM_OCMode_PWM1;
	PWM.TIM_OutputState = TIM_OutputState_Enable;
	PWM.TIM_OCPolarity = TIM_OCPolarity_High;
	/*Pulse_length=((TIM_Period+1)*DutyCycle)/100-1*/
	
	//TIM1_Channel1
	PWM.TIM_Pulse = 0;
	TIM_OC1Init(TIM1,&PWM);
	TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Enable);
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	//TIM1_Channel2
	PWM.TIM_Pulse = 16000;
	TIM_OC2Init(TIM1,&PWM);
	TIM_CCxCmd(TIM1,TIM_Channel_2,TIM_CCx_Enable);
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_Cmd(TIM1,ENABLE); 
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
uint32_t speed_pwm(void)
{
	
	TIM1->CCR2= 5*ADC_Value[3];
	if(TIM1->CCR2 > 16799)	TIM1->CCR2=16799;
	return TIM1->CCR2;
	
}
//void ENCODER_init(void)		//pc6,pc7: TIMER3 CH1,CH2
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	TIM_ICInitTypeDef  TIM_ICInitStructure;	//TIM Input Capture Init structure definition
//	/* Tim3 Peripheral clock enable */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//	
//	GPIO_InitTypeDef GPIO_ENCODER;
//	/* GPIOC Peripheral clock enable */
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//	/*Configure GPIO for Encoder*/
//	GPIO_ENCODER.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_ENCODER.GPIO_OType = GPIO_OType_PP;
//	GPIO_ENCODER.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_ENCODER.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_ENCODER.GPIO_Speed = GPIO_Speed_100MHz;
//	// Connect GPIO pins to Timer 3
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
//	
//	/*Configure Timer for Encoder */
//	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF;
//	TIM_TimeBaseInitStruct.TIM_Prescaler = 83;
//	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
//	
//	// Configure specific settings directly
//	 TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
//	 TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	 TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	 TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	 TIM_ICInitStructure.TIM_ICFilter = 0x0;
//	
//	 TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
//	 TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	 TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	 TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	 TIM_ICInitStructure.TIM_ICFilter = 0x0;
//	 // Configure input capture channel for TIM3
//	 TIM_ICInit(TIM3, &TIM_ICInitStructure);
//	 
//	 // Configure Encoder interface
//	 TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
//	 
//	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
//	TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);

//	// Configure the encoder interrupt
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	// Start Timer 3
//	TIM_Cmd(TIM3, ENABLE);
//}
//void TIM3_IRQHandler(void) {
//	uint32_t encoderValue1,encoderValue2;
//  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
//        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
//				encoderCount++;	
//			uint32_t currentEncoderValue = TIM_GetCapture1(TIM3);
//        timeElapsed = (currentEncoderValue - lastEncoderValue) * TICKS_PER_MICROSECOND;
//        lastEncoderValue = currentEncoderValue;
//			 motorSpeed = (float)(encoderCount * 60000000) / (PULSES_PER_REVOLUTION * timeElapsed);
//			}
//	GPIO_SetBits(GPIOD,GPIO_Pin_13);
////    if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET) {
////        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);  
////     }
//}
void TIM3_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;  
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOC clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //Change here, to GPIOA 

  /* TIM3 channel1 configuration : PC6 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;  //XUNG A
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);   

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;  //XUNG B, DOC DU LIEU DE BIET DIR
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
  /* Connect TIM pin to AF2 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3); //And change to pin 1
//  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 
	TIM_ICInit(TIM3,&TIM_ICInitStructure);

  TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);

  /* Select the TIM3 Input Trigger: TI1FP1 */
  TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);

  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);

  /* TIM enable counter */
  TIM_Cmd(TIM3, ENABLE);

  /* Enable the CC1 Interrupt Request */
  TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
}
void TIM4_SampleTime(void)		//sample time=10ms
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	/* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	/*Configure Timer for Encoder */
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 10000-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 167;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	// Configure the encoder interrupt
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Start Timer 4
	TIM_Cmd(TIM4, ENABLE);
}
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		encoderValue++;
		if(encoderValue>334){	
			encoderValue=0;
			rotaroty++;
		}
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7))	MotorDir=0;
		else MotorDir=1;
		
//		uint32_t currentCaptureValue = TIM_GetCapture1(TIM3);
//		uint32_t capturePeriod = currentCaptureValue - lastCaptureValue;
//		angularVelocity = (2 * 3.14159f) / (capturePeriod * PULSES_PER_REVOLUTION);	//toc do goc
//		lastCaptureValue = currentCaptureValue;

		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
  }
}
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) {
		speed = (rotaroty + (1.0*encoderValue/334))*6000.0;		//RPM
		position = ((float)rotaroty + encoderValue/334.0)*360;
		rotaroty=0;
		encoderValue=0;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
//void DMA2_PWM_Init(void){
//	DMA_InitTypeDef DMA_InitStructure;
//	/* DMA1 Peripheral clock enable */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//	
//	/* Configure DMA controller to manage USART TX and RX DMA request ----------*/ 
//  
//	/* Configure DMA Initialization Structure */
//	DMA_InitStructure.DMA_BufferSize = 1 ;
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(TIM1->CCR1)) ;
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//	DMA_InitStructure.DMA_Channel = DMA_Channel_6 ;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
//	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)&ADC_Value[3] ; 
//	DMA_Init(DMA2_Stream2,&DMA_InitStructure);
//	
//	DMA_Cmd(DMA2_Stream2,ENABLE);
//}