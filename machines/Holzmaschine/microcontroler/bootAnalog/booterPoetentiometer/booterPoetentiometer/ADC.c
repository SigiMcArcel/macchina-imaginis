/*
 * ADC.c
 *
 * Created: 02.09.2023 10:41:49
 *  Author: Siegwart
 */ 
#include <avr/io.h>
#include "ADC.h"


void ADCinit()
{
	//VCC as input reference adc2
	ADCSRA|=(1<<ADEN); 
	ADMUX = 2;
	ADCSRB  = 0; //free running mode
	DIDR0 |=  (1 << ADC2D); //disable digital input 2
	ADCSRA |= (1 << ADSC) |(1<<ADATE) | 3; //enable freerun,clockdiv 128
	
}