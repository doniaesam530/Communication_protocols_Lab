/*
 * LCD_I2C.c  -  16x2 LCD through PCF8574 (I2C), 4-bit mode
 */
#include "inc/LCD_I2C.h"

/* Send one nibble (already in P4..P7 position + control bits) with an E pulse.
 * Both bytes go in a single I2C transaction. */
static void LCD_WriteNibble(uint8 nibbleAndCtrl) {
	I2C_Start();
	if (I2C_SendSlaveAddress(LCD_I2C_ADDR, I2C_WRITE)) {
		I2C_Write(nibbleAndCtrl | LCD_EN_MASK | LCD_BL_MASK);   /* E = 1 */
		I2C_Write((nibbleAndCtrl & ~LCD_EN_MASK) | LCD_BL_MASK); /* E = 0 */
		I2C_Stop();
	}
}

static void LCD_WriteByte(uint8 byte, uint8 rs) {
	uint8 ctrl = rs ? LCD_RS_MASK : 0;
	LCD_WriteNibble((byte & 0xF0) | ctrl);             /* high nibble (already on P4..P7) */
	LCD_WriteNibble(((byte << 4) & 0xF0) | ctrl);      /* low nibble */
}

void LCD_SendCommand(uint8 cmd) {
	LCD_WriteByte(cmd, 0);
	if (cmd == 0x01 || cmd == 0x02)
		_delay_ms(2);
	else
		_delay_us(60);
}

void LCD_SendChar(uint8 ch) {
	LCD_WriteByte(ch, 1);
	_delay_us(60);
}

void LCD_SendString(const char *str) {
	while (*str)
		LCD_SendChar((uint8) *str++);
}

void LCD_GoTo(uint8 row, uint8 col) {
	LCD_SendCommand(0x80 | ((row ? 0x40 : 0x00) + col));
}

void LCD_Clear(void) {
	LCD_SendCommand(0x01);
}

void LCD_Init(void) {
	_delay_ms(50);                       /* power-up wait */
	/* HD44780 "initialisation by instruction" for 4-bit interface */
	LCD_WriteNibble(0x30);  _delay_ms(5);
	LCD_WriteNibble(0x30);  _delay_us(150);
	LCD_WriteNibble(0x30);  _delay_us(150);
	LCD_WriteNibble(0x20);  _delay_us(150);   /* switch to 4-bit */

	LCD_SendCommand(0x28);   /* 4-bit, 2 lines, 5x8 font */
	LCD_SendCommand(0x0C);   /* display ON, cursor OFF */
	LCD_SendCommand(0x06);   /* entry mode: increment, no shift */
	LCD_SendCommand(0x01);   /* clear */
}
