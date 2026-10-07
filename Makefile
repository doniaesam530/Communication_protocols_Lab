# Builds both nodes:  make        -> build/NodeA.hex , build/NodeB.hex
# Needs avr-gcc + make (Atmel/Microchip AVR toolchain or WinAVR).
MCU    = atmega32
F_CPU  = 8000000UL
CC     = avr-gcc
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -std=gnu99 -ffunction-sections -fdata-sections
LDFLAGS= -mmcu=$(MCU) -Wl,--gc-sections

COMMON = MCAL/DIO.c MCAL/SPI.c MCAL/I2C.c MCAL/USART.c MCAL/ADC.c

A_SRC = NodeA_Master/main.c $(COMMON)
B_SRC = NodeB_Slave/main.c  $(COMMON) HAL/LCD_I2C.c

all: build/NodeA.hex build/NodeB.hex

build/NodeA.elf: $(A_SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) -DMASTER_MODE $(A_SRC) -o $@ $(LDFLAGS)

build/NodeB.elf: $(B_SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) -DSLAVE_MODE $(B_SRC) -o $@ $(LDFLAGS)

%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom $< $@
	avr-size --mcu=$(MCU) -C $<

clean:
	rm -rf build
