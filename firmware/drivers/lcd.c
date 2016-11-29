
#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif


#define D4 eS_PORTB2
#define D5 eS_PORTB3
#define D6 eS_PORTB4
#define D7 eS_PORTB5
#define RS eS_PORTC4
#define En eS_PORTC5

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include <avr/io.h>

int main(void)
{
	DDRB = 0xFF;
	DDRC = 0xFF;
	int i;
	Lcd4_Init();
	while(1)
	{
		Lcd4_Clear();
		Lcd4_Set_Cursor(0,0);
		Lcd4_Write_String("string1");
		_delay_ms(2000);
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String("string2");
		_delay_ms(2000);
		Lcd4_Set_Cursor(4,2);
		Lcd4_Write_String("string3");
		_delay_ms(2000);
	}
}