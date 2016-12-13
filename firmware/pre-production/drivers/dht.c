// dht.c
// Davide Gironi



#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "dht.h"

/*
 * get data from sensor
 */
#if DHT_FLOAT == 1
int8_t dht_getdata(float *temperature, float *humidity) {
#elif DHT_FLOAT == 0
int8_t dht_getdata(int8_t *temperature, int8_t *humidity) {
#endif
	uint8_t bits[5];
	uint8_t i,j = 0;

	memset(bits, 0, sizeof(bits));
	
	
	//reset port
	DHT_DDR |= (1<<DHT_INPUTPIN); //output
	DHT_PORT |= (1<<DHT_INPUTPIN); //high
	_delay_ms(100);

	// Host pulls low
	DHT_PORT &= ~(1<<DHT_INPUTPIN); 
	_delay_ms(5);
	
	//	 Host pulls up
	DHT_PORT |= (1<<DHT_INPUTPIN); //high
	DHT_DDR &= ~(1<<DHT_INPUTPIN); //input
	_delay_us(40);
	
	//	(80us) if it's high, pull low
	if((DHT_PIN & (1<<DHT_INPUTPIN))) {
		return -1;
	}
	_delay_us(80);
	//	(80us) if it's low, pull high
	if(!(DHT_PIN & (1<<DHT_INPUTPIN))) {
		return -1;
	}
	_delay_us(80);

	//read the data
	uint16_t timeoutcounter = 0;
	for (j=0; j<5; j++) { //read 5 byte
		uint8_t result=0;
		for(i=0; i<8; i++) {//read every bit
			timeoutcounter = 0;
			while(!(DHT_PIN & (1<<DHT_INPUTPIN))) { //wait for an high input (non blocking)
				timeoutcounter++;
				if(timeoutcounter > DHT_TIMEOUT) {
					return -1; //timeout
				}
			}
			_delay_us(30);
			if(DHT_PIN & (1<<DHT_INPUTPIN)) //if input is high after 30 us, get result
				result |= (1<<(7-i));
			timeoutcounter = 0;
			while(DHT_PIN & (1<<DHT_INPUTPIN)) { //wait until input get low (non blocking)
				timeoutcounter++;
				if(timeoutcounter > DHT_TIMEOUT) {
					return -1; //timeout
				}
			}
		}
		bits[j] = result;
	}



	//	RESET for next transmission
	DHT_DDR |= (1<<DHT_INPUTPIN); //output
	DHT_PORT |= (1<<DHT_INPUTPIN); //low
	_delay_ms(100);




	//	GET TEMP
	// GET checksum
	if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) 
	{
		//return temperature and humidity
	
		uint16_t rawhumidity = bits[0]<<8 | bits[1];
		uint16_t rawtemperature = bits[2]<<8 | bits[3];
		//	CHECK negative temp (high bit check)
		if(rawtemperature & 0x8000) 
		{
			//	GET neg temp
			*temperature = (float)((rawtemperature & 0x7FFF) / 10.0) * -1.0;
		} 
		else 
		{
			//	GET temperature
			*temperature = (float)(rawtemperature)/10.0;
		}
		//	GET humidity 
		*humidity = (float)(rawhumidity)/10.0;

		return 0;
	}

	return -1;

	
	}

	//	GET output values
	float humidity = 0;
	float temperature = 0;
	return dht_getdata(&temperature, humidity);
	return dht_getdata(temperature, &humidity);
	return dht_getdata(temperature, humidity);



}
