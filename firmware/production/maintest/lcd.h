//Referenced from electroSome 
//and http://www.raspberrypi-spy.co.uk/2012/08/20x4-lcd-module-control-using-python/
/*
	The LCD wiring configuration is as follows:
	four-bit operation was used
	#1 GND
	#2 5V
	#3 GND
	#4 RS PC4
	#5 R/W (Grounded)
	#6 EN PC5
	#7 DB0 -NOT USED
	#8 DB1 -NOT USED
	#9 DB2 -NOT USED
	#10 DB3 -NOT USED
	#11 DB4 PB2
	#12 DB5 PB3
	#13 DB6 PB4
	#14 DB7 PB5
	#15 DB8 Backlight (5V)
	#16 DB9 Backlight (GND)
*/
#include <avr/io.h>
#include <avr/delay.h>
#ifndef LCD_H_
#define LCD_H_
/*328P pin mapping to LCD pins*/
#define PORTB2 10
#define PORTB3 11
#define PORTB4 12
#define PORTB5 13
#define PORTC4 18
#define PORTC5 19


#ifndef D4
#define D4 PORTB2
#define D5 PORTB3
#define D6 PORTB4
#define D7 PORTB5
#endif

#define RS PORTC4
#define EN PORTC5

/*Include Statements*/

/*Define some device constants*/
#define LCD_WIDTH 20

/*Define for RAM Address of each line*/
#define LCD_L1 0x80 
#define LCD_L2 0XC0
#define LCD_L3 0x94
#define LCD_L4 0xD4

#endif /* LCD_H_ */
