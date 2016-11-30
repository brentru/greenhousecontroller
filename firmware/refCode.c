Finalized Code (.ino/.c)

/*
 *  Group: Brent Rubell, Jarred Trottier, David Austin 
 *  Project: Automated Greenhouse System
 *  Class: ECE388
 *  Code: Prototype code for project
 */

//  libs  
#include <avr/interrupt.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// pins+vars
byte dat[5];
Servo blindServo;
int photoPin = A1;
int pVal = 0;
int servoPos = 0;
int DHpin = 8;
int redpin = 16;  // select the pin for the red LED
int bluepin = 17; // select the pin for the blue LED
int greenpin = 18;  // select the pin for the green LED
int motorF = 7;
int motorR = 6;

//  lcd setup 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int secondsSinceReset = 0;

// used by start_test, do NOT modify
byte read_data () {
  byte data;
  for (int i = 0; i < 8; i ++) {
    if (digitalRead (DHpin) == LOW) {
      while (digitalRead (DHpin) == LOW); // wait for 50us
      delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'
      if (digitalRead (DHpin) == HIGH)
        data |= (1 << (7-i)); // high front and low in the post
      while (digitalRead (DHpin) == HIGH); // data '1 ', wait for the next one receiver
     }
  }
return data;
}
// call to get humid/tmp values 
// returns to DAT[0] and DAT[2]
void start_test () {
  Serial.println("start test began");
  digitalWrite (DHpin, LOW); // bus down, send start signal
  delay (30); // delay greater than 18ms, so DHT11 start signal can be detected
 
  digitalWrite (DHpin, HIGH);
  delayMicroseconds (40); // Wait for DHT11 response
  pinMode (DHpin, INPUT);
  if ((digitalRead (DHpin) == HIGH)||(digitalRead (DHpin) == LOW));
  delayMicroseconds (80); // DHT11 response, pulled the bus 80us
  Serial.println("DEBUG HIT");
  
 
  for (int i = 0; i < 4; i ++) // receive temperature and humidity data, the parity bit is not considered
    dat[i] = read_data ();
 
  pinMode (DHpin, OUTPUT);
  digitalWrite (DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal
  Serial.println("start test finished");

}
//  call to get updated pVal
void getPhotoVal(){
  // pVal is >= 450 -> Lights on
  pVal = analogRead(photoPin);
}
// rgb led setup
void rgbSetup(){
  pinMode (redpin, OUTPUT);
  pinMode (bluepin, OUTPUT);
  pinMode (greenpin, OUTPUT);
}
//** SETUP **//
void setup(void) {
    //  enable global interrupts
    sei();  
    blindServo.write(servoPos);
    rgbSetup();
    // hum./temp. sensor
    pinMode (DHpin, OUTPUT);    
    //  lcd
    lcd.begin(20, 4);
    // servo motor 
    blindServo.attach(9);
    // dc motor  
    pinMode (motorF,OUTPUT);
    pinMode (motorR,OUTPUT);
    //  listen at 9600baud
    Serial.begin(9600);
}
void updatePVal(){
    getPhotoVal();
    Serial.println(pVal);
}
//*** MAIN LOOP ***//
void loop() 
{
  //  get pval
  getPhotoVal();
  // print pval to serial
  Serial.println(pVal);
  //  get humid/tmp
  start_test();
  //  printing to lcd
  lcd.setCursor(0, 0);
  lcd.print("Set 35 C;Temp C: ");
  lcd.setCursor(16,0); 
  lcd.print(dat[0], DEC);
  lcd.setCursor(0, 1);
  lcd.print("Humid %: ");
  lcd.setCursor(10,1);
  lcd.print(dat[2], DEC);
  lcd.setCursor(0,2);
  lcd.print("Light val: ");
  lcd.setCursor(12,2);
  lcd.print(pVal,DEC);
  lcd.setCursor(0,3);
  lcd.print("Servo pos: ");
  lcd.setCursor(12,3);
  lcd.print(servoPos,DEC);
  //  delay for lcd refresh
  delay(800);
  
  if(pVal <= 350)
  {
    if (servoPos != 0)
    {
      servoPos = servoPos-1;
      blindServo.write(servoPos);
      delay(15);
     }
      // turn led off
      digitalWrite(redpin,0);
      digitalWrite(bluepin,0);
      digitalWrite(greenpin,0); 
      
   }
  else
  {
    if(servoPos != 180)
    {
      servoPos=servoPos+1;
      blindServo.write(servoPos);
      delay(15);
      if(servoPos >=180)
      {
        blindServo.write(180);
        // turn red+blue leds on
        digitalWrite(greenpin,0);
        digitalWrite(bluepin,255);
        digitalWrite(redpin,255);
      } 
    }
  }
  //  temp/humidity dc motor interface
  if ((dat[0] >= 37)||(dat[2] >=25))
  {
    // fan runs in reverse to exhaust air
    digitalWrite(motorR,LOW);
    digitalWrite(motorF,HIGH);
  }
  else if((dat[0] < 37)||(dat[2] <25))
  {
    //  fan acts as an intake
    digitalWrite(motorF,LOW);
    digitalWrite(motorR,HIGH);
  }
}

Segments of Arduino Code Referenced for Program

Button Press

//KY016 3-color LED module
int redpin = 11; // select the pin for the red LED
int bluepin = 10; // select the pin for the blue LED
int greenpin = 9 ;// select the pin for the green LED
int buttonPin = 2;//button pin
int buttonState=1;
int val;
void setup () 
{
  pinMode (redpin, OUTPUT);
  pinMode (bluepin, OUTPUT);
  pinMode (greenpin, OUTPUT);
  pinMode (buttonPin,INPUT);
  Serial.begin (9600);
}
void loop ()
{
  buttonState=digitalRead(buttonPin);
    if (buttonState == HIGH) {
    // turn LED on:
      analogWrite (11, 255);
      analogWrite (10, 255);
      analogWrite (9, 255);
    } 
    else {
      analogWrite (11, 0);
      analogWrite (10, 0);
      analogWrite (9, 0);

  }
}
**LED Code was referenced from TkkrLab (Adruino KY-016 3-color LED Module)**



LED Cycle Through Colors

Simple demo without cables
DSC 2994small.ky-016.JPG
Video with random blink.
Example code random
/*
 Demo for KY-009 and KY-016.
 Plug the board directly in your Arduino board without using
 cables. If you turn the board 180° you should use pin 12 as
 ground and swap the green and blue pin.
 The demo will over time cycle through all the 16777216
 different colors which can be made with 3x8bit.
 */
int groundpin = 8; // write 0 to get ground
int greenpin = 9; // select the pin for the green LED
int redpin = 10; // select the pin for the red LED
int bluepin = 11; // select the pin for the blue LED
 
void setup () {
  pinMode (redpin, OUTPUT);
  pinMode (greenpin, OUTPUT);
  pinMode (bluepin, OUTPUT);
  pinMode (groundpin, OUTPUT);
  digitalWrite (groundpin, LOW);
}
 
void loop () {
  analogWrite (redpin,   random(255));
  analogWrite (greenpin, random(255));
  analogWrite (bluepin,  random(255));
  delay (300);
}
**Code used was from TkkrLab**



LCD
/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}
**Code used was from Arduino built in examples*


Temp/Humidity Sensor Triggers Servo

//KY015 DHT11 Temperature and humidity sensor 
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int DHpin = 7;
int pos = 0;    // variable to store the servo position
byte dat [5];

byte read_data () {
  byte data;
  for (int i = 0; i < 8; i ++) {
    if (digitalRead (DHpin) == LOW) {
      while (digitalRead (DHpin) == LOW); // wait for 50us
      delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'
      if (digitalRead (DHpin) == HIGH)
        data |= (1 << (7-i)); // high front and low in the post
      while (digitalRead (DHpin) == HIGH); // data '1 ', wait for the next one receiver
     }
  }
return data;
}
 
void start_test () {
  digitalWrite (DHpin, LOW); // bus down, send start signal
  delay (30); // delay greater than 18ms, so DHT11 start signal can be detected
 
  digitalWrite (DHpin, HIGH);
  delayMicroseconds (40); // Wait for DHT11 response
 
  pinMode (DHpin, INPUT);
  while (digitalRead (DHpin) == HIGH);
  delayMicroseconds (80); // DHT11 response, pulled the bus 80us
  if (digitalRead (DHpin) == LOW);
  delayMicroseconds (80); // DHT11 80us after the bus pulled to start sending data
 
  for (int i = 0; i < 4; i ++) // receive temperature and humidity data, the parity bit is not considered
    dat[i] = read_data ();
 
  pinMode (DHpin, OUTPUT);
  digitalWrite (DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal
}
 
void setup () {
  Serial.begin (9600);
  pinMode (DHpin, OUTPUT);
    myservo.attach(9);  // attaches the servo on pin 9 to the servo object

}
 
void loop () {
  start_test ();
  Serial.print ("Current humdity =");
  Serial.print (dat [0], DEC); // display the humidity-bit integer;
  Serial.print ('.');
  Serial.print (dat [1], DEC); // display the humidity decimal places;
  Serial.println ('%');
  Serial.print ("Current temperature =");
  Serial.print (dat [2], DEC); // display the temperature of integer bits;
  Serial.print ('.');
  Serial.print (dat [3], DEC); // display the temperature of decimal places;
  Serial.println ('C');
  delay (700);
  while(dat[1] > 40)
  {
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);  
    pos=0;                    // waits 15ms for the servo to reach the position
  }
  }  
}
**Temperature code from TkkrLab, combined with Arduino built in Servo Code**







Groups Integration of Humidity Sensor with LCD

/* 
Name: LCD Greenhouse Test
Desc: Uses 16x2 TC2004A LCD to display humidity data 
Authors: Brent Rubell, Jarred Trottier, David Austin
References: 
- https://tkkrlab.nl/wiki/Arduino_KY-015_Temperature_and_humidity_sensor_module
- https://www.arduino.cc/en/Tutorial/LiquidCrystalDisplay
*/

//	includes
#include <LiquidCrystal.h>

//	variables
int DHpin = 8; 
byte dat [4]; 
LiquidCrystal lcd(12,11,5,4,3,2);
//	read data from DHPin 
byte read_data () {
  byte data;
  for (int i = 0; i < 4; i ++) {
    if (digitalRead (DHpin) == LOW) {
      while (digitalRead (DHpin) == LOW); // wait for 50us
      delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'
      if (digitalRead (DHpin) == HIGH)
        data |= (1 << (7-i)); // high front and low in the post
      while (digitalRead (DHpin) == HIGH); // data '1 ', wait for the next one receiver
     }
  }
return data;
}

void LCDLOOP(){
  lcd.noDisplay();
  delay(500);
  lcd.Display();
  lcd.setCursor(0,1);
}

void getVals(){
	digitalWrite(DHpin, LOW);
	delay(30);
	digitalWrite(DHpin,HIGH);
	delayMicroseconds(40);
	pinMode (DHpin, INPUT);
    while (digitalRead (DHpin) == HIGH);
    delayMicroseconds (80); // DHT11 response, pulled the bus 80us
    if (digitalRead (DHpin) == LOW);
    delayMicroseconds (80); // DHT11 80us after the bus pulled to start sending data
 
   for (int i = 0; i < 2; i ++) // receive temperature and humidity data, the parity bit is not considered
    dat[i] = read_data ();
 
   pinMode (DHpin, OUTPUT);
   digitalWrite (DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal
}

void setup(){
  Serial.begin(9600); 
  pinMode(DHpin, OUTPUT);
  lcd.begin(16,2);
  lcd.print("lcd initialized");
}

void loop(){
   LCDLOOP();
   getVals();
   lcd.print("humidity: %i", dat[0]);
   LCDLOOP();
   lcd.print("humidity: %i", dat[1]);  
}


Photoval and LED

int photoPin = A1;
int redpin = 11; // select the pin for the red LED
int bluepin = 10; // select the pin for the blue LED
int greenpin = 9 ;// select the pin for the green LED
int photoVal = 0;

void setRGB(){
  pinMode (redpin, OUTPUT);
  pinMode (bluepin, OUTPUT);
  pinMode (greenpin, OUTPUT);
  Serial.begin(9600);
}

void setup() {
  // put your setup code here, to run once:
  setRGB();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  photoVal=analogRead(photoPin);
  Serial.println(photoVal);
  if(photoVal >= 450)
  {
      analogWrite (11, 255);
      analogWrite (10, 255);
      analogWrite (9, 255);
  }
  else
  {
      analogWrite (11, 0);
      analogWrite (10, 0);
      analogWrite (9, 0);
  }
}
**Photo-resistor and LED code referenced from TkkrLab**

Servo

/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
**Code referenced from Arduino Servo Test Code**



