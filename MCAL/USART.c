/*
 * USART.c  -  USART driver (ATmega32), polling
 */
#include "inc/USART.h"
#include <stdlib.h>

#define Default_Stop   '\r'

void USART_init(UART_Config_t *pinConfig) {
	uint16 ubrr;

	/* Baud rate:  UBRR = F_CPU / (divisor * baud) - 1   (rounded)
	 * unsigned long is needed: 16 * 9600 does not fit in 16 bits */
	unsigned long div = (unsigned long) pinConfig->DoubleSpeed * pinConfig->baudRate;
	ubrr = (uint16) (((F_CPU + div / 2UL) / div) - 1UL);

	if (pinConfig->DoubleSpeed == DOUBLE_Speed_EN)
		SET(UCSRA, U2X);
	else
		CLEAR(UCSRA, U2X);

	UBRRH = (uint8) (ubrr >> 8);   /* URSEL = 0 -> UBRRH */
	UBRRL = (uint8) ubrr;

	/* FRAME: URSEL=1 selects UCSRC.  Asynchronous, no parity, 1 stop bit, 8 data bits */
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
	CLEAR(UCSRB, UCSZ2);

	/* Enable transmitter and receiver */
	UCSRB |= (1 << TXEN) | (1 << RXEN);
}

void USART_send(uint8 data) {
	/* The transmit buffer can only be written when UDRE is set. */
	while (!GET(UCSRA, UDRE))
		;
	UDR = data;
}

uint8 USART_recieve(void) {
	/* RXC is set when there is unread data in the receive buffer */
	while (!GET(UCSRA, RXC))
		;
	return UDR;
}

void USART_sendNumber(uint32 data) {
	char str[8];
	sprintf(str, "%u", (unsigned int) data);
	USART_sendString((uint8 *) str);
}

uint32 USART_recieveNumber(void) {
	char buf[8];
	USART_recieveString((uint8 *) buf);
	return (uint32) atoi(buf);
}

void USART_sendString(uint8 *str) {
	uint8 i;
	for (i = 0; str[i] != '\0'; i++)      /* was: i < str[i]  (stopped early) */
		USART_send(str[i]);
	USART_send(Default_Stop);
}

void USART_recieveString(uint8 *Buff) {
	uint8 i = 0;
	Buff[i] = USART_recieve();
	while (Buff[i] != Default_Stop) {
		i++;
		Buff[i] = USART_recieve();
	}
	Buff[i] = '\0';
}
