/********************************************************************************************
Binary Clock

This code was written for the following parts:
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
* Note: Adafruit GFX and LEDBackpack libraries can be found at Adafruit.com
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

/*
* Defines the addresses for the D23231 and Adafruit 8x8 matrix
*
* DS3231 Address              -> 0x68
* Adafruit 8x8 Matrix Address -> 0x70
*/
#define DS3231_I2C_ADDRESS 0x68
#define Adafruit_8x8matrix_ADDRESS 0x70

/*
* Defines specific columns to be used on the LED Matrix
*
* When displaying BCD Time:
*    Hour Bits:   Columns 0 & 1
*    Minute BIts: Columns 3 & 4
*    Second Bits: Columns 6 & 7
*
* When displaying Binary Time:
*    Hour Bits:   Column 0
*    Minute Bits: Column 3
*    Second Bits: Column 6
*/
#define BCDhourColA 0
#define BCDhourColB 1
#define BCDminuteColA 3
#define BCDminuteColB 4
#define BCDsecondColA 6
#define BCDsecondColB 7

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

//Declare a new Adafruit_8x8matrix object called -> matrix
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

/*
* Initalizes the four logic controlling booleans.
* Used to switch between different modes.
*
* Binary Counter Time -> bcd
* Binary Time         -> binary
* Digital Time        -> digital
* Party Mode          -> party
* Sleep Mode          -> sleep
*/
volatile boolean bcd = false;
volatile boolean binary = false;
volatile boolean digital = false;
volatile boolean party = false;
volatile boolean sleep = false;

/*
* Initalizes the seven integer values representing the current time and date.
* Used to switch between different modes.
*
* Second -> currSecond
* Minute         -> currMinute
* Hour        -> currHour
* Day of the Week -> currDayOfWeek
* Day of the Month -> currDayOfMonth
* Month -> currMonth
* Year -> currYear
*/
int currSecond = 99;
int currMinute = 99;
int currHour = 99;
int currDayOfWeek = 99;
int currDayOfMonth = 99;
int currMonth = 99;
int currYear = 99;
   
////////////////////////////////////////////////////////////////////////////////
//////////////////////// Set up Functions //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
   
void startup(){
  upDown();
  upDown();
  writeName();
  smile(); 
}

void writeName(){
  boolean blueOn1, blueOn2 = false;
  boolean blueOn3 = true;
  for(int8_t x = 10; x>=-59; x--){
    if(blueOn3){
      digitalWrite(blue3, HIGH);
      digitalWrite(blue1, LOW);
    }
    if(blueOn2){
      digitalWrite(blue2, HIGH);
      digitalWrite(blue3, LOW);
    }
    if(blueOn1){
      digitalWrite(blue1, HIGH);
      digitalWrite(blue2, LOW);
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
  digitalWrite(blue1, LOW);
  digitalWrite(blue2, LOW);
  digitalWrite(blue3, LOW);
}

void upDown(){
  matrix.clear();
  for(int row = 7; row > -1; row--){
    analogWrite(blue1, 200 - (row*25));
    analogWrite(blue2, 200 - (row*25));
    analogWrite(blue3, 200 - (row*25));
    matrix.drawLine(0, row, 7, row, LED_ON);
    matrix.writeDisplay();
    delay(100);
  }
  for(int row = 0; row < 8; row++){
    analogWrite(blue1, 200 - (row*25));
    analogWrite(blue2, 200 - (row*25));
    analogWrite(blue3, 200 - (row*25));
    matrix.drawLine(0,  row, 7, row, LED_OFF);
    matrix.writeDisplay();
    delay(100); 
  }
  
  digitalWrite(blue1, LOW);
  digitalWrite(blue2, LOW);
  digitalWrite(blue3, LOW);
}

void smile(){
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
  digitalWrite(green1, HIGH);
  digitalWrite(green2, HIGH);
  digitalWrite(green3, HIGH);
  delay(1000);
  digitalWrite(green1, LOW);
  digitalWrite(green2, LOW);
  digitalWrite(green3, LOW);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// Arduino Functions ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void setup() {
  
  attachInterrupt(0, switchMode, RISING);
  attachInterrupt(1, powerMode, RISING);
  Serial.begin(9600);
  Serial.println("Matrix Test\nStephen Gilardi\n7/13/2015");
  Serial.println("Setup()");
  
  Wire.begin();
  matrix.begin(Adafruit_8x8matrix_ADDRESS); //the I2C address for the matrix is 0x70
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setRotation(3);
  
  pinMode(red1, OUTPUT);
  pinMode(green1, OUTPUT);
  pinMode(blue1, OUTPUT);
  pinMode(red2, OUTPUT);
  pinMode(green2, OUTPUT);
  pinMode(blue2, OUTPUT);
  pinMode(red3, OUTPUT);
  pinMode(green3, OUTPUT);
  pinMode(blue3, OUTPUT);
  
  //seconds, minutes, hours, day, date, month, year
  //setDS3231time(00,41,20,3,21,7,15);
  startup();
  matrix.clear();
  bcd = true;
  
}

void loop() {
  if(sleep){
    matrix.clear();
    matrix.writeDisplay();
    while(sleep){} 
  }
  else{
    updateTime();
    if(bcd){
      analogWrite(blue1, 25);
      digitalWrite(blue2, LOW);
      digitalWrite(blue3, LOW);
      displayBCDTime(); 
    }
    
    else if(binary){
      analogWrite(blue2, 25);
      digitalWrite(blue1, LOW);
      digitalWrite(blue3, LOW);
      displayBinaryTime();
    }
    
    else if(digital){
      analogWrite(blue3, 25);
      digitalWrite(blue2, LOW);
      digitalWrite(blue1, LOW);
      displayDigitalTime();
    }
    
    else if(party){
      displayparty(); 
    }
    delay(250);
  }
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Watch Face Functions //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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

void powerMode(){
  Serial.println("Power Mode!");
  sleep = !sleep;
}

void displayBCDTime(){
 //Displays the second columns
 turnColumnBinary(BCDsecondColB, currSecond % 10);
 turnColumnBinary(BCDsecondColA, currSecond / 10);

 //Displays the minute columns
 turnColumnBinary(BCDminuteColB, currMinute % 10);
 turnColumnBinary(BCDminuteColA, currMinute / 10);
 
 //Displays the hour columns
 turnColumnBinary(BCDhourColB, currHour % 10);
 turnColumnBinary(BCDhourColA, currHour / 10);
 
 matrix.writeDisplay();
}

void displayBinaryTime(){
 clearColumn(BCDsecondColB);
 clearColumn(BCDminuteColB);
 clearColumn(BCDhourColB);
 turnColumnBinary(BCDsecondColA, currSecond);
 turnColumnBinary(BCDminuteColA, currMinute);
 turnColumnBinary(BCDhourColA, currHour); 
 matrix.writeDisplay();
}

void displayDigitalTime(){
 for(int8_t x = 10; x>=-59; x--){
    if(!digital | sleep){
      matrix.clear();
      break; 
    }
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print(currHour);
    matrix.print(":");
    matrix.print(currMinute);
    matrix.print(":");
    matrix.print(currSecond);
    matrix.writeDisplay(); 
    delay(100);
  } 
}

void displayparty(){
  int blueOn1, blueOn2, blueOn3, redOn1, redOn2, redOn3, greenOn1, greenOn2, greenOn3 = 0;
  smile();
  matrix.clear();
  matrix.writeDisplay();
  
  while(party){
     blueOn1 = random(0, 2);
     blueOn2 = random(0, 2);
     blueOn3 = random(0, 2);
     greenOn1 = random(0, 2);
     greenOn2 = random(0, 2);
     greenOn3 = random(0, 2);
     redOn1 = random(0, 2);
     redOn2 = random(0, 2);
     redOn3 = random(0, 2);
     digitalWrite(blue1, blueOn1);
     digitalWrite(green1, greenOn1);
     digitalWrite(red1, redOn1);
     digitalWrite(blue2, blueOn2);
     digitalWrite(green2, greenOn2);
     digitalWrite(red2, redOn2);
     digitalWrite(blue3, blueOn3);
     digitalWrite(green3, greenOn3);
     digitalWrite(red3, redOn3);
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
  digitalWrite(blue1, LOW);
  digitalWrite(green1, LOW);
  digitalWrite(red1, LOW);
  digitalWrite(blue2, LOW);
  digitalWrite(green2, LOW);
  digitalWrite(red2, LOW);
  digitalWrite(blue3, LOW);
  digitalWrite(green3, LOW);
  digitalWrite(red3, LOW);
  matrix.setRotation(3);
  matrix.clear();
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Matrix Functions //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void turnColumnBinary(int columnToChange, int numToConvertToBinary){
  clearColumn(columnToChange);
  for(int ledHorz = 7; ledHorz > -1; ledHorz--){
     if(numToConvertToBinary % 2 == 1){
       matrix.drawPixel(columnToChange, ledHorz, LED_ON);
     } 
     numToConvertToBinary = numToConvertToBinary / 2;
  }
}

void clearColumn(int column){
 for(int i = 0; i < 8; i++){
    matrix.drawPixel(column, i, LED_OFF);
 }
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




