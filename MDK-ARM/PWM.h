/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PWM_H
#define __PWM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void GPIO_TIM_init(void);
void TIMER_TIM_init(void);
void PWM_TIM_init(void);
uint32_t speed_pwm(void);
void ENCODER_init(void);
void TIM3_Config(void);
void TIM4_SampleTime(void);
#endif /* __PWM_H */