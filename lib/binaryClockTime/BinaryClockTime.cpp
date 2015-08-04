/*
* BinaryClockTime.h - Library for accessing the DS3231 for timekeeping.
* Created by Stephen Gilardi, 2015
*/
#include "BinaryClockTime.h"
#include "Arduino.h"
#include "Wire.h"

#define DS3231_I2C_ADDRESS 0x68

//Variables used to store current time since last "updateTime()" function call
int currentSecond;
int currentMinute;
int currentHour;
int currentDay;
int currentDate;
int currentMonth;
int currentYear;


/*
* Function: Modify Time
* Type: Setter
*
* Purpose: 
*         - Uses byte parameters for second, minute, hour, day, date, month, and year
*         - Sets the DS3231 to have the same time as  determined by the sent parameters.
*/
void BinaryClockTime::modifyTime(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year){
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

/*
* Function: Update Time
* Type: Getter
*
* Purpose: 
*         - Retrieves current second, minute, hour, day, date, month, and year values from the DS3231 and stores locally
*/
void BinaryClockTime::updateTime(){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  currentSecond = bcdToDec(Wire.read() & 0x7f); 
  currentMinute = bcdToDec(Wire.read());
  currentHour = bcdToDec(Wire.read() & 0x3f);
  if(currentHour > 12){
    currentHour = currentHour - 12;
  }
  currentDay = bcdToDec(Wire.read());
  currentDate = bcdToDec(Wire.read());
  currentMonth = bcdToDec(Wire.read());
  currentYear = bcdToDec(Wire.read());
}

/*
* Function: Get Second
* Type: Getter
*
* Purpose: 
*         - Returns the current second value.
*         - This value is equal to the time called during the latest updateTime() function call.
*/
int BinaryClockTime::getSecond(){
  return currentSecond;
}

/*
* Function: Get Minute
* Type: Getter
*
* Purpose: 
*         - Returns the current minute value.
*         - This value is equal to the time called during the latest updateTime() function call.
*/
int BinaryClockTime::getMinute(){
  return currentMinute;
}

/*
* Function: Get Hour
* Type: Getter
*
* Purpose: 
*         - Returns the current hour value.
*         - This value is equal to the time called during the latest updateTime() function call.
*/
int BinaryClockTime::getHour(){
  return currentHour;
}

/*
* Function: Get Day
* Type: Getter
*
* Purpose: 
*         - Returns the current day value.
*         - This value is equal to the time called during the latest updateTime() function call.
*/
int BinaryClockTime::getDay(){
  return currentDay;
}

/*
* Function: Get Date
* Type: Getter
*
* Purpose: 
*         - Returns the current date value.
*         - This value is equal to the time called during the latest updateTime() function call.
*/
int BinaryClockTime::getDate(){
  return currentDate;
}

/*
* Function: Get Month
* Type: Getter
*
* Purpose: 
*         - Returns the current month value.
*         - This value is equal to the time called during the latest updateTime() function call.
*/
int BinaryClockTime::getMonth(){
  return currentMonth;
}

/*
* Function: Get Year
* Type: Getter
*
* Purpose: 
*         - Returns the current year value.
*         - This value is equal to the time called during the latest updateTime() function call.
*/
int BinaryClockTime::getYear(){
  return currentYear;
}

/*
* Function: Decimal to Binary
* Type: Math
*
* Purpose: 
*         - Takes in a decimal value (byte) as a parameter
*         - Converts and returns a binary value (byte)
*/
byte BinaryClockTime::decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}

/*
* Function: Binary to Decimal
* Type: Math
*
* Purpose: 
*         - Takes in a binary value (byte) as a parameter
*         - Converts and returns a decimal value (byte)
*/
byte BinaryClockTime::bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}