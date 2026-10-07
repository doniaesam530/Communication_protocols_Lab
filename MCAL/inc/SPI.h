/*
 * SPI.h  -  SPI driver (ATmega32)
 * Select the role at build time with ONE of these symbols
 * (Makefile passes them with -D, or Project > Toolchain > Symbols):
 *      MASTER_MODE   -> Node A
 *      SLAVE_MODE    -> Node B
 */
#ifndef ATMEGA32_INC_SPI_H_
#define ATMEGA32_INC_SPI_H_

#include "DIO.h"
#include "Atmega32.h"
#include "../../Library/BIT_MATH.h"

#if defined(MASTER_MODE) && defined(SLAVE_MODE)
#error "Define only one of MASTER_MODE / SLAVE_MODE"
#elif !defined(MASTER_MODE) && !defined(SLAVE_MODE)
#error "Define MASTER_MODE or SLAVE_MODE (-D flag or Toolchain > Symbols)"
#endif

#define SPI_PORT   PORT_B
#define MOSI       PIN_5
#define MISO       PIN_6
#define SCLK       PIN_7
#define SS         PIN_4

void  SPI_Init(void);
uint8 SPI_SendRecieveData(uint8 Data);

#ifdef MASTER_MODE
void  SPI_SlaveSelect(void);     /* SS = LOW  */
void  SPI_SlaveDeselect(void);   /* SS = HIGH */
#endif

#endif /* ATMEGA32_INC_SPI_H_ */
