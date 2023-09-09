#include "ADC.h"
uint16_t ADC_Value[5];		//dung 5 channel cua ADC
void ADC1_init(void){
	//Init GPIO,ADC,DMA
	DMA_InitTypeDef DMA_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC1_Init;
	ADC_CommonInitTypeDef ADC_Common;
	
	/* GPIOC,ADC1,DMA Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	/*Configure DMA2_Channel_0/Stream0(support ADC1)*/
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_Value;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = NUMBER_OF_CHANNEL;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	
	/* Configure PORTC in output pushpull mode */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStruct);	
	
	/*ADC_Common*/
	ADC_Common.ADC_Mode = ADC_Mode_Independent;
	ADC_Common.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_Common.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_Common.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	
	ADC_CommonInit(&ADC_Common);
	/*ADC_Init*/
	ADC1_Init.ADC_Resolution = ADC_Resolution_12b;
	ADC1_Init.ADC_ScanConvMode = ENABLE;
	ADC1_Init.ADC_DataAlign = ADC_DataAlign_Right;
	ADC1_Init.ADC_ContinuousConvMode = ENABLE;
	ADC1_Init.ADC_NbrOfConversion = 5;
	ADC1_Init.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	
	ADC_Init(ADC1,&ADC1_Init);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,3,ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13,4,ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_14,5,ADC_SampleTime_15Cycles);
	
	DMA_ITConfig (DMA2_Stream0,DMA_IT_TC,ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the DMA1_Stream6 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
	ADC_DMACmd (ADC1,ENABLE);

	ADC_Cmd(ADC1,ENABLE);	
	// Start ADC conversion
	ADC_SoftwareStartConv(ADC1);
}
void choose_channel(uint8_t channel)
{
	ADC_RegularChannelConfig(ADC1,channel,1,ADC_SampleTime_15Cycles);
	// Start ADC conversion
	ADC_SoftwareStartConv(ADC1);
}
uint16_t ADC1_Read(void)
{
    // Wait until conversion is finish
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

    return ADC_GetConversionValue(ADC1);
}
void DMA2_Stream0_IRQHandler(void){
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0)){
		TIM1->CCR2= 5*ADC_Value[3];
		if(TIM1->CCR2 > 16799)	TIM1->CCR2=16799;
	}
	DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
}