/*
 * ADC.h
 *
 * Created: 11/28/2016 4:50:09 PM
 *  Author: jt049_000
 */ 

uint8_t theLowEight;

#ifndef ADC_H_
#define ADC_H_

//	Vegetronix
int analogSoilRead()
{
	uint16_t tenBitResult;
	DDC2 = 0;
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
	uint16_t tenBitResult;
	DDC3 = 0;
	ADCSRA = 0x87;
	ADMUX = 0x43;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA&(1<<ADIF))==0);
	theLowEight = ADCL;
	tenBitResult = ADCH<<8 | theLowEight;
	return tenBitResult;
}


#endif /* ADC_H_ */