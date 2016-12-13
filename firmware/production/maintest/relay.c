/*
 * RelayCode.c
 *
 * Created: 11/28/2016 9:01:00 PM
 * Author : dmaus
 */ 

#include <avr/io.h>
#include "relay.h"
    
void lightOn()
{
	relay_PORT |= (1<<relay_OUTPUTPIN);
}
		
void lightOff()
{
	relay_PORT &= ~(1<<relay_OUTPUTPIN);

}

  

