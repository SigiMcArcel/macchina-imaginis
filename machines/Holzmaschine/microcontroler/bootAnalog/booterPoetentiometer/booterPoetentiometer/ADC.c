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
	//VCC as input referenc
	ADMUX = 2;
	ADCSRB  = 0; //free running mode
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | 3; //enable freerun,clockdiv 128
	DIDR0 |=  (1 << ADC2D); //disable digital input 2
}