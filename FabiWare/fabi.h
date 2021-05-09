
/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI
     
     Module: fabi.h - general defines & declarations
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _FABI_H_
#define _FABI_H_

#include <Arduino.h>
#include <string.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include "commands.h"
#include "bluetooth.h"
#include "hid_hal.h"
#include "NeoPixel.h"
#include "buzzer.h"

//#include <Wire.h>
//#include <SPI.h>


#define VERSION_STRING "FABI v2.5"

//#define DEBUG_OUTPUT      //  if debug output is desired

#include <Mouse.h>
#include <Keyboard.h>

#define NUMBER_OF_BUTTONS  11         // number of pyhsical plus virtual switches, note: if more than 16, change buttonState type to uint32_t!

#define MAX_SLOTNAME_LEN      10      // maximum lenght for a slotname
#define KEYSTRING_BUFFER_LEN 400      // maximum lenght for all string parameters of a slot 
#define MAX_CMDLEN           120      // maximum lenght of a single AT command

#define DEFAULT_WAIT_TIME            5   // wait time for one loop interation in milliseconds
#define DEFAULT_CLICK_TIME           8   // time for mouse click (loop iterations from press to release)
#define DOUBLECLICK_MULTIPLIER       5   // CLICK_TIME factor for double clicks
#define DEFAULT_WHEEL_STEPSIZE       3   // stepsize for scroll wheel
#define DEFAULT_SIP_THRESHOLD        0   // sip action disabled per default
#define DEFAULT_PUFF_THRESHOLD    1023   // puff action disabled per default
#define DEFAULT_ANTITREMOR_PRESS     5   // debouncing interval for button-press
#define DEFAULT_ANTITREMOR_RELEASE   2   // debouncing interval for button-release
#define DEFAULT_ANTITREMOR_IDLE      1   // debouncing interval for button idle time
#define DEFAULT_BT_MODE              1   // USB HID only
#define DEFAULT_TRESHOLD_TIME        0   // treshold time for long press (0: disable long press)
#define DEFAULT_DOUBLEPRESS_TIME     0   // treshold time for double press (0: disable double press)
#define DEFAULT_AUTODWELL_TIME       0   // treshold time for automatic dwelling after mouse movement (0: disable autodwell)


struct settingsType {
  char slotname[MAX_SLOTNAME_LEN];     // EEPROM slotname maximum length
  uint8_t  ws;     // wheel stepsize  
  uint16_t tt;     // threshold time for longpress 
  uint16_t ts;     // threshold sip
  uint16_t tp;     // threshold puff 
  uint16_t ap;     // antitremor press time 
  uint16_t ar;     // antitremor release time 
  uint16_t ai;     // antitremor idle time
  uint8_t  bt;     // bt-mode (0,1,2)
  uint16_t dp;     // double press time  
  uint16_t ad;     // automatic dwelling time  
  uint32_t sc;     // slotcolor (0x: rrggbb)
};

struct atCommandType {              // holds settings for a button function 
  char atCmd[3];
  uint8_t  partype;   // type of parameter: int, uint or string
};

struct buttonType {                 // holds settings for a button function 
  int mode;          // buttonmode index (AT command type)
  int value;         // value (if numeric parameter) - note that strings are stored in keystringBuffer[]
};

struct buttonDebouncerType {       // holds working data for button debouncing and longpress detection 
  uint16_t pressCount;
  uint16_t releaseCount;
  uint16_t idleCount;
  uint8_t  pressState;
} ; 

extern uint8_t PCBversion;
extern uint8_t actSlot;
extern uint8_t reportSlotParameters;
extern uint8_t reportRawValues;
extern struct settingsType settings;
extern int EmptySlotAddress;
extern struct buttonType buttons[NUMBER_OF_BUTTONS];
extern struct buttonDebouncerType buttonDebouncers[NUMBER_OF_BUTTONS];
extern const struct atCommandType atCommands[];
extern char cmdstring[MAX_CMDLEN];                 // buffer for incoming AT commands
extern char keystringBuffer[KEYSTRING_BUFFER_LEN]; // buffer for all string parameters for the buttons of a slot


extern uint8_t leftMouseButton;
extern uint8_t middleMouseButton;
extern uint8_t rightMouseButton;
extern uint8_t leftClickRunning;
extern uint8_t rightClickRunning;
extern uint8_t middleClickRunning;
extern uint8_t doubleClickRunning;
extern uint32_t mouseMoveTimestamp;
extern char * writeKeystring;
extern int8_t moveX;       
extern int8_t moveY;

char * getKeystring (uint8_t button);
void setKeystring (uint8_t button, char * text);
void printKeystrings ();
uint16_t  keystringMemUsage(uint8_t button);
void parseCommand (char * cmdstr);
void parseByte (int newByte);

#define strcpy_FM   strcpy_PF
#define strcmp_FM   strcmp_PF
typedef uint_farptr_t uint_farptr_t_FM;


#endif
