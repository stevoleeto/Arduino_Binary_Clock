/*
* binaryClockTime.h - Library for accessing the DS3231 for timekeeping.
* Created by Stephen Gilardi, 2015
*/

#ifndef BinaryClockTime_h
#define BinaryClockTime_h

#include "Arduino.h"

class BinaryClockTime{
	public:
		void modifyTime(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);
		void updateTime();
		int getSecond();
		int getMinute();
		int getHour();
		int getDay();
		int getDate();
		int getMonth();
		int getYear();

	private:
		byte decToBcd(byte val);
		byte bcdToDec(byte val);
		int currentSecond;
		int currentMinute;
		int currentHour;
		int currenDay;
		int currentDate;
		int currentMonth;
		int currenYear;
};
	
#endif