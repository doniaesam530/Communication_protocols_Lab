# Mini Project - Temperature Monitor (2x ATmega32, SPI + I2C + UART)

Node A (master): LM35 -> ADC -> SPI -> Node B ; LED on PC0 when T > 30 C
Node B (slave) : SPI in -> LCD 16x2 via PCF8574 (I2C) + UART to Virtual Terminal (9600 8N1)

## Pins
| Node A (U1)      | Connected to            |
|------------------|-------------------------|
| PA0 (ADC0)       | LM35 VOUT               |
| PB4 (SS)         | Node B PB4 (SS)         |
| PB5 (MOSI)       | Node B PB5              |
| PB6 (MISO)       | Node B PB6              |
| PB7 (SCK)        | Node B PB7              |
| PC0              | LED + 330R -> GND       |

| Node B (U2)      | Connected to                         |
|------------------|--------------------------------------|
| PB4..PB7         | SPI lines from Node A                |
| PC0 (SCL)        | PCF8574 SCL (+ 4.7k pull-up to 5V)   |
| PC1 (SDA)        | PCF8574 SDA (+ 4.7k pull-up to 5V)   |
| PD1 (TXD)        | Virtual Terminal RXD                 |

PCF8574: A0=A1=A2=GND (address 0x40 write). P0=RS, P1=RW, P2=E, P4..P7=D4..D7.
If your wiring differs, edit the macros in HAL/inc/LCD_I2C.h.

## Proteus settings (both ATmega32)
* Clock Frequency = 8MHz  (must equal F_CPU = 8000000UL in MCAL/inc/Atmega32.h)
* U1 Program File = build/NodeA.hex   |   U2 Program File = build/NodeB.hex
* Virtual Terminal: 9600 baud, 8 data bits, no parity, 1 stop bit
* AREF can stay open (internal 2.56 V reference), AVCC = 5V

## Build
    make            # needs avr-gcc ; produces build/NodeA.hex and build/NodeB.hex
Atmel Studio: make two projects (one per node), add the common MCAL/HAL/Library files,
and add the symbol MASTER_MODE (Node A) or SLAVE_MODE (Node B) in
Toolchain > AVR/GNU C Compiler > Symbols. Add include dirs for MCAL/inc, HAL/inc, Library.

## Fixes made to the lab driver skeleton
* SPI.c / USART.c / USART_recieve were empty -> implemented; ADC driver added; LCD-over-I2C HAL added
* SPI.h: prototype typo SPI_SlendRecieveData -> SPI_SendRecieveData
* USART.h: baudRate was uint8 (cannot hold 9600) ; DOUBLE_Speed_EN was defined twice
* USART_sendString stopped early (`i < str[i]`) -> now stops at '\0'
* I2C: TWSR status now masked (&0xF8); no infinite wait if the slave does not ACK;
  I2C_Stop waits for TWSTO; Slave_Read had `(1 < TWINT)` typo
* F_CPU 1 MHz made 100 kHz I2C impossible (negative TWBR) -> default 8 MHz
