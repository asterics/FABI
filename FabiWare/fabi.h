
/* 
     Assistive Button Interface (FABI) Version 2.1  - AsTeRICS Academy 2015 - http://www.asterics-academy.net
      allows control of HID functions via switches and/or AT-commands  
   

   requirements:  Arduino Pro Micro 
                  or Teensy/Teensy2.0++ with Teensyduino AddOn for Arduino IDE 
                  (USB type set to USB composite device: Serial + Keyboard + Mouse + Joystick)
        sensors:  up to 9 momentary switches connected to GPIO pins / optional pressure sensor connected to ADC pins 
       
   
   for a list of supported AT commands, see commands.h / commands.cpp
   
 */


#ifndef _FABI_H_
#define _FABI_H_

#include <Arduino.h>
#include <string.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include "commands.h"


#define VERSION_STRING "FABI v2.2"

//#define TEENSY                 //  if a Teensy controller is used
#define ARDUINO_PRO_MICRO   //  if Arduino Leonardo or Arduino Pro Micro is used 

#ifdef  ARDUINO_PRO_MICRO
  #include <Mouse.h>
  #include <Keyboard.h>
#endif

#define NUMBER_OF_BUTTONS  11         // number of connected or virtual switches
#define NUMBER_OF_PHYSICAL_BUTTONS 9  // number of connected switches
#define NUMBER_OF_LEDS     3          // number of connected leds
#define EEPROM_SIZE        1023       // maximum size of EEPROM storage


#define MOUSE_ACCELDELAY   50         // steps to reach mouse move speed

#define MAX_KEYSTRING_LEN 25          // maximum lenght for key identifiers / keyboard text
#define MAX_SLOTNAME_LEN  10          // maximum lenght for a slotname
#define MAX_CMDLEN MAX_KEYSTRING_LEN+3

#define PARTYPE_NONE  0
#define PARTYPE_UINT  1
#define PARTYPE_INT   2
#define PARTYPE_STRING  3

#define REPORT_NONE  0  
#define REPORT_ONE_SLOT  1
#define REPORT_ALL_SLOTS 2

#define DEFAULT_WAIT_TIME       5   // wait time for one loop interation in milliseconds
#define DEFAULT_CLICK_TIME      8    // time for mouse click (loop iterations from press to release)
#define DOUBLECLICK_MULTIPLIER  5    // CLICK_TIME factor for double clicks
#define DEFAULT_DEBOUNCING_TIME 7   // debouncing interval for button-press / release


struct settingsType {
  char slotname[MAX_SLOTNAME_LEN];     // EEPROM data is stored only until ths string's end
  uint8_t  ws;     // wheel stepsize  
  uint16_t tt;     // threshold time 
  uint16_t ts;     // threshold sip
  uint16_t tp;     // threshold puff 
};

struct atCommandType {                      // holds settings for a button function 
  char atCmd[3];
  uint8_t  partype;
};

struct buttonType {                      // holds settings for a button function 
  int mode;
  int value;
  char keystring[MAX_KEYSTRING_LEN];
};

struct buttonDebouncerType {              // holds working data for button debouncing and longpress detection 
  uint8_t bounceCount;
  uint8_t bounceState;
  uint8_t stableState;
  uint8_t longPressed;
  uint32_t timestamp;
} ; 

extern uint8_t DebugOutput;
extern uint8_t actSlot;
extern uint8_t reportSlotParameters;
extern uint8_t reportRawValues;
extern struct settingsType settings;
extern int EmptySlotAddress;
extern struct buttonType buttons[NUMBER_OF_BUTTONS];
extern struct buttonDebouncerType buttonDebouncers[NUMBER_OF_BUTTONS];
extern const struct atCommandType atCommands[];

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

void performCommand (uint8_t cmd, int16_t par1, char * keystring, int8_t periodicMouseMovement);
void saveToEEPROM(char * slotname);
void readFromEEPROM(char * slotname);
void deleteSlots();
void listSlots();
void printCurrentSlot();


void BlinkLed();
int freeRam ();
void parseByte (int newByte);

int getKeycode(char*);
void sendToKeyboard( char * );
void setKeyValues(char* text); // presses individual keys
void releaseKeys(char* text);  // releases individual keys
void release_all();            // releases all previously pressed keys and buttons

#define strcpy_FM   strcpy_PF
#define strcmp_FM   strcmp_PF
typedef uint_farptr_t uint_farptr_t_FM;

#endif
