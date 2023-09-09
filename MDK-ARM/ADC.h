/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "TIMER.h"
#include "gpio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define ADC1_DR_ADDRESS ((uint32_t) 0x4001204C)
#define NUMBER_OF_CHANNEL 5U

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ADC1_init(void);
void choose_channel(uint8_t channel);
uint16_t ADC1_Read(void);
#endif /* __ADC_H */
