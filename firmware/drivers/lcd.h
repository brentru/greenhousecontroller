//Referenced from electroSome 
//and http://www.raspberrypi-spy.co.uk/2012/08/20x4-lcd-module-control-using-python/
/*
	The LCD wiring is as follows:
	#1 GND
	#2 5V
	#3 GND
	#4 RS C4
	#5 R/W (Grounded)
	#6 EN C5
	#7 DB0 -NOT USED
	#8 DB1 -NOT USED
	#9 DB2 -NOT USED
	#10 DB3 -NOT USED
	#11 DB4
	#12 DB5
	#13 DB6
	#14 DB7
	#15 DB8 Backlight (5V)
	#16 DB9 Backlight (GND)
*/

/*328P pin mapping to LCD pins*/
#define eS_PORTB2 10
#define eS_PORTB3 11
#define eS_PORTB4 12
#define eS_PORTB5 13
#define eS_PORTC4 18
#define eS_PORTC5 19


#ifndef D4
#define D4 eS_PORTB2
#define D5 eS_PORTB3
#define D6 eS_PORTB4
#define D7 eS_PORTB5
#endif

#define RS eS_PORTC4
#define EN eS_PORTC5

/*Include Statements*/
#include<util/delay.h>

/*Define some device constants*/
#define LCD_WIDTH 20

/*Define for RAM Address of each line*/
#define LCD_L1 0x80 
#define LCD_L2 0XC0
#define LCD_L3 0x94
#define LCD_L4 0xD4
void pinChange(int a, int b)
{
	if(b == 0)
	{
	
	    if(a == eS_PORTB2)
		  PORTB &= ~(1<<PORTB2);  
		else if(a == eS_PORTB3)
		  PORTB &= ~(1<<PORTB3);  
		else if(a == eS_PORTB4)
		  PORTB &= ~(1<<PORTB4);  
		else if(a == eS_PORTB5)
		  PORTB &= ~(1<<PORTB5);  
        else if(a == eS_PORTC4)
          PORTC &= ~(1<<PORTC4);		
		else if(a == eS_PORTC5)
		  PORTC &= ~(1<<PORTC5);          
	}
	else
	{
		if(a == eS_PORTB2)
		  PORTB |= (1<<PORTB2);
		else if(a == eS_PORTB3)
		  PORTB |= (1<<PORTB3);
		else if(a == eS_PORTB4)
		  PORTB |= (1<<PORTB4);
		else if(a == eS_PORTB5)
		  PORTB |= (1<<PORTB5);
		else if(a == eS_PORTC4)
		  PORTC |= (1<<PORTC4);  
		else if(a == eS_PORTC5)
		  PORTC |= (1<<PORTC5);
	}
}


//LCD 4 Bit Interfacing Functions

void Lcd4_Port(char a)
{
	if(a & 1)
	pinChange(D4,1);
	else
	pinChange(D4,0);
	
	if(a & 2)
	pinChange(D5,1);
	else
	pinChange(D5,0);
	
	if(a & 4)
	pinChange(D6,1);
	else
	pinChange(D6,0);
	
	if(a & 8)
	pinChange(D7,1);
	else
	pinChange(D7,0);
}
void Lcd4_Cmd(char a)
{
	pinChange(RS,0);             // => RS = 0
	Lcd4_Port(a);
	pinChange(EN,1);            // => E = 1
	_delay_ms(1);
	pinChange(EN,0);             // => E = 0
	_delay_ms(1);
}

void Lcd4_Clear()
{
	Lcd4_Cmd(0);
	Lcd4_Cmd(1);
}

void Lcd4_Set_Cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
	{
		temp = 0x80 + b;
		z = temp>>4;
		y = (0x80+b) & 0x0F;
		Lcd4_Cmd(z);
		Lcd4_Cmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b;
		z = temp>>4;
		y = (0xC0+b) & 0x0F;
		Lcd4_Cmd(z);
		Lcd4_Cmd(y);
	}
	else if(a == 3)
	{
		temp = 0x94 + b;
		z = temp>>4;
		y = (0x94+b) & 0x0F;
		Lcd4_Cmd(z);
		Lcd4_Cmd(y);
	}
	else if(a == 4)
	{
		temp = 0xD4 + b;
		z = temp>>4;
		y = (0xD4+b) & 0x0F;
		Lcd4_Cmd(z);
		Lcd4_Cmd(y);
	}	
}

void Lcd4_Init()
{
	Lcd4_Port(0x00);
	_delay_ms(20);
	/*Datasheet reset process*/
	Lcd4_Cmd(0x33);
	_delay_ms(20);
	Lcd4_Cmd(0x32);
	_delay_ms(20);
	Lcd4_Cmd(0x06);
	Lcd4_Cmd(0x0C);
	Lcd4_Cmd(0x28);
	Lcd4_Cmd(0x01);
}

void Lcd4_Write_Char(char a)
{
	char temp,y;
	temp = a&0x0F;
	y = a&0xF0;
	pinChange(RS,1);             // => RS = 1
	Lcd4_Port(y>>4);             //Data transfer
	pinChange(EN,1);
	_delay_ms(1);
	pinChange(EN,0);
	_delay_ms(1);
	Lcd4_Port(temp);
	pinChange(EN,1);
	_delay_ms(1);
	pinChange(EN,0);
	_delay_ms(1);
}

void Lcd4_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	Lcd4_Write_Char(a[i]);
}

void Lcd4_Shift_Right()
{
	Lcd4_Cmd(0x01);
	Lcd4_Cmd(0x0C);
}

void Lcd4_Shift_Left()
{
	Lcd4_Cmd(0x01);
	Lcd4_Cmd(0x08);
}
