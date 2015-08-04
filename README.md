### Welcome to the BitClock, powered by Arduino (Stephen Gilardi, 2015)

BitClock is a simple clock which features three time modes to display the current time, and an additional party mode to have fun with.

#### DIFFERENT MODES
* Default time mode selected is Binary Counting, which uses a single column to represent a decimal digit, in binary, to display the time.
* Additional time mode, Binary, does a straight conversion from the hour, minute, and second values to binary to display time.
* Additional time mode, Digital, displays the time in text which scrolls along the display.
* Additional mode, Party, flashes various lights to simulate a fun party environment.

#### ADDITIONAL FEATURES
* Three RGB LEDs are used to display information back to the user (what mode is selected, etc).
* A button to switch modes.
* A button to turn the device on and off.

#### DEMO
Watch the youtube demo video [here](http://www.youtube.com/watch?v=_swiyXcUvNY)

##### This code was written for the following pieces of hardware:
  * Arduino UNO
  * Adafruit 8x8 LED matrix (Additional Libraries needed, found at Adafruit.com)
  * DS3231
  * RGB LED x 3
  * Tactile Button x 2
