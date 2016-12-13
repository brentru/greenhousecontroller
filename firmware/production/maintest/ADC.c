/*
 * ADC.c
 *
 * Created: 11/28/2016 4:50:09 PM
 *  Author: jt049_000
 */ 
 #include "alldecs.h"

//	Vegetronix
int analogSoilRead()
{
	ADCSRA = 0x87;
	ADMUX = 0x42;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA&(1<<ADIF))==0);
	theLowEight = ADCL;
	tenBitResult = ADCH<<8 | theLowEight;
	return tenBitResult;
}

//	bouyancy 
int analogWaterRead()
{
	ADCSRA = 0x87;
	ADMUX = 0x43;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA&(1<<ADIF))==0);
	theLowEight = ADCL;
	tenBitResult = ADCH<<8 | theLowEight;
	return tenBitResult;
}

int analogPhoto1Read()
{
	ADCSRA = 0x87;
	ADMUX = 0x40;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA&(1<<ADIF))==0);
	theLowEight = ADCL;
	tenBitResult = ADCH<<8 | theLowEight;
	return tenBitResult;
}

int analogPhoto2Read()
{
	ADCSRA = _BV(ADEN) | (0b111<<ADPS0);//0x87;
	ADMUX = 0x41;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA&(1<<ADIF))==0);
	theLowEight = ADCL;
	tenBitResult = ADCH<<8 | theLowEight;
	return tenBitResult;
}
