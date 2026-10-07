/*
 * ADC.h  -  ADC driver (ATmega32), polling
 */
#ifndef ATMEGA32_INC_ADC_H_
#define ATMEGA32_INC_ADC_H_

#include "Atmega32.h"
#include "../../Library/BIT_MATH.h"

/* Reference selection: pick ONE */
#define ADC_REF_INTERNAL_2V56      /* AREF left open, Vref = 2.56 V (0.25 C per count for LM35) */
//#define ADC_REF_AVCC             /* Vref = AVCC = 5 V */

#ifdef ADC_REF_INTERNAL_2V56
#define ADC_VREF_MV   2560UL
#else
#define ADC_VREF_MV   5000UL
#endif

void   ADC_Init(void);
uint16 ADC_Read(uint8 channel);              /* raw 10-bit value (0..1023) */
uint16 ADC_ReadMilliVolt(uint8 channel);

#endif
