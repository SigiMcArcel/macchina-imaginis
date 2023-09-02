/*
 * booterPoetentiometer.c
 *
 * Created: 28.08.2023 19:29:06
 * Author : Siegwart
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "I2CMaster.h"
#include "I2CSlave.h"
#include "ADC.h"

static int step = 0;
static unsigned char i2cBuff[2];
static int hoerterCount = 0;
static unsigned char adcHigh,adcLow = 0;

extern unsigned char* USI_Slave_register_buffer[];

int main(void)
{
    while (1) 
    {
		switch(step)
		{
			case 0: //init master
			{
				//set pb3 to input (raspberry ready)
				DDRB &= ~(1 << PB3);
				//init master i2c
				USI_I2C_Master_Init();
				step = 10;
				break;
			}
			case 10: //lamps on
			{
				for(hoerterCount = 0;hoerterCount < 3;hoerterCount++)
				{
					i2cBuff[0] = (0x20 + hoerterCount);
					i2cBuff[1] = 0x00;
					USI_I2C_Master_Start_Transmission(i2cBuff, 2);
				}
				step = 20;
				break;
			}
			case 20: //lamps off
			{
				for(hoerterCount = 0;hoerterCount < 3;hoerterCount++)
				{
					i2cBuff[0] = (0x20 + hoerterCount);
					i2cBuff[1] = 0xFF;
					USI_I2C_Master_Start_Transmission(i2cBuff, 2);
				}
				//Check raspberry
				if(PORTB && (1 << PB3))
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
				USI_I2C_Init(0x68);
				ADCinit();
				USI_Slave_register_buffer[0] = &adcLow;
				USI_Slave_register_buffer[1] = &adcHigh;
				step = 40; //idle
			}
			case 40:
			{
				adcLow = ADCL;
				adcHigh = ADCH;
				break;
			}
		}
		_delay_ms(250);
    }
}

