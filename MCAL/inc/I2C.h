/*
 * I2C.h  -  I2C (TWI) driver (ATmega32)
 */
#ifndef ATMEGA32_INC_I2C_H_
#define ATMEGA32_INC_I2C_H_

#include "DIO.h"
#include "Atmega32.h"
#include "../../Library/BIT_MATH.h"

#define MASTER_Mode
//#define SLAVE_Mode

#define prescalar     1          /* 1, 4, 16, 64 */
#define SCL_Clock     100000UL   /* 100 kHz (needs F_CPU >= 1.6 MHz) */

#define I2C_WRITE     0
#define I2C_READ      1

void  I2C_init(void);
void  I2C_Start(void);
void  I2C_Stop(void);
void  I2C_Write(uint8 data);
uint8 I2C_Read(uint8 ack);
/* slaveAddress = 8-bit write address (7-bit address << 1).
 * returns 1 if the slave ACKed, 0 if not (a STOP is sent in that case) */
uint8 I2C_SendSlaveAddress(uint8 slaveAddress, uint8 Direction);

/* Slave functions */
void  I2C_setAddress(uint8 address);
uint8 Slave_Read(uint8 ack);

#endif /* ATMEGA32_INC_I2C_H_ */
