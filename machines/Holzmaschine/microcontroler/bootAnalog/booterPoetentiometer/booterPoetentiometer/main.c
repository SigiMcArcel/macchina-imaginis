/*
 * booterPoetentiometer.c
 *
 * Created: 28.08.2023 19:29:06
 * Author : Siegwart
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "I2CMaster.h"
#include "I2CSlave.h"
#include "ADC.h"

extern uint16_t ADCValueL;
extern uint16_t ADCValueH;


#define NULL_REGISTER 0x00
uint8_t currentRegisterR = NULL_REGISTER;
uint8_t currentRegisterW = NULL_REGISTER;

void usitwi_onStart(uint8_t read) {
	if (!read) {
		currentRegisterR = NULL_REGISTER;
	}
}

void usitwi_onStop() {
	currentRegisterR = NULL_REGISTER;
}

uint8_t usitwi_onRead() {
	uint8_t value = 0;
	switch(currentRegisterR) {
		case 0:
		value = ADCValueL;
		break;
		case 1:
		value = ADCValueH;
		break;
		default:
		return 0xFF;
		break;
	}
	currentRegisterR++;
	if(currentRegisterR > 1)
	{
		currentRegisterR = NULL_REGISTER;
	}
	return value;
}

void usitwi_onWrite(uint8_t value) {
	if(value == 0xff)
	{
		currentRegisterR = NULL_REGISTER;
	}
}

uint8_t usitwi_address = 0x42;

int main(void)
{
	usitwi_init();
	ADCinit();
	sei();
    while (1) 
    {
		
	}
}

