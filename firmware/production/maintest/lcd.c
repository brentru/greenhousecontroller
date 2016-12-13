#include "lcd.h"

void pinChange(int a, int b)
{
	if(b == 0)
	{
		
		if(a == PORTB2)
		PORTB &= ~(1<<PORTB2);
		else if(a == PORTB3)
		PORTB &= ~(1<<PORTB3);
		else if(a == PORTB4)
		PORTB &= ~(1<<PORTB4);
		else if(a == PORTB5)
		PORTB &= ~(1<<PORTB5);
		else if(a == PORTC4)
		PORTC &= ~(1<<PORTC4);
		else if(a == PORTC5)
		PORTC &= ~(1<<PORTC5);
	}
	else
	{
		if(a == PORTB2)
		PORTB |= (1<<PORTB2);
		else if(a == PORTB3)
		PORTB |= (1<<PORTB3);
		else if(a == PORTB4)
		PORTB |= (1<<PORTB4);
		else if(a == PORTB5)
		PORTB |= (1<<PORTB5);
		else if(a == PORTC4)
		PORTC |= (1<<PORTC4);
		else if(a == PORTC5)
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
	///////////// Reset process from datasheet /////////
	Lcd4_Cmd(0x03);
	_delay_ms(5);
	Lcd4_Cmd(0x03);
	_delay_ms(11);
	Lcd4_Cmd(0x03);
	/////////////////////////////////////////////////////
	Lcd4_Cmd(0x02);
	Lcd4_Cmd(0x02);
	Lcd4_Cmd(0x08);
	Lcd4_Cmd(0x00);
	Lcd4_Cmd(0x0C);
	Lcd4_Cmd(0x00);
	Lcd4_Cmd(0x06);
	//Lcd4_Port(0x01); //Clears entire display
//
	/////////////// Reset process from datasheet /////////
	//Lcd4_Cmd(0x03); //sets DDRAM, returns display
	//_delay_ms(2);
	//Lcd4_Cmd(0x05);//Sets cursor move direction/display shift
	//_delay_ms(1);
	//Lcd4_Cmd(0x03);
	///////////////////////////////////////////////////////
	//Lcd4_Cmd(0x02);
	//Lcd4_Cmd(0x02);
	//Lcd4_Cmd(0x08);
	//Lcd4_Cmd(0x00);
	//Lcd4_Cmd(0x0C);
	//Lcd4_Cmd(0x00);
	//Lcd4_Cmd(0x06);
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