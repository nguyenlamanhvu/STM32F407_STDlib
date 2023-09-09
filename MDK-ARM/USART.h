/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "string.h"
#include "gpio.h"
#include "TIMER.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define BufferSize 1
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USART2_Init(void);
void send_data(uint8_t data);
uint8_t receive_data(void);
void send_char(char *data);
uint8_t Buffer_compare(char* string1,char* string2, uint32_t length );
void reset_buffer(char receive_usart[]);
void DMA1_USART2_init(void);
__weak void USART_receivedata(void);
void sendhc05(void);
#endif /* __USART_H */
