/*
 * Node B  (ATmega32 #2)  -  SPI SLAVE
 *   receives the temperature from Node A over SPI
 *   shows it on the 16x2 LCD over I2C (PCF8574)
 *   sends it to the Virtual Terminal over UART (9600, 8N1)
 */
#include "../MCAL/inc/SPI.h"
#include "../MCAL/inc/I2C.h"
#include "../MCAL/inc/USART.h"
#include "../HAL/inc/LCD_I2C.h"

#define TEMP_LIMIT   30

int main(void) {
	uint8 temp;
	char line[17];
	UART_Config_t uart = { USART_BaudRate_9600, DOUBLE_Speed_DIS };

	SPI_Init();
	I2C_init();
	USART_init(&uart);
	LCD_Init();

	LCD_GoTo(0, 0);
	LCD_SendString("Waiting data...");

	while (1) {
		/* blocks until Node A clocks a byte in */
		temp = SPI_SendRecieveData(0x00);

		/* LCD (I2C) */
		LCD_GoTo(0, 0);
		sprintf(line, "Temp = %3u C    ", (unsigned int) temp);
		LCD_SendString(line);
		LCD_GoTo(1, 0);
		LCD_SendString(temp > TEMP_LIMIT ? "ALARM: HIGH!    " : "Status: Normal  ");

		/* UART -> Virtual Terminal  (sendString adds '\r', we add '\n') */
		sprintf(line, "Temp = %u C", (unsigned int) temp);
		USART_sendString((uint8 *) line);
		USART_send('\n');
	}
}
