/*
 * main.c
 * ece388/team1/greenhouseproject
 * dave a., brent r., jarred t.
*/ 

/*	important defines	*/
#define F_CPU 16000000UL
#define _BV(bit) (1 << (bit))
//	custom plant being used for model
//	replace with customer plant type
#define CUSTOM 

/*	important includes	*/
#include "alldecs.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "dht.h"

/*	variable decls	*/
//	buffers for LCD display
char buffer[30];
char buf2[10];
//	global sensor vars
volatile float temp_low=0;
volatile float temp_high=0;
volatile float sTemp = 0;
volatile float temp;
volatile int lhtVal1;
volatile int lhtVal2;
volatile int waterlvl;
volatile int soillvl;
//	global actuator booleans
volatile int pump = 0;
volatile int light = 0;
volatile int servo = 0;
volatile int fan = 0;
main()
{
	/*	init system	*/
	initRelay();
	initMainLCD();
	initHbridge();
	initServo();
	initADC();
	/*	pre-run setups */
	custSetup();
	LCDSetup();
	/*	realtime loop	*/
	while(1)
	{
		temp_light();
		checkSoil();
	}
}
void initRelay()
{
	//	set as output
	relay_DDR |= (1<<relay_OUTPUTPIN); 
	//	initialize as low
	relay_PORT &= ~(1<<relay_OUTPUTPIN);
	 //DDRB |= (1<<DDB0);
}

void initMainLCD()
{
	//	Initialize Pins for LCD as outputs
	DDRB |= (1<<DDB2)|(1<<DDB3)|(1<<DDB4)|(1<<DDB5);
	DDRC |= (1<<DDC4)|(1<<DDC5);
	//Clear LCD
	Lcd4_Clear();
	//Run initalization of the LCD
	Lcd4_Init();
}

void initHbridge()
{
	DDRD |= (11101000); 
}

void initServo()
{
	////Servo
	DDRB |= (1<<DDB1);
	//	Servo Timer (ISR)
	TCCR1A|=(1<<COM1A1)|(1<<COM1A0)|(1<<WGM11);        //NON Inverted PWM
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //PRESCALER=64 MODE 14(FAST PWM)
	//fPWM=16MHz
	//ICR1=5000-1;  
	PORTB |= (1<<PORTB1);
}

void initADC()
{
	//	Initialize ADC Ports
	DDRC &= ~((0<<DDC0)|(0<<DDC1)|(0<<DDC2)|(0<<DDC3));
	PORTD |= (1<<PORTD2);
}

void custSetup()
{
	/* CUSTOMER PROFILE SETUP */
	#ifdef LETTUCE
		//Plant: Lettuce 
		temp_low = 15;
		temp_high = 50;
		sTemp = 35;
		
	#endif
	#ifdef TOMATO
		//	PLANT: Tomato 
		temp_low = 10;
		temp_high = 55;
		sTemp = 40;
	#endif
	#ifdef CUSTOM
		//	CUSTOM PLANT
		temp_low = 23;
		temp_high = 24;
		sTemp = 23;
	#endif 
}

void LCDSetup(){
	//	LCD Setup
	Lcd4_Set_Cursor(1,0);
	//	preset temp
	Lcd4_Write_String("St:"); 
	Lcd4_Set_Cursor(1,4);
	itoa(sTemp,buf2,10);
	Lcd4_Write_String(buf2);
	Lcd4_Set_Cursor(1,7);
	//	inside Temp
	Lcd4_Write_String("It"); 
	//	light values
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("L:");
	//	action bar
	Lcd4_Set_Cursor(3,0);
	Lcd4_Write_String("Action:");
	//	error bar
	Lcd4_Set_Cursor(4,0);
	Lcd4_Write_String("Error:");
}

void temp_light()
{
	//get photoresistor value
	lhtVal1 = analogPhoto1Read();
	lhtVal2 = analogPhoto2Read();
	//	lcd display
	Lcd4_Set_Cursor(2,2);
	Lcd4_Write_String("                  ");
	Lcd4_Set_Cursor(2,2);
	itoa(lhtVal1,buffer,10);
	Lcd4_Write_String(buffer);
	Lcd4_Set_Cursor(2,7);
	itoa(lhtVal2,buffer,10);
	Lcd4_Write_String(buffer);
	//Lcd4_Write_String("Dark >450");
	/*Check temperature*/
	//	get DHT data
	dht_gettemperature(&temp);
	/*Display temp*/
	Lcd4_Set_Cursor(1,10);
	itoa(temp,buf2,10);
	Lcd4_Write_String(buf2);

	if((lhtVal1 > 500)&&(lhtVal2 < 500)) //Dark Reading 
	{
			//Lcd4_Write_String("Dark >450");
			/*Check temperature*/
			//	get DHT data
			dht_gettemperature(&temp);
			/*Display temp*/
			Lcd4_Set_Cursor(1,10);
			itoa(temp,buf2,10);
			Lcd4_Write_String(buf2);

			if(temp >= temp_high)
			{	
				lightOff();			
				ZeroDegree(); 
				turnOnFan();
						
				Lcd4_Set_Cursor(3,7);
				Lcd4_Write_String("             ");
				Lcd4_Set_Cursor(3,7);
				Lcd4_Write_String("lF,sF,fO,"); //print actuators to action line
			}
			else if(temp < temp_low)
			{				
				lightOn();			
				EightyDegree();			
				turnOffFan();
				
				Lcd4_Set_Cursor(3,7);
				Lcd4_Write_String("             ");
				Lcd4_Set_Cursor(3,7);
				Lcd4_Write_String("lO,sO,fF");  //print actuators to action line
			}
			else
			{		
			    lightOff();				
			    //EightyDegree();			
				Lcd4_Set_Cursor(3,7);
				Lcd4_Write_String("             ");
				Lcd4_Set_Cursor(3,7);
				Lcd4_Write_String("lF,sO");  //print actuators to action line
			}
	}
	else if((lhtVal1<300) && (lhtVal2>800))//bright
	{
		//Lcd4_Set_Cursor(2,2);
		//Lcd4_Write_String("                  ");
		//Lcd4_Set_Cursor(2,2);
		//Lcd4_Write_String("Bright <350");
		/*Check temperature*/
		//	get DHT data
		dht_gettemperature(&temp);
		/*Display temp*/
		Lcd4_Set_Cursor(1,10);
		itoa(temp,buf2,10);
		Lcd4_Write_String(buf2);

		if(temp >= temp_high)
		{	
				lightOff();
				turnOnFan();
				//ZeroDegree();
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("             ");
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("lF,sF,fO"); //print actuators to action line
		}
		else if(temp < temp_low)
		{	
				lightOn();
				turnOffFan();
				EightyDegree();
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("             ");
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("lO,sO,fF");  //print actuators to action line
		}
		else
		{
		//Lcd4_Set_Cursor(2,2);
		//Lcd4_Write_String("                  ");
		//Lcd4_Set_Cursor(2,2);
		//Lcd4_Write_String("Norm 350<L<450");
		/*Check temperature*/
		//	get DHT data
		dht_gettemperature(&temp);
		/*Display temp*/
		Lcd4_Set_Cursor(1,10);
		itoa(temp,buf2,10);
		Lcd4_Write_String(buf2);
			lightOff();
			turnOffFan();
			//ZeroDegree();
		Lcd4_Set_Cursor(3,7);
		Lcd4_Write_String("             ");
		Lcd4_Set_Cursor(3,7);
		Lcd4_Write_String("lF,fF"); //print actuators to action line
			
		}
	}
		else //not bright or dark: in the middle
		{
		//Lcd4_Set_Cursor(2,2);
		//Lcd4_Write_String("                  ");
		//Lcd4_Set_Cursor(2,2);
		//Lcd4_Write_String("Bright <350");
		/*Check temperature*/
		//	get DHT data
		dht_gettemperature(&temp);
		/*Display temp*/
		Lcd4_Set_Cursor(1,10);
		itoa(temp,buf2,10);
		Lcd4_Write_String(buf2);

		if(temp >= temp_high)
		{
			lightOff();
			turnOnFan();
			//ZeroDegree();
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("             ");
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("lF,sF,fO"); //print actuators to action line
		}
		else if(temp < temp_low)
		{
			lightOn();
			turnOffFan();
			EightyDegree();
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("             ");
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("lO,sO,fF");  //print actuators to action line
		}
		else
		{
			//Lcd4_Set_Cursor(2,2);
			//Lcd4_Write_String("                  ");
			//Lcd4_Set_Cursor(2,2);
			//Lcd4_Write_String("Norm 350<L<450");
			/*Check temperature*/
			//	get DHT data
			dht_gettemperature(&temp);
			/*Display temp*/
			Lcd4_Set_Cursor(1,10);
			itoa(temp,buf2,10);
			Lcd4_Write_String(buf2);
			lightOff();
			turnOffFan();
			//ZeroDegree();
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("             ");
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("lF,fF"); //print actuators to action line
		}
	}
}


void checkSoil()
{
		//	water level loop
		int soillvl = analogSoilRead();
		int waterlvl = analogWaterRead();
		//Print Soil Lvl to screen, next to temp
		Lcd4_Set_Cursor(1,14);
		Lcd4_Write_String("SL:");
		Lcd4_Set_Cursor(1,16);
		itoa(soillvl,buffer,10);
		Lcd4_Write_String(buffer);

		//Take ADC val, and convert take just 1's place to get 0 or 1
		if(waterlvl >= 1000)
		waterlvl = 1;
		else
		waterlvl = 0;
		//Check water buoyancy first, then soil
		if(waterlvl == HIGH)
		{
			// LCD Print Water Level Normal	
			Lcd4_Set_Cursor(4,7);
			Lcd4_Write_String("             ");
		}
		else 
	    {
			// LCD Print Water Level Low
			// Turn Off Water Pump If Running	
			Lcd4_Set_Cursor(4,7);
		    Lcd4_Write_String("             ");
			Lcd4_Set_Cursor(4,7);
			Lcd4_Write_String("wC"); //Water critical
		
		}
		//Verify buoyancy sensor outputs 1, turn on pump if needed
		if(soillvl<=100 && (waterlvl==1))
		{
			Lcd4_Set_Cursor(3,0);
			Lcd4_Write_String("Action:");
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("             ");
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("pO");
			turnOnPump();
			_delay_ms(200);
			_delay_ms(200);
			_delay_ms(200);
			checkSoil();
			turnOffPump();
			Lcd4_Set_Cursor(3,0);
			Lcd4_Write_String("Action:");
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("             ");
			Lcd4_Set_Cursor(3,7);
			Lcd4_Write_String("pF");
		}
		else if(soillvl<=SOIL_CRIT&&waterlvl)
		{
				Lcd4_Set_Cursor(4,0);
				Lcd4_Write_String("Error:");
				Lcd4_Set_Cursor(4,7);
				Lcd4_Write_String("             ");
				Lcd4_Set_Cursor(4,7);
				Lcd4_Write_String("sC");
		        Lcd4_Set_Cursor(3,0);
				Lcd4_Write_String("Action:");
				Lcd4_Set_Cursor(3,7);
				Lcd4_Write_String("             ");
				Lcd4_Set_Cursor(3,7);
				Lcd4_Write_String("pO");
				turnOnPump();
				_delay_ms(200);
				_delay_ms(200);
				_delay_ms(200);
				checkSoil();		  			
				turnOffPump();
				Lcd4_Set_Cursor(3,0);
				Lcd4_Write_String("Action:");
				Lcd4_Set_Cursor(3,7);
				Lcd4_Write_String("             ");
				Lcd4_Set_Cursor(3,7);
				Lcd4_Write_String("pF");	
		}
	
	
		else if(soillvl<=SOIL_LOW && soillvl<=SOIL_MAX)
		{
		        turnOffPump();
				Lcd4_Set_Cursor(3,0);
				Lcd4_Write_String("Action:");
				Lcd4_Set_Cursor(3,7);
				Lcd4_Write_String("             ");
				Lcd4_Set_Cursor(3,7);
				Lcd4_Write_String("pF");			
		}
}


	

	
