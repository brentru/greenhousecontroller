/*
 * WaterLevel.c
 *
 * Created: 11/28/2016 9:27:38 PM
 * Author : jt049_000
 */ 

#include <avr/io.h>


int main(void)
{
	
    while (1) 
    {
		if(tenBitResult == HIGH)
		{
			// LCD Print Water Level Normal
		}
		else if(tenBitResult == LOW)
		{
			// LCD Print Water Level Low
			// Turn Off Water Pump If Running
		}
    }
}

