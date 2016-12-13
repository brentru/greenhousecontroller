//	chip-specific defines 
#define FOSC 16000000 //clk speed for 16mHz
#define BAUD 9600 //baud rate transmit
#define MYUBBR FOSC/16/BAUD-1

/*	STORED VALUES: MEASURED & EXPECTED VALS */
//	water lvl sensor
#define WLEVEL_MAX 3.3
#define WLEVEL_MIN 0.8
//	photoresistor
#define PHOTOVAL 450
//	soil sensor (expecting volts)
//	range: 65cm-90cm
#define SOIL_MAX 500
//	15cm
#define SOIL_LOW 100
//	0cm
#define SOIL_CRIT 50
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


/*	includes	*/
//	c-specific includes 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
//	project-specific includes
#include "ADC.h"
#include "dht.h"
#include "Hbridge.h"
#include "lcd.h"
#include "relay.h"
#include "servo.h"
#include "SoilProbe.h"
#include "usart.h"


