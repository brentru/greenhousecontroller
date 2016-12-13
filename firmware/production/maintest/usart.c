/*
 * usart.c
 *
 * Created: 11/30/2016 10:03:49 AM
 *  Author: brent
 */ 

 #include "usart.h"
 #include "alldecs.h"
 //char datastr;

 void USARTINIT()
 {
	 uint16_t ubbr = FOSC/16/9600-1;
	 // uart init
	 // set baud rate
	 UBRR0 = ubbr;
	 //UBRR0H = (ubbr >> 8);
	 //UBRR0L = (ubbr);
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

 //void USART_TRANSMIT_STRING(datastr)
 //{
	////	get length of string
	//int dstrlen = strlen (datastr);
	//while(dstrlen>1)
	//{
		//USART_TRANSMIT_DATA();
	//}
//
 //}


 //	transmit one char
 void USART_TRANSMIT_DATA (unsigned char data)
 {
	 // wait for empty transmit buf.
	 while (!(UCSR0A & (1<<UDRE0)));
	 UDR0 = data; // put data into buffer, send data
 }
