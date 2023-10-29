/*
 * ADC.c
 *
 * Created: 02.09.2023 10:41:49
 *  Author: Siegwart
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ADC.h"

uint16_t ADCValueL = 0;
uint16_t ADCValueH = 0;

void ADCinit()
{
	//VCC as input reference adc2
	ADCSRA =(1<<ADEN); 
	ADMUX = 2;
	ADCSRB  = 0; //free running mode
	DIDR0 |=  (1 << ADC2D); //disable digital input 2
	ADCSRA |=  (1 << ADIE) | 3; //enable freerun,clockdiv 128
}

void ADCRun()
{
	ADCSRA |= (1 << ADSC);
}

ISR(ADC_vect)
{
	ADCValueL  = ADCL;
	ADCValueH  = ADCH;
}