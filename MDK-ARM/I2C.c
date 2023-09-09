#include "I2C.h"
#define FAST_I2C_MODE 

uint8_t Accel_data[6];
uint8_t Gyro_data[6];

uint8_t Clock_data[7]={0x00,0x38,0x09,0x05,0x24,0x08,0x23};
uint8_t MPU_data[1];

int16_t Accel_X_RAW=0;
int16_t Accel_Y_RAW=0;
int16_t Accel_Z_RAW=0;
int16_t Gyro_X_RAW=0;
int16_t Gyro_Y_RAW=0;
int16_t Gyro_Z_RAW=0;

double Accel_X=0;
double Accel_Y=0;
double Accel_Z=0;

double Gyro_X=0;
double Gyro_Y=0;
double Gyro_Z=0;
//int data1;
void I2C_init(I2C_TypeDef *I2Cx)
{
	/**** STEPS FOLLOWED  ************
	1. Enable the I2C CLOCK and GPIO CLOCK
	2. Configure the I2C PINs for ALternate Functions
		a) Select Alternate Function in MODER Register
		b) Select Open Drain Output 
		c) Select High SPEED for the PINs
		d) Select Pull-up for both the Pins
		e) Configure the Alternate Function in AFR Register
	3. Reset the I2C 	
	4. Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
	5. Configure the clock control registers
	6. Configure the rise time register
	7. Program the I2C_CR1 register to enable the peripheral
	*/
	if(I2Cx==I2C1){
		// Enable the I2C1 peripheral clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    // Enable the GPIOB peripheral clock (change GPIOB to your desired GPIO port)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		
		GPIO_InitTypeDef GPIO_InitStructure;
		I2C_InitTypeDef I2C_InitStructure;
		
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7|GPIO_Pin_8;  //PB7:SDA1,PB8:SCL1
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
		
		//Cau hinh AF
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_I2C1); 
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_I2C1);
		
		I2C_DeInit(I2C1);
		
		I2C_InitStructure.I2C_ClockSpeed = 100000;
		I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStructure.I2C_OwnAddress1 = 0x01; // This is the device's own address
		I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_Init(I2C1, &I2C_InitStructure);
		
		I2C_Cmd(I2C1,ENABLE); 
	}
	else if(I2Cx==I2C2)
	{
		// Enable the I2C1 peripheral clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

    // Enable the GPIOB peripheral clock (change GPIOB to your desired GPIO port)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		
		GPIO_InitTypeDef GPIO_InitStructure;
		I2C_InitTypeDef I2C_InitStructure;
		
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_11;  //PB7:SDA1,PB8:SCL1
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
		
		//Cau hinh AF
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_I2C2); 
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_I2C2);
		
		I2C_DeInit(I2C2);
		
		I2C_InitStructure.I2C_ClockSpeed = 100000;
		I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStructure.I2C_OwnAddress1 = 0x01; // This is the device's own address
		I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_Init(I2C2, &I2C_InitStructure);
		
		I2C_Cmd(I2C2,ENABLE);
	}
}

void I2C_Start (I2C_TypeDef *I2Cx)
{
	/**** STEPS FOLLOWED  ************
	1. Send the START condition 
	2. Wait for the SB ( Bit 0 in SR1) to set. This indicates that the start condition is generated
	*/
	I2C_GenerateSTART(I2Cx,ENABLE);
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT));		//check EV5
}
void I2C_Write (I2C_TypeDef *I2Cx,uint8_t data)
{
	/**** STEPS FOLLOWED  ************
	1. Wait for the TXE (bit 7 in SR1) to set. This indicates that the DR is empty
	2. Send the DATA to the DR Register
	3. Wait for the BTF (bit 2 in SR1) to set. This indicates the end of LAST DATA transmission
	*/	
	while (!(I2Cx->SR1 & (1<<7)));  // wait for TXE bit to set
	I2Cx->DR = data;
	while (!(I2Cx->SR1 & (1<<2)));  // wait for BTF bit to set
}
void I2C_Address (I2C_TypeDef *I2Cx,uint8_t Address)
{
	/**** STEPS FOLLOWED  ************
	1. Send the Slave Address to the DR Register
	2. Wait for the ADDR (bit 1 in SR1) to set. This indicates the end of address transmission
	3. clear the ADDR by reading the SR1 and SR2
	*/	
	I2C_Send7bitAddress(I2Cx,Address<<1,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));		//check EV6
}
void I2C_Stop (I2C_TypeDef *I2Cx)
{
	I2C_GenerateSTOP(I2Cx,ENABLE);
//	I2Cx->CR1 |= (1<<9);  // Generate STOP
}

void I2C_WriteMulti (I2C_TypeDef *I2Cx,uint8_t *data, uint8_t size)
{
	/**** STEPS FOLLOWED  ************
	1. Wait for the TXE (bit 7 in SR1) to set. This indicates that the DR is empty
	2. Keep Sending DATA to the DR Register after performing the check if the TXE bit is set
	3. Once the DATA transfer is complete, Wait for the BTF (bit 2 in SR1) to set. This indicates the end of LAST DATA transmission
	*/	
	while(size--)
	{
		while (!(I2Cx->SR1 & (1<<7)));  // wait for TXE bit to set
		I2Cx->DR = *data++;
	}
	while (!(I2Cx->SR1 & (1<<2)));  // wait for BTF bit to set
}

void I2C_Read (I2C_TypeDef *I2Cx,uint8_t Address, uint8_t *buffer, int8_t size)
{
	/**** STEPS FOLLOWED  ************
	1. If only 1 BYTE needs to be Read
		a) Write the slave Address, and wait for the ADDR bit (bit 1 in SR1) to be set
		b) the Acknowledge disable is made during EV6 (before ADDR flag is cleared) and the STOP condition generation is made after EV6
		c) Wait for the RXNE (Receive Buffer not Empty) bit to set
		d) Read the data from the DR

	2. If Multiple BYTES needs to be read
		a) Write the slave Address, and wait for the ADDR bit (bit 1 in SR1) to be set
		b) Clear the ADDR bit by reading the SR1 and SR2 Registers
		c) Wait for the RXNE (Receive buffer not empty) bit to set
		d) Read the data from the DR 
		e) Generate the Acknowlegment by setting the ACK (bit 10 in SR1)
		f) To generate the nonacknowledge pulse after the last received data byte, the ACK bit must be cleared just after reading the 
			 second last data byte (after second last RxNE event)
		g) In order to generate the Stop/Restart condition, software must set the STOP/START bit 
			 after reading the second last data byte (after the second last RxNE event)
	*/	
	int8_t remaining=size;

		if (size == 1)
	{
		I2Cx->DR = Address<<1|0x01;  //  send the address
		while (!(I2Cx->SR1 & (1<<1)));  // wait for ADDR bit to set
			
		I2Cx->CR1 &= ~(1<<10);  // clear the ACK bit 
		uint8_t temp = I2Cx->SR1 | I2Cx->SR2;  // read SR1 and SR2 to clear the ADDR 
		I2Cx->CR1 |= (1<<9);  // Stop I2C

		while (!(I2Cx->SR1 & (1<<6)));  // wait for RxNE to set
		
		/**** STEP 1-d ****/	
		buffer[size-remaining] = I2Cx->DR;  // Read the data from the DATA REGISTER
		
	}
	
	else 
	{
		I2Cx->DR = Address<<1|0x01;  //  send the address
		while (!(I2Cx->SR1 & (1<<1)));  // wait for ADDR bit to set

		uint8_t temp = I2Cx->SR1 | I2Cx->SR2;  // read SR1 and SR2 to clear the ADDR bit
		
		while (remaining-->2)
		{
			while (!(I2Cx->SR1 & (1<<6)));  // wait for RxNE to set
			
			/**** STEP 2-d ****/
			buffer[size-remaining] = I2Cx->DR;  // copy the data into the buffer			
			
			/**** STEP 2-e ****/
			I2Cx->CR1 |= 1<<10;  // Set the ACK bit to Acknowledge the data received
		}
		
		// Read the SECOND LAST BYTE
		while (!(I2Cx->SR1 & (1<<6)));  // wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;
		
		I2Cx->CR1 &= ~(1<<10);  // clear the ACK bit 
		I2Cx->CR1 |= (1<<9);  // Stop I2C	
		remaining--;
		
		// Read the Last BYTE
		while (!(I2Cx->SR1 & (1<<6)));  // wait for RxNE to set
		buffer[size-remaining] = I2Cx->DR;  // copy the data into the buffer
	}
}
void MPU_Write(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t Reg, uint8_t Data)
{
	I2C_Start(I2Cx);
	I2C_Address(I2Cx,Address);
	I2C_Write(I2Cx,Reg);
	I2C_Write(I2Cx,Data);
	I2C_Stop(I2Cx);
}
void MPU_Read(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t Reg, uint8_t *buffer,uint8_t size)
{
	I2C_Start(I2Cx);
	I2C_Address(I2Cx,Address);
	I2C_Write(I2Cx,Reg);
	I2C_Start(I2Cx);
	I2C_Read(I2Cx,Address,buffer,size);
	I2C_Stop(I2Cx);
}
void MPU6050_Init(I2C_TypeDef *I2Cx)
{
	MPU_data[0]=0;
	I2C_Send(I2Cx, MPU6050_ADDRESS, PWR_MGMT_1_REG, MPU_data, 1);		//wake up sensor
	MPU_data[0]=0x07;
	I2C_Send(I2Cx, MPU6050_ADDRESS, SMPLRT_DIV_REG, MPU_data, 1);		//Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
	//AFS_SEL=0->Full Scale Range= ± 2g
	//XA_ST=0,YA_ST=0,ZA_ST=0
	MPU_data[0]=0x00;
	I2C_Send(I2Cx, MPU6050_ADDRESS, ACCEL_CONFIG_REG, MPU_data, 1);	
	//FS_SEL=0->Full Scale Range= ± 250 °/s
	//XG_ST=0,YG_ST=0,ZG_ST=0
	MPU_data[0]=0x00;
	I2C_Send(I2Cx, MPU6050_ADDRESS, GYRO_CONFIG_REG, MPU_data, 1);

}
void MPU6050_Read_Accel(I2C_TypeDef *I2Cx)
{

	I2C_Receive(I2Cx, MPU6050_ADDRESS, ACCEL_XOUT_H_REG,Accel_data,6);
	/*
	Rx_data[0]=ACCEL_XOUT_H
	Rx_data[1]=ACCEL_XOUT_L
	Rx_data[2]=ACCEL_YOUT_H
	Rx_data[3]=ACCEL_YOUT_L
	Rx_data[4]=ACCEL_ZOUT_H
	Rx_data[5]=ACCEL_ZOUT_L
	*/
	Accel_X_RAW = (int16_t)(Accel_data[0]<<8|Accel_data[1]);
	Accel_Y_RAW = (int16_t)(Accel_data[2]<<8|Accel_data[3]);
	Accel_Z_RAW = (int16_t)(Accel_data[4]<<8|Accel_data[5]);
	
	Accel_X=Accel_X_RAW/16384.0;
	Accel_Y=Accel_Y_RAW/16384.0;
	Accel_Z=Accel_Z_RAW/16384.0;
}
void MPU6050_Read_GYRO(I2C_TypeDef *I2Cx)
{
	I2C_Receive(I2Cx, MPU6050_ADDRESS, GYRO_XOUT_H_REG,Gyro_data,6);
	/*
	Rx_data[0]=GYRO_XOUT_H
	Rx_data[1]=GYRO_XOUT_L
	Rx_data[2]=GYRO_XOUT_H
	Rx_data[3]=GYRO_XOUT_L
	Rx_data[4]=GYRO_XOUT_H
	Rx_data[5]=GYRO_XOUT_L
	*/
	Gyro_X_RAW = (int16_t)(Gyro_data[0]<<8|Gyro_data[1]);
	Gyro_Y_RAW = (int16_t)(Gyro_data[2]<<8|Gyro_data[3]);
	Gyro_Z_RAW = (int16_t)(Gyro_data[4]<<8|Gyro_data[5]);
	
	Gyro_X=Gyro_Y_RAW/131.0;
	Gyro_Y=Gyro_Y_RAW/131.0;
	Gyro_Z=Gyro_Y_RAW/131.0;
}

void I2C_Send(I2C_TypeDef *I2Cx,uint8_t deviceAddr, uint8_t regAddr, uint8_t *data, uint8_t size)
{
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);		// send start
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));		// check EV5
	I2C_Send7bitAddress(I2Cx, deviceAddr<<1, I2C_Direction_Transmitter);		//send address, address<<1
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));		//check EV6
	I2C_Cmd(I2Cx,ENABLE);
	
	I2C_SendData(I2Cx, regAddr);				//send the slave internal address to transmitter
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));		//check EV8
	
	while(size--)
	{
		I2C_SendData(I2Cx, *data++);
		regAddr++;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));		//check EV8
	}
	
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

void I2C_Receive(I2C_TypeDef *I2Cx,uint8_t deviceAddr, uint8_t regAddr,uint8_t *diachi,uint8_t len) 
{
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)){}
  I2C_GenerateSTART(I2Cx, ENABLE);
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(I2Cx, (deviceAddr<<1),I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2Cx, regAddr);
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTART(I2Cx, ENABLE);
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(I2Cx, (deviceAddr<<1), I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	I2C_AcknowledgeConfig(I2Cx,ENABLE);
//	I2C_Cmd(I2C1,ENABLE);
	while(len)
	{
//	I2C_Cmd(I2C1,ENABLE);
		if(len==1)
		{	
			I2C_AcknowledgeConfig(I2Cx,DISABLE);
			I2C_GenerateSTOP(I2Cx, ENABLE);
		}
		while(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)));
		*diachi=I2C_ReceiveData(I2Cx);
		len--;
		diachi++;
		I2C_AcknowledgeConfig(I2Cx,ENABLE);
	}
}
void RTC_init(void)
{
	I2C_Send(I2C1,RTC_ADDRESS,SEC_REG,Clock_data,7);
}