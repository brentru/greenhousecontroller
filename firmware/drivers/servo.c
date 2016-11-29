/*
 * SG90.c
 *
 * Created: 11/23/2016 11:16:27 AM
 *  Author: brent
 */ 


/*
SG90 Positions
0 (middle) = 1.5ms
90 (right) = 2ms
-90 (left) = 1ms
*/

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>

void WaitServo()
{
	uint16_t i;
	for(i=0;i<50;i++)
	{
		_delay_loop_2(0);
		_delay_loop_2(0);
		_delay_loop_2(0);
	}
}

//	turn servo to the middle
void turn_middle(()
{
	OCR1A = 0.375;
	WaitServo();
}
void turn_right()
{
	OCR1A = 0.5;
	WaitServo();
}
void turn_left()
{
	OCR1A = 0.25; 
	WaitServo();
}


int main(void)
{
	
	//FOR TIMER1
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);        //NON Inverted PWM
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //PRESCALER=64 MODE 14(FAST PWM)
	ICR1=4999;  //fPWM=50Hz
	DDRD|=(1<<PD4)|(1<<PD5);
	
	
    while(1)
    {
        
    }
}