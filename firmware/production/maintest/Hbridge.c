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
void hbridgeINIT()
{
	hbridge_PORT |= ((1<<hbridge_ENA) | (1<<hbridge_ENB));
}

void turnOnFan()
{
	hbridge_PORT|= (HIGH<<hbridge_ENA);
	hbridge_PORT |= (HIGH<<hbridge_IN1);
}
void turnOffFan()
{
	hbridge_PORT&= ~(HIGH<<hbridge_ENA);

	hbridge_PORT &= ~((HIGH<<hbridge_IN1));
}
void turnOnPump()
{
	hbridge_PORT|= (HIGH<<hbridge_ENB);
	hbridge_PORT |= (HIGH<<hbridge_IN3);
}
void turnOffPump()
{
	hbridge_PORT &= ~(HIGH<<hbridge_ENB);
	hbridge_PORT &= ~(HIGH<<hbridge_IN3);
}


