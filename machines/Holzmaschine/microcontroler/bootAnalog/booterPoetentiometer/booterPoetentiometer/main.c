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

static int step = 30;
static unsigned char i2cBuff[2];
static int hoerterCount = 0;


#define NULL_REGISTER 0xFF
uint8_t currentRegister = NULL_REGISTER;

void usitwi_onStart(uint8_t read) {
	if (!read) {
		currentRegister = NULL_REGISTER;
	}
}

void usitwi_onStop() {
	currentRegister = NULL_REGISTER;
}

uint8_t usitwi_onRead() {
	switch(currentRegister) {
		case 0:
		return ADCL;
		break;
		case 1:
		return ADCH;
		break;
		default:
		return 0xFF;
		break;
	}
	currentRegister = NULL_REGISTER;
}

void usitwi_onWrite(uint8_t value) {
	if (currentRegister == NULL_REGISTER) {
		currentRegister = value;
		} else {
		switch(currentRegister) {
			case 0:
			//register1 = ADCL;
			break;
			case 1:
			//register2 = ADCH;
			break;
		}
		currentRegister = NULL_REGISTER;
	}
}

uint8_t usitwi_address = 0x42;

int main(void)
{
    while (1) 
    {
		switch(step)
		{
			case 0: //init master
			{
				//set pb3 to input (raspberry ready)
				DDRB &= ~(1 << PB1);
				PORTB |= (1 << PB1);  //activate pull-up resistor for PB3
				//init master i2c
				USI_I2C_Master_Init();
				step = 10;
				break;
			}
			case 10: //lamps on
			{
				for(hoerterCount = 0;hoerterCount < 3;hoerterCount++)
				{
					i2cBuff[0] = (0x20 + hoerterCount) << 1;
					i2cBuff[1] = 0x00;
					USI_I2C_Master_Start_Transmission(i2cBuff, 2);
					_delay_ms(250);
				}
				step = 20;
				break;
			}
			case 20: //lamps off
			{
				for(hoerterCount = 0;hoerterCount < 3;hoerterCount++)
				{
					i2cBuff[0] = (0x20 + hoerterCount) << 1;
					i2cBuff[1] = 0xFF;
					USI_I2C_Master_Start_Transmission(i2cBuff, 2);
					_delay_ms(250);
				}
				//Check raspberry
				if((PINB & (1 << PB1)))
				{
					step = 30;
				}
				else
				{
					step = 10;
				}
				break;
			}
			case 30: //init slave and adc
			{
				usitwi_init();
				ADCinit();
				sei();
				step = 40; //idle
			}
			case 40:
			{
				
				break;
			}
		}
		
    }
}

