

/********************************************************************************************
Binary Clock

This code was written for the following pieces of hardware:
   Arduino UNO
   Adafruit 8x8 LED matrix
   DS3231
   RGB LED x 3
   Tactile Button x 2

Written by Stephen Gilardi, 2015
********************************************************************************************/

/*
* Includes necessary Libraries
*
* Note: Adafruit GFX and LEDBackpack libraries can be downloaded at Adafruit.com
*/
#include <Wire.h>
#include <BinaryClockDisplay.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <BinaryClockDisplay.h>

/*
* Defines the addresses for the D23231 and Adafruit 8x8 matrix
*
* DS3231 Address              -> 0x68
* Adafruit 8x8 Matrix Address -> 0x70
*/
#define DS3231_I2C_ADDRESS 0x68

/*
* Defines the two buttons 
*
* Switch Button: Digital Pin #2
* Update Button: Digital Pin #4
*/
#define switchButton 2
#define powerButton 4

/*
* Defines the three RGB LEDs
*
* RGB LED #1:
*    Blue:  Digital Pin #11 (PWM PIN)
*    Green: Digital Pin #12
*    Red:   Digital Pin #13
*
* RGB LED #2:
*    Blue:  Digital Pin #9 (PWM PIN)
*    Green: Digital Pin #8
*    Red:   Digital Pin #10
*
* RGB LED #3:
*    Blue:  Digital Pin #5 (PWM PIN)
*    Green: Digital Pin #6
*    Red:   Digital Pin #7
*/
#define red1 13
#define green1 12
#define blue1 11
#define red2 10
#define green2 8
#define blue2 9
#define red3 7
#define green3 6
#define blue3 5

/*
* Initalizes the four logic controlling booleans.
* Used to switch between different modes.
*
* Binary Counter Time
* Binary Time
* Digital Time
* Party Mode
* Sleep Mode
*/
boolean bcd = false;
boolean binary = false;
boolean digital = false;
boolean party = false;
boolean sleep = false;

/*
* Initalizes the seven integer values representing the current time and date.
* Used to switch between different modes.
*
* Second
* Minute
* Hour
* Day of the Week
* Day of the Month
* Month
* Year
*/
int currSecond = 99;
int currMinute = 99;
int currHour = 99;
int currDayOfWeek = 99;
int currDayOfMonth = 99;
int currMonth = 99;
int currYear = 99;


   
////////////////////////////////////////////////////////////////////////////////
//////////////////////// Set up ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// Arduino Functions ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

BinaryClockDisplay display;

/*
* Function: Setup
* Type: Arduino Functions
*
* Purpose: 
*         - Mandatory Arduino Function required to perform initial setup.
*         - Attaches necessary interrupts for Switch and Power Buttons.
*         - Begins Serial Communications and prints initial program information.
*         - Begins the Wire Library and connects the LED Matrix to it.
*         - Initalizes the LED Matrix to the appropiate settings.
*         - Initalizes the 9 digital pins for the 3 RBG LEDs to be outputs.
*         - Calls the Startup Function.
*         - Clears the LED Matrix.
*         - Initalizes BCD to be the default time mode.
*/
void setup() {
  
  attachInterrupt(0, switchMode, RISING);
  attachInterrupt(1, powerMode, RISING);
  Serial.begin(9600);
  Serial.println("Matrix Test\nStephen Gilardi\n7/13/2015");
  Serial.println("Setup()");  

  Wire.begin();
  display.setup(blue1, green1, red1, blue2, green2, red2, blue3, green3, red3, &sleep, &digital, &party);

  //seconds, minutes, hours, day, date, month, year
  //setDS3231time(00,41,20,3,21,7,15);
  //display.playStartUp();
  bcd = true;
  
}

/*
* Function: Loop
* Type: Arduino Functions
*
* Purpose: 
*         - Mandatory Arduino Function required to perform continuous use.
*         - Checks to see if sleep mode has been activiated:
*             - If TRUE, clears the display and waits until sleep mode has been deactivated.
*             - If FALSE, calls the Update Time Function and checks for other modes.
*         - Checks to see if bcd mode has been activiated:
*             - If TRUE, turns on only the first LED on to blue (dim) and calls the Display BCD Time Function.
*         - Checks to see if binary mode has been activiated:
*             - If TRUE, turns on only the second LED on to blue (dim) and calls the Display Binary Time Function.
*         - Checks to see if digital mode has been activiated:
*             - If TRUE, turns on only the third LED on to blue (dim) and calls the Display Digital Time Function.
*         - Checks to see if party mode has been activiated:
*             - If TRUE, calls the Display Party Function
*         - Delays program 1/4 of a second to prevent overloading.
*/
void loop() {
  if(sleep){
    display.sleep();
  }
  else{
    updateTime();
    if(bcd){
      display.showLEDBCD();
      display.BCDTime(currHour, currMinute, currSecond); 
    }
    
    else if(binary){
      display.showLEDBinary();
      display.binaryTime(currHour, currMinute, currSecond);
    }
    
    else if(digital){
      display.showLEDDigital();
      display.digitalTime(currHour, currMinute, currSecond);
    }
    
    else if(party){
      display.party(); 
    }
    delay(250);
  }
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Interrupt Functions ///////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*
* Function: Switch Mode
* Type: Interrupt Functions
*
* Purpose: 
*         - Called when the Switch Mode button has been pressed, used to switch between modes.
*         - Serial Prints "Mode Switched!" 
*         - Checks to see if bcd mode is currently activiated:
*             - If TRUE, deactivates bcd mode, activates binary mode, and Serial Prints "Binary Mode!".
*         - Checks to see if binary mode is currently activiated:
*             - If TRUE, deactivates binary mode, activates digital mode, and Serial Prints "Digital Mode!".
*         - Checks to see if digital mode is currently activiated:
*             - If TRUE, deactivates digital mode, activates party mode, and Serial Prints "Party Mode!".
*         - Checks to see if party mode is currently activiated:
*             - If TRUE, deactivates party mode, activates bcd mode and Serial Prints "BCD Mode!".
*/
void switchMode(){
 Serial.println("Mode Switched!"); 
 if(bcd){
    bcd = false;
    binary = true;
    Serial.println("Binary mode");
 }
 else if(binary){
    binary = false;
    digital = true;
    Serial.println("Digital mode");
 }
 else if(digital){
    digital = false;
    party = true;
    Serial.println("Party mode");
 }
 else if(party){
    party = false;
    bcd = true;
    Serial.println("BCD Mode");
 }
}

/*
* Function: Power Mode
* Type: Interrupt Functions
*
* Purpose: 
*         - Called when the Power Mode button has been pressed, used to switch between power modes (On / Off).
*         - Serial Prints "Power Mode!".
*         - Flips the current state of the Sleep Mode (used to flip between On and Off).
*/
void powerMode(){
  Serial.println("Power Mode!");
  sleep = !sleep;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Additional Clock Functions ///////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
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

void updateTime(){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  currSecond = bcdToDec(Wire.read() & 0x7f); 
  currMinute = bcdToDec(Wire.read());
  currHour = bcdToDec(Wire.read() & 0x3f);
  if(currHour > 12){
    currHour = currHour - 12;
  }
  currDayOfWeek = bcdToDec(Wire.read());
  currDayOfMonth = bcdToDec(Wire.read());
  currMonth = bcdToDec(Wire.read());
  currYear = bcdToDec(Wire.read());
}




