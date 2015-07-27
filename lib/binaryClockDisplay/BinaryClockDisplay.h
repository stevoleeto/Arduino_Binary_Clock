/*
* binaryClockDisplay.h - Library for accessing the 8x8 LED Matrix
* Created by Stephen Gilardi, July 24, 2015
*/
#ifndef BinaryClockDisplay_h
#define BinaryClockDisplay_h

#include "Arduino.h"

class BinaryClockDisplay{
	public:
		BinaryClockDisplay(int, int, int, int, int, int, int, int, int);
		void playStartUp();
		void smile();
		void clearColumn(int);
		void showLEDBCD();
		void showLEDBinary();
		void showLEDDigital();
		void displayBCDTime(int, int, int);
		void displayBinaryTime(int, int, int);
		void displayDigitalTime(int, int, int);
		void displayparty();

	private:
		void upDown();
		void writeWelcome();
		void turnColumnBinary(int, int);
		int rgbLEDs [3][3];
};
	
#endif