/*
* binaryClockDisplay.h - Library for accessing the 8x8 LED Matrix
* Created by Stephen Gilardi, July 24, 2015
*/

#ifndef BinaryClockDisplay_h
#define BinaryClockDisplay_h

#include "Arduino.h"

class BinaryClockDisplay{
	public:
		BinaryClockDisplay(int LED1Blue, int LED1Green, int LED1Red, int LED2Blue, int LED2Green, int LED2Red, int LED3Blue, int LED3Green, int LED3Red);
		void playStartUp();
		void displayBCDTime(int theCurrentHour, int theCurrentMinute, int theCurrentSecond);
		void displayBinaryTime(int theCurrentHour, int theCurrentMinute, int theCurrentSecond);
		void displayDigitalTime(int theCurrentHour, int theCurrentMinute, int theCurrentSecond);
		void displayparty();

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