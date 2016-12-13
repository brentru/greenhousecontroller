/*
 * Hbridge.h
 *
 * Created: 11/28/2016 10:01:59 PM
 *  Author: dmaus
 */ 

#ifndef HBRIDGE_H_
#define HBRIDGE_H_


//setup ports
#define hbridge_DDR DDRD
#define hbridge_PORT PORTD
#define hbridge_ENA	 PORTD6 //Enable A is PD6
#define hbridge_ENB  PORTD5 //Enable B is PD5
#define hbridge_IN1  PORTD7 //Input 1 is PD7
#define hbridge_IN3  PORTD3 //Input 3 is PD3

//Define of high/low
#define HIGH 1
#define LOW 0

#endif /* HBRIDGE_H_ */