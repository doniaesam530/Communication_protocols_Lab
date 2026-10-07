/*
 * USART.h  -  USART driver (ATmega32)
 */
#ifndef ATMEGA32_INC_USART_H_
#define ATMEGA32_INC_USART_H_

#include "Atmega32.h"
#include "../../Library/BIT_MATH.h"

typedef struct {
	unsigned long baudRate;   /* @ref USART_BaudRate_define (was uint8; uint32 is only 16-bit on AVR) */
	uint8  DoubleSpeed;   /* @ref Double_speed : divisor 16 (normal) or 8 (U2X) */
} UART_Config_t;

/* @ref USART_BaudRate_define */
#define USART_BaudRate_2400      2400UL
#define USART_BaudRate_4800      4800UL
#define USART_BaudRate_9600      9600UL
#define USART_BaudRate_14400     14400UL
#define USART_BaudRate_19200     19200UL
#define USART_BaudRate_28800     28800UL
#define USART_BaudRate_38400     38400UL
#define USART_BaudRate_57600     57600UL
#define USART_BaudRate_76800     76800UL
#define USART_BaudRate_115200    115200UL
#define USART_BaudRate_230400    230400UL
#define USART_BaudRate_250000    250000UL

/* @ref Double_speed  (the value is the baud-rate divisor) */
#define DOUBLE_Speed_EN          8
#define DOUBLE_Speed_DIS         16

void   USART_init(UART_Config_t *pinConfig);
void   USART_send(uint8 data);
uint8  USART_recieve(void);

void   USART_sendString(uint8 *str);
void   USART_sendNumber(uint32 data);

void   USART_recieveString(uint8 *Buff);
uint32 USART_recieveNumber(void);

/* Not implemented yet (later labs): */
uint8  USART_recievePeriodicData(uint8 *data);
void   USART_sendNoBlock(uint8 data);
uint8  USART_recieveNoBlock(void);
void   USART_TX_interrupt_Enable(void);
void   USART_TX_interrupt_Disable(void);
void   USART_RX_interrupt_Enable(void);
void   USART_RX_interrupt_Disable(void);
void   USART_send_string_Asynch(uint8 *str);

#endif /* ATMEGA32_INC_USART_H_ */
