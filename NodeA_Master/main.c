/*
 * Node A  (ATmega32 #1)  -  SPI MASTER
 *   LM35 (PA0 / ADC0) -> temperature -> SPI -> Node B
 *   LED on PC0 turns ON when temperature > 30 C
 */
#include "../MCAL/inc/ADC.h"
#include "../MCAL/inc/SPI.h"
#include "../MCAL/inc/DIO.h"

#define LED_PORT      PORT_C
#define LED_PIN       PIN_0
#define LM35_CHANNEL  0
#define TEMP_LIMIT    24

int main(void) {
	uint16 adc, temp;

	DIO_initPin(LED_PIN, LED_PORT, OUTPUT);
	DIO_WritePin(LED_PIN, LED_PORT, LOW);

	ADC_Init();
	SPI_Init();

	while (1) {
		adc = ADC_Read(LM35_CHANNEL);

		/* LM35 = 10 mV / C  ->  temp = adc * Vref(mV) / 1024 / 10  (rounded) */
		temp = (uint16) (((unsigned long) adc * ADC_VREF_MV + 5120UL) / 10240UL);
		if (temp > 255)
			temp = 255;

		/* LED alarm */
		if (temp > TEMP_LIMIT)
			DIO_WritePin(LED_PIN, LED_PORT, HIGH);
		else
			DIO_WritePin(LED_PIN, LED_PORT, LOW);

		/* send one byte to Node B */
		SPI_SlaveSelect();
		SPI_SendRecieveData((uint8) temp);
		SPI_SlaveDeselect();

		_delay_ms(500);
	}
}
