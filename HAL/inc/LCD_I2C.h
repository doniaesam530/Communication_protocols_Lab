/*
 * LCD_I2C.h  -  16x2 LCD (HD44780 / LM016L) through a PCF8574 I2C expander, 4-bit mode
 *
 * Wiring assumed (matches the Proteus circuit):
 *   PCF8574 P0 -> RS    P1 -> RW    P2 -> E    P3 -> backlight (unused)
 *   PCF8574 P4..P7 -> LCD D4..D7
 *   A0 = A1 = A2 = GND  ->  7-bit address 0x20  ->  8-bit write address 0x40
 * If your wires are different, change only the macros below.
 */
#ifndef HAL_INC_LCD_I2C_H_
#define HAL_INC_LCD_I2C_H_

#include "../../MCAL/inc/I2C.h"

#define LCD_I2C_ADDR     0x40      /* 8-bit write address (use 0x70 for PCF8574A) */

#define LCD_RS_MASK      (1 << 0)
#define LCD_RW_MASK      (1 << 1)
#define LCD_EN_MASK      (1 << 2)
#define LCD_BL_MASK      (1 << 3)
#define LCD_DATA_SHIFT   4         /* D4..D7 on P4..P7 */

void LCD_Init(void);
void LCD_SendCommand(uint8 cmd);
void LCD_SendChar(uint8 ch);
void LCD_SendString(const char *str);
void LCD_GoTo(uint8 row, uint8 col);
void LCD_Clear(void);

#endif
