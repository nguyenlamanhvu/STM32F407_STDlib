/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#ifdef FAST_I2C_MODE 
#define I2C_SPEED 400000 
#define I2C_DUTYCYCLE I2C_DutyCycle_16_9 
#else /* STANDARD_I2C_MODE*/ 
#define I2C_SPEED 100000 
#define I2C_DUTYCYCLE I2C_DutyCycle_2 
#endif /* FAST_I2C_MODE*/ 

#define MPU6050_ADDRESS 0x68 // MPU-6050 I2C address
#define RTC_ADDRESS 0x68
#define SEC_REG 0
#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define SMPLRT_DIV_REG 0x19
#define ACCEL_CONFIG_REG 0x1C
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_XOUT_H_REG 0x3B
#define GYRO_XOUT_H_REG 0x43
/* Exported functions ------------------------------------------------------- */
void I2C_init(I2C_TypeDef *I2Cx);
void I2C_Start (I2C_TypeDef *I2Cx);
void I2C_Write (I2C_TypeDef *I2Cx,uint8_t data);
void I2C_Address (I2C_TypeDef *I2Cx,uint8_t Address);
void I2C_Stop (I2C_TypeDef *I2Cx);
void I2C_WriteMulti (I2C_TypeDef *I2Cx,uint8_t *data, uint8_t size);
void I2C_Read (I2C_TypeDef *I2Cx,uint8_t Address, uint8_t *buffer, int8_t size);
void I2C_Send(I2C_TypeDef *I2Cx,uint8_t deviceAddr, uint8_t regAddr, uint8_t *data, uint8_t size);
void I2C_Receive(I2C_TypeDef* I2Cx,uint8_t deviceAddr, uint8_t regAddr,uint8_t *diachi,uint8_t len);
void RTC_init(void);

void MPU_Write(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t Reg, uint8_t Data);
void MPU_Read(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t Reg, uint8_t *buffer,uint8_t size);
void MPU6050_Init(I2C_TypeDef *I2Cx);
void MPU6050_Read_Accel(I2C_TypeDef *I2Cx);
void MPU6050_Read_GYRO(I2C_TypeDef *I2Cx);
#endif /* __I2C_H */