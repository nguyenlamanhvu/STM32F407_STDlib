#include "USART.h"
char receive_usart[BufferSize];
char receive_buffer[50];
char trans_usart[50]={};
char hc05[50];
extern volatile int32_t speed;
extern volatile float position;
extern double Accel_X;
extern double Accel_Y;
extern double Accel_Z;
extern double Gyro_X;
extern double Gyro_Y;
extern double Gyro_Z;
extern double VAR;					//ADC_Channel_10
extern double Humidity;		//ADC_Channel_11
extern double Temperature;	//ADC_Channel_12
extern uint16_t joystickY;		//ADC_Channel_13
extern uint16_t joystickX;		//ADC_Channel_14
__weak void USART_RXNE(void)
	{
	};

void USART2_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* GPIOA,USART2 Peripheral clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	/* Configure PORTA in output pushpull mode */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStruct);	

//	RCC->AHB1ENR |= (1<<0);
//	
//	GPIOA->MODER &= ~((1<<4)|(1<<6));
//	GPIOA->MODER |= (1<<5)|(1<<7);
//	
//	GPIOA->PUPDR |= (1<<4);
//	GPIOA->OSPEEDR |= (3<<4);
//	GPIOA->AFR[0] &= ~(15<<8)|(15<<12);
//	GPIOA->AFR[0] |= (7<<8)|(7<<12);
	
	/* Configure USART2 Tx/Rx */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	/* Connect USART pins to AF7 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	USART_DMACmd(USART2,USART_DMAReq_Rx|USART_DMAReq_Tx,ENABLE);
	
	USART_ITConfig (USART2,USART_IT_RXNE, ENABLE);//|USART_IT_IDLE
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable USART */
	USART_Cmd(USART2, ENABLE);
}

void send_data(uint8_t data)
{
	while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE))
		USART_SendData(USART2,data);
	while(!USART_GetFlagStatus(USART2,USART_FLAG_TC));
}

void send_char(char *data)
{
	while(*data)	send_data(*data++);
}

uint8_t receive_data(void)
{
	while(!USART_GetFlagStatus(USART2,USART_FLAG_RXNE));
	return USART_ReceiveData(USART2);
}

uint8_t Buffer_compare(char* string1,char* string2, uint32_t length )
{
	while(length--)
	{
		if(*string1!=*string2)	return 0;
		string1++;
		string2++;
	}
	return 1;
}

void reset_buffer(char receive_usart[])
{
	for(uint32_t i=0;i<BufferSize;i++){
		receive_usart[i]=0;
	}
}

void DMA1_USART2_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	/* DMA1 Peripheral clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
	/* Configure DMA controller to manage USART TX and RX DMA request ----------*/ 
  
	/* Configure DMA Initialization Structure */
	DMA_InitStructure.DMA_BufferSize = BufferSize ;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(USART2->DR)) ;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/* Configure TX DMA */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)&trans_usart ;
	DMA_Init(DMA1_Stream6,&DMA_InitStructure);
	/* Configure RX DMA */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)&receive_usart ; 
	DMA_Init(DMA1_Stream5,&DMA_InitStructure);
	
	DMA_ITConfig (DMA1_Stream6,DMA_IT_TC,ENABLE);
	/* Enable DMA USART RX Stream */
	DMA_Cmd(DMA1_Stream5,ENABLE);
	/* Enable DMA USART TX Stream */
//	DMA_Cmd(DMA1_Stream6,ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the DMA1_Stream6 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void DMA1_Stream6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6)){
		GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
		Delay_Ms(1000);
	}
	DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus (USART2,USART_IT_RXNE)){		
		USART_RXNE();
	}
	USART_ClearITPendingBit (USART2,USART_IT_RXNE);
}
void sendhc05(void){
	sprintf(hc05, "Var:%.2lf Humidity:%.2lf Temp:%.2lf\nJoystickX:%d JoystickY:%d\n", VAR,Humidity,Temperature,joystickX,joystickY);
	for (int i = 0; hc05[i] != '\0'; i++) {
		while (!(USART2->SR & USART_SR_TXE)); // Wait until TXE flag is set
		USART2->DR = hc05[i]; // Send a character
	}
	sprintf(hc05, "Accel: X:%.2lf, Y:%.2lf, Z%.2lf\n", Accel_X,Accel_Y,Accel_Z);
	for (int i = 0; hc05[i] != '\0'; i++) {
		while (!(USART2->SR & USART_SR_TXE)); // Wait until TXE flag is set
		USART2->DR = hc05[i]; // Send a character
	}
	sprintf(hc05, "Gyro: X:%.2lf, Y:%.2lf, Z%.2lf\n", Gyro_X,Gyro_Y,Gyro_Z);
	for (int i = 0; hc05[i] != '\0'; i++) {
		while (!(USART2->SR & USART_SR_TXE)); // Wait until TXE flag is set
		USART2->DR = hc05[i]; // Send a character
	}
	sprintf(hc05, "Speed: %d, Position: %.2lf\n----------------------------\n", speed,position);
	for (int i = 0; hc05[i] != '\0'; i++) {
		while (!(USART2->SR & USART_SR_TXE)); // Wait until TXE flag is set
		USART2->DR = hc05[i]; // Send a character
	}
}
