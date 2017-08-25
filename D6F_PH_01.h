/**
  ******************************************************************************
  * File Name          : 	D6F_PH_01.h
  * Description        : Main program body
  ******************************************************************************
  *
	* COPYRIGHT(c) 2017 Akash kapashia
  * Created by Akash kapashia
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#ifndef __DF6_PH_01_H
#define __DF6_PH_01_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l0xx_hal.h"

#define                     SA_8                0x6C  <<1// for 7bit Slave Address
 //#define RANGE_MODE 100 // Full Range +/-50[Pa]
 #define                     RANGE_MODE          250 // Full Range 0-250[Pa]
// #define RANGE_MODE 1000 // Full Range +/-500[Pa]
#define    I2Cxp             I2C2
/*=======================*/
/* for Measure Mode */
/*=======================*/
#define P 1 // Pressure mode
#define T 2 // Temperature mode
/* Function prototypes -------------------------------------------------------*/
void Initialize_dp( void );
int Press_meas( void );
short Temp_meas( void );
/* Private Functions --------------------------------------------------------*/
int I2C_WR(unsigned char add, uint8_t *dbuf, unsigned char n);
uint16_t I2C_RD_8(unsigned char add, uint8_t *dbuf, unsigned char n);
uint16_t I2C_RD_16(unsigned char add, uint8_t *dbuf, unsigned char n);
//unsigned short I2C_RD_u16(unsigned char add, char *dbuf, unsigned char n);
void I2Cxp_Init(void) ;
void I2C1_Start(void);
void I2C1_MastrSel(uint8_t address, uint8_t rw);
void I2C1_AckEn(void);
void I2C1_AckDis(void);
void I2C1_Stop(void);
void I2C1_senddata(uint8_t data);
uint8_t I2C1_rcvdata(void);
#ifdef __cplusplus
}
#endif

#endif