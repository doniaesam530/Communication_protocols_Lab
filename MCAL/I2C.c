/*
 * I2C.c  -  I2C (TWI) driver (ATmega32)
 */
#include "inc/I2C.h"

#define TW_STATUS   (TWSR & 0xF8)       /* mask the prescaler bits */

/* Master Functions */
void I2C_init(void) {
	/* SCL = F_CPU / (16 + 2*TWBR*prescaler)
	 * NB: with F_CPU = 1 MHz and 100 kHz the result is negative -> use 8 MHz or 50 kHz */
	switch (prescalar) {
	case 1:  TWSR = 0; break;
	case 4:  TWSR = 1; break;
	case 16: TWSR = 2; break;
	case 64: TWSR = 3; break;
	}
	TWBR = (uint8) (((F_CPU / SCL_Clock) - 16) / (2 * prescalar));
	TWCR = (1 << TWEN);
}

void I2C_Start(void) {
	/* START bit + enable + clear interrupt flag */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	/* TWINT is set after START is sent (status 0x08, or 0x10 for repeated START) */
	while (!GET(TWCR, TWINT))
		;
}

void I2C_Write(uint8 data) {
	TWDR = data;
	/* clear TWINT (write 1) to start the transfer */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/* TWINT set: data sent and ACK/NACK received */
	while (!GET(TWCR, TWINT))
		;
}

uint8 I2C_Read(uint8 ack) {
	if (ack)
		TWCR = (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
	else
		TWCR = (1 << TWINT) | (1 << TWEN);
	while (!GET(TWCR, TWINT))
		;
	return TWDR;
}

void I2C_Stop(void) {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while (GET(TWCR, TWSTO))        /* hardware clears TWSTO when STOP is done */
		;
}

uint8 I2C_SendSlaveAddress(uint8 slaveAddress, uint8 Direction) {
	I2C_Write(slaveAddress + Direction);
	/* 0x18: SLA+W sent, ACK received      0x40: SLA+R sent, ACK received */
	if ((Direction == I2C_WRITE && TW_STATUS == 0x18)
			|| (Direction == I2C_READ && TW_STATUS == 0x40))
		return 1;

	I2C_Stop();                     /* no ACK: do not hang forever */
	return 0;
}

/* Slave Functions */
void I2C_setAddress(uint8 address) {
	TWAR = address;
}

uint8 Slave_Read(uint8 ack) {
	/* wait for own SLA+W (status 0x60) */
	TWCR = (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
	while (!GET(TWCR, TWINT))
		;
	while (TW_STATUS != 0x60)
		;
	/* receive one data byte: 0x80 = data received, ACK returned */
	if (ack)
		TWCR = (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
	else
		TWCR = (1 << TWINT) | (1 << TWEN);
	while (!GET(TWCR, TWINT))
		;
	return TWDR;
}
