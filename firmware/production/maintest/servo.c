#include "servo.h"

void Wait()
{
	uint8_t i;
	for(i=0;i<50;i++)
	{
		_delay_loop_2(0);
		_delay_loop_2(0);
		_delay_loop_2(0);
	}

}

void ZeroDegree()
{
	 OCR1A=249;   //0 degree
	Wait();
	//PORTB |= (1<<PORTB1);
	//_delay_us(800);
	//PORTB &= ~(1<<PORTB1);
	//_delay_us(19200);
}

void NinetyDegree()
{
	OCR1A=374;  //90 degree
	Wait();
	//PORTB |= (1<<PORTB1);
	//_delay_us(1500);
	//PORTB &= ~(1<<PORTB1);
	//_delay_us(18500);
}

/*void ThirtyDegree()
{
	OCR1A=425;  //135 degree
	Wait();
}*/

void EightyDegree()
{
	OCR1A=499;  //180 degree
	Wait();
	//PORTB |= (1<<PORTB1);
	//_delay_us(2200);
	//PORTB &= ~(1<<PORTB1);
	//_delay_us(17800);
}