/*
 * SoilProbe.c
 *
 * Created: 12/6/2016 8:02:39 PM
 *  Author: dmaus
 */ 

 #include "SoilProbe.h"
 #include <avr/io.h>

 float readVH400(int analogPin) {
	 // This function returns Volumetric Water Content by converting the analogPin value to voltage
	 // and then converting voltage to VWC using the piecewise regressions provided by the manufacturer
	 // at http://www.vegetronix.com/Products/VH400/VH400-Piecewise-Curve.phtml
	 
	 // NOTE: You need to set analogPin to input in your setup block
	 //   ex. pinMode(<analogPin>, INPUT);
	 //   replace <analogPin> with the number of the pin you're going to read from.
	 
	 // Read value and convert to voltage
	 uint16_t sensor1DN = analogSoilRead();
	 float sensorVoltage = sensor1DN*(3.0 / 1023.0);
	 float VWC;
	 
	 // Calculate VWC
	 if(sensorVoltage <= 1.1) {
		 VWC = 10*sensorVoltage-1;
		 } else if(sensorVoltage > 1.1 && sensorVoltage <= 1.3) {
		 VWC = 25*sensorVoltage-17.5;
		 } else if(sensorVoltage > 1.3 && sensorVoltage <= 1.82) {
		 VWC = 48.08*sensorVoltage-47.5;
		 } else if(sensorVoltage > 1.82) {
		 VWC = 26.32*sensorVoltage-7.89;
	 }
	 return(VWC);
 }
