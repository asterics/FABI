/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: display.cpp - implementation of the Oled display functions 
     The utilized library is https://github.com/greiman/SSD1306Ascii

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include <Arduino.h>
#include "FlipWare.h"
#include "gpio.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "display.h"

#define SCREEN_ADDRESS 0x3C

uint8_t displayAvailable=0;   // indicates if LCD was found on I2C bus
uint8_t displayPaused=0;      // if true, display updates are paused

SSD1306AsciiWire *oled;    // pointer to the display driver class

/**
   @name initDisplay
   @param uint8_t useWire1:  true if Wire1 should be used, else use Wire
   @return uint_t: returns true if display was found, else false

   initialises the i2c-communication and display module, prints out module name
*/
uint8_t initDisplay () {
  displayAvailable=0;

  // check if LCD is found on Wire0 
  Wire.beginTransmission(SCREEN_ADDRESS);  
  if (!Wire.endTransmission()) {
    displayAvailable=1;
    oled = new SSD1306AsciiWire(Wire);
  }
/*  else {
    // check if LCD is found on Wire1 
    Wire1.beginTransmission(SCREEN_ADDRESS);  
    if (!Wire1.endTransmission()) {
      displayAvailable=1;
      oled = new SSD1306AsciiWire(Wire1);
    } 
  }  */

  if (displayAvailable) {
    oled->begin(&Adafruit128x32, SCREEN_ADDRESS);
    oled->setFont(Adafruit5x7);
    //oled->setFont(Arial_bold_14);
    //displayClear();
    //oled->set2X();
    //oled->print(moduleName);
    return true;
  }
  return false;
}

/**
   @name isDisplayAvailable
   @brief returns true if LC Display is available at interface Wire0 or Wire1
   @return true if LCD was initialized, false if not
*/
uint8_t isDisplayAvailable() {
  return (displayAvailable);
}


/**
   @name displayClear
   @param none
   @return none

   clears the display and sets correct rotation and display mode
*/
void displayClear(void) {
  oled->clear();
  if (slotSettings.ro>90)
    oled->displayRemap(true);
  else oled->displayRemap(false);
  oled->setInvertMode(false);
}


/**
   @name displayMessage
   @param char * msg: string which is printed
   @return none

   clears the display and prints message (center line, double sized)
*/
void displayMessage(char * msg) {
  if (!displayAvailable) return;
  displayClear();
  oled->set2X();
  oled->setCursor(5,1);
  oled->print(msg);  
}

/**
   @name displayUpdate
   @param none
   @return none

   clear display, then print current slotname and mode information
*/
void displayUpdate(void) {
  if ((!displayAvailable) || (displayPaused)) return;
  displayClear();
  oled->set2X();
  oled->setCursor(5,1);
  oled->print(slotSettings.slotName);  
  oled->set1X();

  // display modes for FLipPad
  #ifdef FLIPPAD
    oled->setCursor(100,0);
    switch (slotSettings.stickMode) {
      case 0:
      case 1: oled->print("Stk"); break;
      case 2:
      case 3:
      case 4: oled->print("Joy"); break;
    }
  #endif  

  oled->setCursor(100,3);
  switch (slotSettings.bt) {
    case 1: oled->print("USB"); break;
    case 2: oled->print("BT"); break;
    case 3: oled->print("both"); break;
  }
}


/**
   @name pauseDisplayUpdates
   @param pause: if true, display updates will be paused
   @brief allows to bypass display updates
   @return none
*/
void pauseDisplayUpdates(uint8_t pause) {
  displayPaused=pause;
}
