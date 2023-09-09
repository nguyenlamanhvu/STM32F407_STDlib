/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LEDMATRIX_H
#define __LEDMATRIX_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define sda_port GPIOB
#define sck_port GPIOB
#define lat_port GPIOB

#define sda_pin GPIO_Pin_15
#define sck_pin GPIO_Pin_13
#define lat_pin GPIO_Pin_2
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ledmatrix_init(void);
void hc595_shift_matrix(uint8_t data);
void display_matrix(void);
void TIM5_SampleTime(void);
#endif /* __LEDMATRIX_H */
