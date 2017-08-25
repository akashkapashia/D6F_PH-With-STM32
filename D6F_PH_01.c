/**
  ******************************************************************************
  * File Name          : 	D6F_PH_01.c
  * Description        : Main program body
  ******************************************************************************
  *
	* COPYRIGHT(c) 2017 Akash kapashia
  * Created by Akash kapashia
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "DF6_PH_01.h"
#define I2C1_SCL_PIN GPIO_Pin_6
#define I2C1_SDA_PIN GPIO_Pin_7
#define I2C2_SCL_PIN GPIO_Pin_10
#define I2C2_SDA_PIN GPIO_Pin_11
typedef unsigned char uint8; 
typedef unsigned short uint16;
typedef unsigned long uint32;
 uint16 RD_FIFO; /* 16bit data width */
unsigned short uRD_FIFO; /* 16bit data width */
 uint8_t RD_REG; /* 8bit data width */
 char setting_done_flag = 0;
// Dummy wait routine
I2C_HandleTypeDef hi2cxp;

void adc_wait( unsigned long delaay)
{
while(delaay) {
	    
	delaay--;
}
}
/*=================================================*/
/* Initialize Function */
/* Usage : Initialize( void ) */
/* Argument : Null */
/* Return value : T.B.D */
/*=================================================*/
void Initialize_dp( void )
{
 /* EEPROM Control <= 00h */
	char send1[] = {0x0B, 0x00};
	 while (HAL_I2C_GetState(&hi2cxp) != HAL_I2C_STATE_READY)
		{
		}  
		uint8_t m[1];
    m[0]=0;
	 HAL_I2C_Mem_Write(&hi2cxp,0xD8,0x0B, 1, m,1,100);
	// HAL_I2C_Mem_Write(&hi2cxp,0xD8,0x0B, 1, (uint8_t *)0x00,1,100);

//  I2C_WR(SA_8, send1, 2);
}
/*=======================================================*/
/* Pressure measure Function */
/* Usage : Press_meas( void ) */
/* Argument : NULL */
/* Return value : Compensated Pressure value(unsigned) */
/*=======================================================*/
int Press_meas(void)
{
 short rd_fifo;
 int rd_flow;
	RD_FIFO=0;
 unsigned long wait_time;
 /* [D040] <= 06h */
 uint8_t send2[] = {0x00, 0xD0, 0x40, 0x18, 0x06};
 I2C_WR(SA_8, send2, 5);
 wait_time = 33; /*33msec wait */
 /* wait time depend on resolution mode */
 adc_wait(wait_time);
 /* [D051/D052] => Read Compensated Flow value */
 uint8_t send3[] = {0x00, 0xD0, 0x51, 0x2C, 0x07};
  RD_FIFO = I2C_RD_16(SA_8, send3, 5);
 // Press Mode : [Pa] = (xx[count] - 1024) * Full Range [Pa]/ 60000 - Full Range [Pa] at other
if (RANGE_MODE == 250) {
 rd_flow = ((RD_FIFO - 1024) * RANGE_MODE *10/ 60000); /* convert to [Pa] */
}
else {
 rd_flow = ((RD_FIFO - 1024) * RANGE_MODE *10/ 60000) - (RANGE_MODE*10)/2; /* convertto [Pa] */
}
 return rd_flow;
}
/*=======================================================*/
/* Temperature measure Function */
/* Usage : Temp_meas() */
/* Argument : NULL */
/* Return value : x10 Temperature */
/*=======================================================*/
short Temp_meas(void)
{
 short rd_temp;
 unsigned long wait_time;
 /* [D040] <= 06h */
 char send2[] = {0x00, 0xD0, 0x40, 0x18, 0x06};
 I2C_WR(SA_8, send2, 5);
 /* wait time depend on resolution mode */
 wait_time = 33; /* 33msec wait */
 adc_wait(wait_time);
 /* [D061/D062] => Read TMP_H/TMP_L value */
 char send3[] = {0x00, 0xD0, 0x61, 0x2C, 0x07};
 RD_FIFO = I2C_RD_16 (SA_8, send3, 5);
 rd_temp = ((RD_FIFO -10214)*1000 / 3739); // convert to degree-C(x10)
 return rd_temp;
}
/* Public Basic Functions ----------------------------------------------------------*/
/*=================================================*/
/* I2C Write command */
/* Usage : I2C1_WR() */
/* Argument : 7bit Slave Address(char) */
/*
/* Return value : 8bit Read result */
/*=================================================*/
int I2C_WR(unsigned char add, uint8_t *dbuf, unsigned char n) {
 HAL_I2C_Master_Transmit(&hi2cxp,add, dbuf,n, 200);
 return 0;
}
 
/*=================================================*/
/* I2C Read command */
/* Usage : I2C_RD_16() */
/* Argument : char add (7bit Slave Address) */
/* char *dbuf (Write data) */
/* unsigned char n (Number of bytes)*/
/* Return value : 16bit Read result */
/*=================================================*/
uint16 I2C_RD_16 (unsigned char add, uint8_t *dbuf, unsigned char n) {
 int i= 0;
 char n_w;
 uint8_t rd_fifo[2] = {0, 0};
 n_w = n - 1;
 /* I2C Pre-WR Access */
while( HAL_I2C_IsDeviceReady(&hi2cxp,add,5,100));
 
 //while (n_w--) {
 HAL_I2C_Master_Transmit(&hi2cxp,add, dbuf,n_w, 100); /* Send Data */
 //i++;
 //}

 adc_wait(5); /* 5msec wait */
  while( HAL_I2C_IsDeviceReady(&hi2cxp,add,5,100));

 HAL_I2C_Master_Transmit(&hi2cxp,add,(uint8_t  *)&dbuf[n-1],1, 100); /* Send Data */
 add=0xD9;
 while( HAL_I2C_IsDeviceReady(&hi2cxp,add,5,100));

 HAL_I2C_Master_Receive(&hi2cxp,add, rd_fifo,2,100);
 
 RD_FIFO = ((rd_fifo[0] << 8) | rd_fifo[1]);
 return RD_FIFO;
}
 
void I2Cxp_Init() 
{

  hi2cxp.Instance = I2Cxp;
  hi2cxp.Init.Timing = 0x00000000;
  hi2cxp.Init.OwnAddress1 = 0;
  hi2cxp.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2cxp.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2cxp.Init.OwnAddress2 = 0;
  hi2cxp.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2cxp.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2cxp.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2cxp) != HAL_OK)
  {
   
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2cxp, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2cxp, 0) != HAL_OK)
  {
  
  }

}
 
 