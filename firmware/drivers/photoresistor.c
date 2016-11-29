/*
 * Photoresistor.c
 *
 * Created: 11/23/2016 10:54:01 AM
 *  Author: brent
 */ 


// photoresistor @ ADC0 & adc1, PC0/PC1

#include <avr/io.h>
#include <avr/interrupt.h>

ISR(ADC_vect)
{
	PORTD = ADCL;
	PORTB = ADCH;
}

int main(void)
{
	//	
	DDRB = 0xFF; 
	DDRD = 0xFF;
	DDRA = 0;
	sei();
	ADCSRA = 0x8F;
	ADMUX = 0xC0;
	ADCSRA|= (1<<ADSC);
	
    
}