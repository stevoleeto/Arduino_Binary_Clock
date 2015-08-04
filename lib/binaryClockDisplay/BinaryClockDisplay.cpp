/*
* BinaryClockDisplay.h - Library for accessing the 8x8 LED Matrix and the three RBG LEDs
* Created by Stephen Gilardi, July 24, 2015
*/
#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include "BinaryClockDisplay.h"


#define blue 0
#define green 1
#define red 2

/*
* Defines specific columns to be used on the LED Matrix
*
* When displaying BCD or Binary Time:
*    Hour Bits:   Columns 0 & 1
*    Minute BIts: Columns 3 & 4
*    Second Bits: Columns 6 & 7
*
* When displaying Binary Time:
*    Hour Bits:   Column 0
*    Minute Bits: Column 3
*    Second Bits: Column 6
*/
#define hourColA 0
#define hourColB 1
#define minuteColA 3
#define minuteColB 4
#define secondColA 6
#define secondColB 7

#define Adafruit_8x8matrix_ADDRESS 0x70

//Declare a new Adafruit_8x8matrix object called -> matrix
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

/*
* Function: Binary Clock Display Constructor 
*
* Purpose: 
*         - Initalizes an Object to be used to access the Binary Clock Display
*/
BinaryClockDisplay::BinaryClockDisplay(int LED1Blue, int LED1Green, int LED1Red, int LED2Blue, int LED2Green, int LED2Red, int LED3Blue, int LED3Green, int LED3Red){
	rgbLEDs[0][0] = LED1Blue;
	rgbLEDs[0][1] = LED1Green;
	rgbLEDs[0][2] = LED1Red;
	rgbLEDs[1][0] = LED2Blue;
	rgbLEDs[1][1] = LED2Green;
	rgbLEDs[1][2] = LED2Red;
	rgbLEDs[2][0] = LED3Blue;
	rgbLEDs[2][1] = LED3Green;
	rgbLEDs[2][2] = LED3Red;

	for(int indexLED = 0; indexLED < 3; indexLED++){
		for(int indexColor = 0; indexColor < 3; indexColor++){
			pinMode(rgbLEDs[indexLED][indexColor], OUTPUT);
		}
	}

	matrix.begin(Adafruit_8x8matrix_ADDRESS); //the I2C address for the matrix is 0x70
	matrix.setTextSize(1);
	matrix.setTextWrap(false);
	matrix.setTextColor(LED_ON);
	matrix.setRotation(3);
}

/*
* Function: Start Up
* Type: Set up 
*
* Purpose: 
*         - Calls four visual functions to be performed during inital setup.
*/
void BinaryClockDisplay::playStartUp(){
	upDown();
  	upDown();
  	writeWelcome();
  	smile();
  	matrix.clear();
}

/*
* Function: Up Down
* Type: Set up 
*
* Purpose: 
*         - Rolls a wall up and down the LED matrix.
*         - Dims the three LEDs blue as wall moves up and down the matrix.
*/
void BinaryClockDisplay::upDown(){
	matrix.clear();
	for(int row = 7; row > -1; row--){
		analogWrite(rgbLEDs[0][blue], 200 - (row*25));
		analogWrite(rgbLEDs[1][blue], 200 - (row*25));
		analogWrite(rgbLEDs[2][blue], 200 - (row*25));
		matrix.drawLine(0, row, 7, row, LED_ON);
		matrix.writeDisplay();
		delay(100);
	}
	for(int row = 0; row < 8; row++){
		analogWrite(rgbLEDs[0][blue], 200 - (row*25));
		analogWrite(rgbLEDs[1][blue], 200 - (row*25));
		analogWrite(rgbLEDs[2][blue], 200 - (row*25));
		matrix.drawLine(0,  row, 7, row, LED_OFF);
		matrix.writeDisplay();
		delay(100); 
	}
	digitalWrite(rgbLEDs[0][blue], LOW);
	digitalWrite(rgbLEDs[1][blue], LOW);
	digitalWrite(rgbLEDs[2][blue], LOW);
}

/*
* Function: Write Welcome
* Type: Set up 
*
* Purpose: 
*         - Displays a welcome messge, "Hi Stephen", across the LED matrix.
*         - Blinks the three LEDs blue in same direction as text.
*/
void BinaryClockDisplay::writeWelcome(){
	boolean blueOn1, blueOn2 = false;
	boolean blueOn3 = true;
	for(int8_t x = 10; x>=-59; x--){
		if(blueOn3){
		  digitalWrite(rgbLEDs[2][blue], HIGH);
		  digitalWrite(rgbLEDs[0][blue], LOW);
		}
		if(blueOn2){
		  digitalWrite(rgbLEDs[1][blue], HIGH);
		  digitalWrite(rgbLEDs[2][blue], LOW);
		}
		if(blueOn1){
		  digitalWrite(rgbLEDs[0][blue], HIGH);
		  digitalWrite(rgbLEDs[1][blue], LOW);
		}
		matrix.clear();
		matrix.setCursor(x, 0);
		matrix.print("Hi Stephen");
		matrix.writeDisplay();
		delay(100);
		if(blueOn3){
		  blueOn3 = false;
		  blueOn2 = true;
		}
		else if(blueOn2){
		  blueOn2 = false;
		  blueOn1 = true;
		}
		else if(blueOn1){
		  blueOn1 = false;
		  blueOn3 = true;
		}
	}
	digitalWrite(rgbLEDs[0][blue], LOW);
	digitalWrite(rgbLEDs[1][blue], LOW);
	digitalWrite(rgbLEDs[2][blue], LOW);
}

/*
* Function: Smile
* Type: Set up 
*
* Purpose: 
*         - Displays a smiley face on the LED matrix.
*         - Turns on three LEDs to be green during the duration of the smiley face.
*/
void BinaryClockDisplay::smile(){
	static const uint8_t PROGMEM
	smileMap[] =
		{ B00111100,
		B01000010,
		B10100101,
		B10000001,
		B10100101,
		B10011001,
		B01000010,
		B00111100 };
	matrix.clear();
	matrix.drawBitmap(0, 0, smileMap, 8, 8, LED_ON);
	matrix.writeDisplay();
	digitalWrite(rgbLEDs[0][green], HIGH);
	digitalWrite(rgbLEDs[1][green], HIGH);
	digitalWrite(rgbLEDs[2][green], HIGH);
	delay(1000);
	digitalWrite(rgbLEDs[0][green], LOW);
	digitalWrite(rgbLEDs[1][green], LOW);
	digitalWrite(rgbLEDs[2][green], LOW);
}

/*
* Function: Clear Column
* Type: LED Matrix Modification 
*
* Purpose: 
*         - Clears a specific column on the LED matrix, setting the LEDs off.
*/
void BinaryClockDisplay::clearColumn(int columnToRemove){
	for(int i = 0; i < 8; i++){
    	matrix.drawPixel(columnToRemove, i, LED_OFF);
	}
}

/*
* Function: showLEDBCD
* Type: RBG LEDs Modification 
*
* Purpose: 
*         - Turns on the first RBG LED to blue, signaling BCD mode.
*		  - Turns off the remaining blue RBG LEDs.
*/
void BinaryClockDisplay::showLEDBCD(){
	analogWrite(rbgLEDs[0][blue], 25);
    digitalWrite(rbgLEDs[1][blue], LOW);
    digitalWrite(rbgLEDs[2][blue], LOW);
}

/*
* Function: showLEDBinary
* Type: RBG LEDs Modification 
*
* Purpose: 
*         - Turns on the second RBG LED to blue, signaling Binary mode.
*		  - Turns off the remaining blue RBG LEDs.
*/
void BinaryClockDisplay::showLEDBinary(){
	analogWrite(rbgLEDs[1][blue], 25);
    digitalWrite(rbgLEDs[0][blue], LOW);
    digitalWrite(rbgLEDs[2][blue], LOW);
}

/*
* Function: showLEDDigital
* Type: RBG LEDs Modification 
*
* Purpose: 
*         - Turns on the third RBG LED to blue, signaling Digital mode.
*		  - Turns off the remaining blue RBG LEDs.
*/
void BinaryClockDisplay::showLEDDigital(){
	analogWrite(rbgLEDs[2][blue], 25);
    digitalWrite(rbgLEDs[1][blue], LOW);
    digitalWrite(rbgLEDs[0][blue], LOW);
}

/*
* Function: Turn Column Binary
* Type: LED Matrix Modification 
*
* Purpose: 
*         - Takes a column and an integer as parameters.
*		  - Converts the integer value to binary, and displays it on the specific column.
*/
void BinaryClockDisplay::turnColumnBinary(int columnToChange, int numToConvertToBinary){
	clearColumn(columnToChange);
	for(int ledHorz = 7; ledHorz > -1; ledHorz--){
		if(numToConvertToBinary % 2 == 1){
			matrix.drawPixel(columnToChange, ledHorz, LED_ON);
	 	} 
	 	numToConvertToBinary = numToConvertToBinary / 2;
	}
}

/*
* Function: Display BCD Time
* Type: LED Matrix Mode
*
* Purpose: 
*         - Takes in three integer values to represent an hour, minute, and second.
*		  - Uses the turnColumnBinary() function to display the time in BCD format.
*/
void BinaryClockDisplay::displayBCDTime(int theCurrentHour, int theCurrentMinute, int theCurrentSecond){
	//Displays the second columns
	turnColumnBinary(secondColB, theCurrentSecond % 10);
	turnColumnBinary(secondColA, theCurrentSecond / 10);

	//Displays the minute columns
	turnColumnBinary(minuteColB, theCurrentMinute % 10);
	turnColumnBinary(minuteColA, theCurrentMinute / 10);

	//Displays the hour columns
	turnColumnBinary(hourColB, theCurrentHour % 10);
	turnColumnBinary(hourColA, theCurrentHour / 10);

	matrix.writeDisplay();
}

/*
* Function: Display Binary Time
* Type: LED Matrix Mode
*
* Purpose: 
*         - Takes in three integer values to represent an hour, minute, and second.
*		  - Uses the turnColumnBinary() function to display the time in binary format.
*		  - Uses the clearColumn() function to confirm that the extra columns are turned off.
*/
void BinaryClockDisplay::displayBinaryTime(int theCurrentHour, int theCurrentMinute, int theCurrentSecond){
	clearColumn(secondColB);
	clearColumn(minuteColB);
	clearColumn(hourColB);
	turnColumnBinary(secondColA, theCurrentSecond);
	turnColumnBinary(minuteColA, theCurrentMinute);
	turnColumnBinary(hourColA, theCurrentHour); 
	matrix.writeDisplay();
}

/*
* Function: Display Digital Time
* Type: LED Matrix Mode
*
* Purpose: 
*         - Takes in three integer values to represent an hour, minute, and second.
*		  - Prints the time as a string to the matrix
*		  - Moves the text along the screen so that the full time can be viewed.
*		  - Continually monitors digital mode boolean and sleep mode booleans to be able to quit abrutly.
*/
void BinaryClockDisplay::displayDigitalTime(int theCurrentHour, int theCurrentMinute, int theCurrentSecond){
	for(int8_t x = 10; x>=-59; x--){
    if(!digital | sleep){ //TODO - Make this work via new library
      matrix.clear();
      break; 
    }
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print(theCurrentHour);
    matrix.print(":");
    matrix.print(theCurrentMinute);
    matrix.print(":");
    matrix.print(theCurrentSecond);
    matrix.writeDisplay(); 
    delay(100);
  }
}

/*
* Function: Display Party Mode
* Type: LED Matrix Mode
*
* Purpose: 
*		  - Randomly turns the RBG LEDs on and off
*		  - Chooses random pixels in the LED matrix to turn off and on.
*		  - Monitors the party boolean and sleep boolean values to contiune operating until quit.
*		  - When disabled, returns LED matrix and RGB LEDs to their default values.
*/
void BinaryClockDisplay::displayparty(){
	int blueOn1, blueOn2, blueOn3, redOn1, redOn2, redOn3, greenOn1, greenOn2, greenOn3 = 0;
	
	smile();
	
	matrix.clear();
	matrix.writeDisplay();

	while(party & !sleep){ //TODO: Get this to work via new library
		blueOn1 = random(0, 2);
		blueOn2 = random(0, 2);
		blueOn3 = random(0, 2);
		greenOn1 = random(0, 2);
		greenOn2 = random(0, 2);
		greenOn3 = random(0, 2);
		redOn1 = random(0, 2);
		redOn2 = random(0, 2);
		redOn3 = random(0, 2);
		digitalWrite(rbgLEDs[0][blue], blueOn1);
		digitalWrite(rbgLEDs[0][green], greenOn1);
		digitalWrite(rbgLEDs[0][red], redOn1);
		digitalWrite(rbgLEDs[1][blue], blueOn2);
		digitalWrite(rbgLEDs[1][green], greenOn2);
		digitalWrite(rbgLEDs[1][red], redOn2);
		digitalWrite(rbgLEDs[2][blue], blueOn3);
		digitalWrite(rbgLEDs[2][green], greenOn3);
		digitalWrite(rbgLEDs[2][red], redOn3);
		matrix.drawPixel(random(0, 8), random(0, 8), LED_ON);
		matrix.drawPixel(random(0, 8), random(0, 8), LED_ON);
		matrix.drawPixel(random(0, 8), random(0, 8), LED_OFF);
		matrix.drawPixel(random(0, 8), random(0, 8), LED_OFF);
		matrix.drawPixel(random(0, 8), random(0, 8), LED_OFF);
		matrix.drawPixel(random(0, 8), random(0, 8), LED_OFF);
		matrix.setRotation(random(0, 5));
		matrix.writeDisplay();
		delay(500);
	}
	digitalWrite(rbgLEDs[0][blue], LOW);
	digitalWrite(rbgLEDs[0][green], LOW);
	digitalWrite(rbgLEDs[0][red], LOW);
	digitalWrite(rbgLEDs[1][blue], LOW);
	digitalWrite(rbgLEDs[1][green], LOW);
	digitalWrite(rbgLEDs[1][red], LOW);
	digitalWrite(rbgLEDs[2][blue], LOW);
	digitalWrite(rbgLEDs[2][green], LOW);
	digitalWrite(rbgLEDs[2][red], LOW);
	matrix.setRotation(3);
	matrix.clear();
}