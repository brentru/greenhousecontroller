/*
 * Hbridge.c
 *
 * Created: 11/28/2016 10:01:44 PM
 * Author : dmaus
 */ 

#include <avr/io.h>
#include "Hbridge.h"


//Since the second output for each motor is tied to GND
//Only needed to work with the one input not tied to GND
//Returns 0 or 1 in order to show on LCD from main loop
int turnOnFan()
{
	PORTD |= (HIGH<<hbridge_IN1);
	return HIGH;
}
int turnOffFan()
{
	PORTD |= (LOW<<hbridge_IN1);
	return LOW;
}
int turnOnPump()
{
	PORTD |= (HIGH<<hbridge_IN3);
	return HIGH;
}
int turnOffPump()
{
	PORTD |= (LOW<<hbridge_IN3);
	return LOW;
}


