/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED7SEG_H
#define __LED7SEG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "USART.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define sda_port GPIOD
#define sck_port GPIOD
#define lat_port GPIOC

#define sda_pin GPIO_Pin_10
#define sck_pin GPIO_Pin_11
#define lat_pin GPIO_Pin_8
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void led7seg_init(void);
void hc595_shift(uint8_t data);
void display_7seg(uint32_t data);
void TIM7_SampleTime(void);
#endif /* __LED7SEG_H */
