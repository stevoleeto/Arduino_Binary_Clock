/*
* binaryClockDisplay.h - Library for accessing the 8x8 LED Matrix
* Created by Stephen Gilardi, 2015
*/

#ifndef BinaryClockDisplay_h
#define BinaryClockDisplay_h

#include "Arduino.h"

class BinaryClockDisplay{
	public:
		void setup(int LED1Blue, int LED1Green, int LED1Red, int LED2Blue, int LED2Green, int LED2Red, int LED3Blue, int LED3Green, int LED3Red, boolean *sleepPointer, boolean *digitalPointer, boolean *partyPointer);
		void playStartUp();
		void BCDTime(int theCurrentHour, int theCurrentMinute, int theCurrentSecond);
		void binaryTime(int theCurrentHour, int theCurrentMinute, int theCurrentSecond);
		void digitalTime(int theCurrentHour, int theCurrentMinute, int theCurrentSecond);
		void party();
		void sleep();

	private:
		void upDown();
		void writeWelcome();
		void smile();
		void turnColumnBinary(int columnToChange, int numToConvertToBinary);
		void showLEDBCD();
		void showLEDBinary();
		void showLEDDigital();
		void clearColumn(int columnToRemove);
		int rgbLEDs [3][3];
};
	
#endif