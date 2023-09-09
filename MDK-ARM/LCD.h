/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "gpio.h"
#include "TIMER.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void init_lcd();
void OUT_LCD(char line[],uint8_t x,uint8_t y);
void xulyhex(uint8_t data);
void cmdwrite(uint8_t data);
void datawrite(uint8_t data);
void CLEAR_LCD(void);
#endif /* __INIT_H */
