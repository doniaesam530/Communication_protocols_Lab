/*
 * ADC.c  -  ADC driver (ATmega32), polling
 */
#include "inc/ADC.h"

void ADC_Init(void) {
#ifdef ADC_REF_INTERNAL_2V56
	ADMUX = (1 << REFS1) | (1 << REFS0);          /* internal 2.56 V */
#else
	ADMUX = (1 << REFS0);                         /* AVCC */
#endif
	/* enable ADC, prescaler = 64  (8 MHz / 64 = 125 kHz, inside 50..200 kHz) */
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

uint16 ADC_Read(uint8 channel) {
	uint8 lo, hi;
	ADMUX = (ADMUX & 0xE0) | (channel & 0x1F);    /* keep REFS/ADLAR, set channel */
	SET(ADCSRA, ADSC);                            /* start conversion */
	while (GET(ADCSRA, ADSC))
		;                                         /* wait until done */
	lo = ADCL;                                    /* ADCL must be read first */
	hi = ADCH;
	return ((uint16) hi << 8) | lo;
}

uint16 ADC_ReadMilliVolt(uint8 channel) {
	return (uint16) (((unsigned long) ADC_Read(channel) * ADC_VREF_MV) / 1024UL);
}
