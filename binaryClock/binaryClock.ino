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
#include <BinaryClockTime.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

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

//creates display and time objects to reference to
BinaryClockDisplay display;
BinaryClockTime timeKeep;

/*
* Function: Setup
* Type: Arduino Function
*
* Purpose: 
*         - Mandatory Arduino Function required to perform initial setup.
*         - Attaches necessary interrupts for Switch and Power Buttons.
*         - Begins Serial Communications and prints initial program information.
*         - Begins the Wire Library.
*         - Initalizes the Binary Clock Display to the appropiate settings.
*         - If uncommented, calls the set time function to initalize the time.
*         - Calls the Display Startup function.
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
  //timeKeep.modifyTime(0,0,0,0,0,0,0);
  
  display.playStartUp();
  bcd = true;
  
}

/*
* Function: Loop
* Type: Arduino Functions
*
* Purpose: 
*         - Mandatory Arduino Function required to perform continuous use.
*         - Used as the primary point of logic, decides what should be displayed.
*         - Checks to see if sleep mode has been activiated:
*             - If TRUE, calls the Display Sleep function.
*             - If FALSE, calls the Update Time Function and checks for other modes.
*         - Checks to see if bcd mode has been activiated:
*             - If TRUE, calls the Display BCD Time function.
*         - Checks to see if binary mode has been activiated:
*             - If TRUE, calls the Display Binary Time function.
*         - Checks to see if digital mode has been activiated:
*             - If TRUE, calls the Display Digital Time function.
*         - Checks to see if party mode has been activiated:
*             - If TRUE, calls the Display Party function
*         - Delays program 1/4 of a second to prevent overloading.
*/
void loop() {
  if(sleep){
    display.sleep();
  }
  else{
    timeKeep.updateTime();
    if(bcd){
      display.BCDTime(timeKeep.getHour(), timeKeep.getMinute(), timeKeep.getSecond()); 
    }
    
    else if(binary){
      display.binaryTime(timeKeep.getHour(), timeKeep.getMinute(), timeKeep.getSecond());
    }
    
    else if(digital){
      display.digitalTime(timeKeep.getHour(), timeKeep.getMinute(), timeKeep.getSecond());
    }
    
    else if(party){
      display.party(); 
    }
    delay(250);
  }
}

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