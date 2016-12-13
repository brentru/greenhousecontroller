/*
 * main.c
 * ece388/team1/greenhouseproject
 * david austin, brent rubell, jarred trottier
 */ 

//	start includes	//
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "ADC.h"
#include "dht.h"
#include "Hbridge.h"
#include "lcd.h"
#include "relay.h"
#include "SoilProbe.h"
#include "relay.h"
//	end includes	//

//	start defines	//
#define F_CPU 16000000
#define FOSC 16000000 //clk speed for 16mHz
#define BAUD 9600 //baud rate transmit 
#define MYUBBR FOSC/16/BAUD-1

/* Customer/Client Greenhouse PROFILE */
//	measure and average room temp
#define TEMP_ROOM 50
//	to use: uncomment the plant used 
#define LETTUCE 
//#DEFINE TOMATO
//#DEFINE CUSTOMPLANT
//#DEFINE CHI = 0
//#DEFINE CLOW = 0


/*	STORED VALUES: MEASURED & EXPECTED VALS */
//	water lvl sensor
#define WLEVEL_MAX 3.3
#define WLEVEL_MIN 0.8
//	photoresistor 
#define PHOTOVAL 450
//	soil sensor (expecting volts)
//	range: 65cm-90cm
#define SOIL_MAX 3
//	30cm
#define SOIL_MED 2
//	15cm
#define SOIL_LOW 1.6
//	0cm
#define SOIL_CRIT 0
#define TEMP_CRIT 0

/* STORED VALUES: STATUS & STATES */
//	servo position
#define SERVO_POS 0
//	led indicators 
#define STATUS_LED 0
#define PWR_LED 0
//	rst button state
#define BUTTON_STATE 0
//	relay
#define RELAY_STATE 0
//	end defines	//


static void USARTINIT()
{
	uint16_t ubbr = FOSC/16/BAUD-1
	// uart init 
	// set baud rate
	UBBR0H = (ubbr >> 8);
	UBBR0L = (ubbr);
	// enable rx and tx
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// set frame: 8 bit data, 2 stop bits
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}

unsigned char UART_RECIEVE(void)
{
	// wait for data to be recieved
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0; // get&return data from buffer
}

void USART_TRANSMIT (unsigned char data)
{
	// wait for empty transmit buf.
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data; // put data into buffer, send data
}

//	adc vector 
ISR(ADC_vect)
{
	PORTD = ADCL;
	PORTB = ADCH;
}

//	main loop
main()
{
	// local vars
	float temp_low=0;
	float temp_high=0;
	float temp;
	float humidity;
	//	reset servo
	servo.turn_middle()
	//	interrupts
	sei();
	// init serial comms
	USARTINIT();
	//	transmit a welcome message to terminal
	USART_TRANSMIT('b');  
	USART_TRANSMIT('o');  
	USART_TRANSMIT('o');  
	USART_TRANSMIT('t');  
	//	RELAY
	relay_DDR |= (1<<relay_OUTPUTPIN); //Set as output
	relay_PORT|= (0<<relay_OUTPUTPIN); //Initialize as low
	//	LCD 
	DDRB = 0xFF;
	DDRC = 0xFF;
	Lcd4_Init();
	//	hbridge
	//Enable DDRD of pins 3,5,6,7 as outputs
	DDRD |= (11101000);
	PORTD |= (LOW<<hbridge_ENA);
	PORTD |= (LOW<<hbridge_ENB);
	PORTD |= (HIGH<<hbridge_IN1);
	PORTD |= (HIGH<<hbridge_IN3);
	//	Servo Timer (ISR)
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);        //NON Inverted PWM
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //PRESCALER=64 MODE 14(FAST PWM)
	//	fPWM=50Hz
	ICR1=4999;  
	DDRD|=(1<<PD4)|(1<<PD5);
	/* CUSTOMER PROFILE SETUP */
	#ifdef LETTUCE
		//	Plant: Lettuce 
		temp_low = 15;
		temp_hi = 50
	#ifdef TOMATO
		//	PLANT: Tomato 
		temp_low = 10;
		temp_hi = 55;
	#else
		//	CUSTOM PLANT
		temp_hi = CHI;
		temp_low = CLOW;
	#endif 
	while(;;)
	{
		//	get photoresistor value
		ADCSRA|= (1<<ADSC);
		if(ADCSRA <=350)
		{
			servo.turn_right();
			//	turn relay off
			relay.lightOff();
		}
		else
		{
			servo.turn_left();
			relay.lightOn();
		}
		//	get DHT data
		dht_getdata(temp,humidity);
		//	DHT control loop
		if((temp>=temp_hi))
		{
			Hbridge.turnOnFan();
		}
		else
		{
			//	do nothing
		}
		//	water level loop
		int soillvl = analogSoilRead();
		int waterlvl = analogWaterRead();
		if(soillvl==SOIL_LOW)
		{
			while(soillvl < SOIL_LOW)
			{
			Lcd4_Clear();
			Lcd4_Set_Cursor(0,0);
			Lcd4_Write_String("Filling Soil");
			_delay_ms(2000);
			turnOnPump();
			}
			turnOffPump();
			Lcd4_Clear();
		}
		else if(soillvl<=SOIL_CRIT)
		{
			Lcd4_Clear();
			Lcd4_Set_Cursor(0,0);
			Lcd4_Write_String("SOIL CRITICAL");
			_delay_ms(2000);
			while(soillvl < SOIL_LOW)
			{
				turnOnPump();
			}
			turnOffPump();
			Lcd4_Clear();
		}
		else if(soillvl>=SOIL_MAX)
		{
			Lcd4_Clear();
			Lcd4_Set_Cursor(0,0);
			Lcd4_Write_String("Soil Good");
			_delay_ms(2000);
			Lcd4_Clear();
			turnOffPump();
		}
		//	PRINTING to LCD
		Lcd4_Clear();
		Lcd4_Set_Cursor(0,0);
		Lcd4_Write_String("T: %f, H: %f",temp,humidity);
		_delay_ms(2000);
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String("soil-lvl: %d,water-lvl: %d",soillvl,waterlvl);
		_delay_ms(2000);
		Lcd4_Set_Cursor(4,2);
		Lcd4_Write_String("light status: %d",ADCSRA);
		_delay_ms(2000);


	}
	
}

