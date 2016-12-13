/*
 * Hbridge.h
 *
 * Created: 11/28/2016 10:01:59 PM
 *  Author: dmaus
 */ 
#ifndef HBRIDGE_H_
#define HBRIDGE_H_

#include <stdio.h>
#include <avr/io.h>

//setup ports
#define hbridge_DDR DDRD
#define hbridge_PORT PORTD
#define hbridge_ENA	 PORTD6
#define hbridge_ENB  PORTD5
#define hbridge_IN1  PORTD7
#define hbridge_IN3  PORTD3

//Define of high/low
#define HIGH 1
#define LOW 0

#endif /* HBRIDGE_H_ */