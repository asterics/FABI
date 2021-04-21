
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

#define VERSION_STRING "FABI v2.5"

//#define DEBUG_OUTPUT      //  if debug output is desired
//#define TEENSY            //  if a Teensy2.0++ controller is used
#define ARDUINO_PRO_MICRO   //  if Arduino Leonardo or Arduino (Pro) Micro is used 

#ifdef  ARDUINO_PRO_MICRO
  #include <Mouse.h>
  #include <Keyboard.h>
  #define EEPROM_SIZE        1023     // maximum size of EEPROM storage for Arduino (Pro) Micro
#else
  #define EEPROM_SIZE        4096     // maximum size of EEPROM storage for Teensy2.0++
#endif

#define NUMBER_OF_BUTTONS  11         // number of connected or virtual switches
#define NUMBER_OF_PHYSICAL_BUTTONS 9  // number of connected switches
#define NUMBER_OF_LEDS     3          // number of connected leds


#define MOUSE_ACCELDELAY   50         // steps to reach mouse move speed

#define MAX_SLOTNAME_LEN      10      // maximum lenght for a slotname
#define KEYSTRING_BUFFER_LEN 500      // maximum lenght for all string parameters of a slot 
#define MAX_CMDLEN           200      // maximum lenght of a single AT command

#define PARTYPE_NONE   0
#define PARTYPE_UINT   1
#define PARTYPE_INT    2
#define PARTYPE_STRING 3

#define REPORT_NONE  0  
#define REPORT_ONE_SLOT  1
#define REPORT_ALL_SLOTS 2

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

#define DEFAULT_TRESHOLD_TIME     5000   // treshold time for short / long press (5000: disable long press)
#define BUTTON_PRESSED  1
#define BUTTON_RELEASED 0
#define BUTTONSTATE_NOT_PRESSED   0
#define BUTTONSTATE_SHORT_PRESSED 1
#define BUTTONSTATE_LONG_PRESSED  2
#define BUTTONSTATE_IDLE          3

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
extern uint16_t freeEEPROMbytes;
extern const int usToDE[];

extern uint8_t leftMouseButton;
extern uint8_t middleMouseButton;
extern uint8_t rightMouseButton;
extern uint8_t leftClickRunning;
extern uint8_t rightClickRunning;
extern uint8_t middleClickRunning;
extern uint8_t doubleClickRunning;
extern char * writeKeystring;
extern int8_t moveX;       
extern int8_t moveY;

char * getKeystring (uint8_t button);
void setKeystring (uint8_t button, char * text);
void printKeystrings ();
uint16_t  keystringMemUsage(uint8_t button);
void parseCommand (char * cmdstr);
void performCommand (uint8_t cmd, int16_t par1, char * keystring, int8_t periodicMouseMovement);
void saveToEEPROM(char * slotname);
void readFromEEPROM(char * slotname);
void deleteSlots();
void listSlots();
void printCurrentSlot();


void BlinkLed();
int freeRam ();
void parseByte (int newByte);

void setBeepCount(uint16_t count);
void beepXtimes(uint8_t numberOFbeeps);
void write2Display(const char* text, uint8_t newLine);
void writeSlot2Display();
void updateSlot(uint8_t newSlotNumber);

int getKeycode(char*);
void sendToKeyboard( char * );
void pressSingleKeys(char* text); // presses individual keys
void releaseSingleKeys(char* text);  // releases individual keys
void release_all();            // releases all previously pressed keys and buttons

#define strcpy_FM   strcpy_PF
#define strcmp_FM   strcmp_PF
typedef uint_farptr_t uint_farptr_t_FM;



// fabi logo for 128x32 OLED screen; height:32px widht: 55px
/*
const unsigned char FABIlogo [] PROGMEM = {
  };
  */



  const uint8_t FABIlogo1 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x78, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x18, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x84, 0x3F, 
  0x02, 0x00, 0x00, 0x00, 0x00, 0x62, 0x60, 0x04, 0x00, 0x00, 0x00, 0x00, 
  0x11, 0x80, 0x09, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x0B, 0x00, 0x00, 
  0x00, 0x80, 0x04, 0x0E, 0x12, 0x00, 0x00, 0x00, 0x80, 0x04, 0x1E, 0x14, 
  0x00, 0x00, 0x00, 0x80, 0xC2, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x00, 0xE2, 
  0x07, 0x24, 0x00, 0x00, 0x00, 0x40, 0xE2, 0x7F, 0x24, 0x00, 0x00, 0x00, 
  0x40, 0xC0, 0xFE, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x24, 0x00, 
  0x00, 0x00, 0x80, 0x80, 0x07, 0x14, 0x00, 0x80, 0x00, 0xF0, 0x80, 0x07, 
  0x14, 0xFE, 0xC3, 0x01, 0xFF, 0x71, 0x07, 0x12, 0x1E, 0xC0, 0x81, 0x87, 
  0x31, 0x00, 0x0B, 0x06, 0xE0, 0x01, 0x83, 0x31, 0x80, 0x09, 0x06, 0x60, 
  0x03, 0xC3, 0x30, 0xE0, 0x04, 0x06, 0x30, 0x03, 0x73, 0x30, 0x3E, 0x02, 
  0xFE, 0x31, 0x03, 0xFB, 0x30, 0x80, 0x01, 0xFE, 0x18, 0x07, 0xCB, 0xF1, 
  0x70, 0x00, 0x06, 0xFC, 0x07, 0x83, 0x19, 0x0F, 0x00, 0x03, 0x3C, 0x8E, 
  0x81, 0x19, 0x00, 0x00, 0x03, 0x0E, 0x8C, 0xC1, 0x18, 0x00, 0x00, 0x03, 
  0x06, 0x9C, 0x71, 0x38, 0x00, 0x00, 0x03, 0x02, 0x98, 0x3D, 0x10, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


  #endif
