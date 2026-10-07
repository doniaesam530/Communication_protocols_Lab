/*
 * SPI.c  -  SPI driver (ATmega32)
 */
#include "inc/SPI.h"

void SPI_Init(void) {
#ifdef MASTER_MODE
	/* GPIO pins: MOSI, SCK, SS = output ; MISO = input */
	DIO_initPin(MOSI, SPI_PORT, OUTPUT);
	DIO_initPin(SCLK, SPI_PORT, OUTPUT);
	DIO_initPin(SS,   SPI_PORT, OUTPUT);
	DIO_initPin(MISO, SPI_PORT, INPUT);
	DIO_WritePin(SS, SPI_PORT, HIGH);          /* slave not selected */

	/* Master configuration: enable SPI, master, shift clock = Fosc/16 */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
#endif

#ifdef SLAVE_MODE
	/* GPIO pins: MISO = output ; MOSI, SCK, SS = input */
	DIO_initPin(MISO, SPI_PORT, OUTPUT);
	DIO_initPin(MOSI, SPI_PORT, INPUT);
	DIO_initPin(SCLK, SPI_PORT, INPUT);
	DIO_initPin(SS,   SPI_PORT, INPUT);

	/* Slave configuration: just enable SPI (MSTR = 0) */
	SPCR = (1 << SPE);
#endif
}

/* Master: starts a transfer.  Slave: preloads reply and waits for the master. */
uint8 SPI_SendRecieveData(uint8 Data) {
	SPDR = Data;
	while (!GET(SPSR, SPIF))
		;
	return SPDR;       /* reading SPSR then SPDR clears SPIF */
}

#ifdef MASTER_MODE
void SPI_SlaveSelect(void)   { DIO_WritePin(SS, SPI_PORT, LOW);  }
void SPI_SlaveDeselect(void) { DIO_WritePin(SS, SPI_PORT, HIGH); }
#endif
