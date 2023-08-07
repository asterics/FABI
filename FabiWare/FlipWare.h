
/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: FlipWare.h  - main header file

        This firmware allows control of HID functions via FABI module and/or AT-commands
        For a description of the supported commands see: commands.h

   For a list of supported AT commands, see commands.h / commands.cpp

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/


#ifndef _FLIPWARE_H_
#define _FLIPWARE_H_

#include <Arduino.h>
#include <Wire.h>
#include <MouseBLE.h>
#include <KeyboardBLE.h>
#include <JoystickBLE.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <Joystick.h>
#include <EEPROM.h>
#include <string.h>
#include <stdint.h>
#include "commands.h"
#include "bluetooth.h"
#include "eeprom.h"
#include "buttons.h"
#include "infrared.h"
#include "hid_hal.h"

#define VERSION_STRING "v3.0"

//  V3.0: first FABI3 draft, mostly from FLipMouse code


// Optional Debug Output Control

//#define DEBUG_OUTPUT_FULL      // if full debug output is desired
//#define DEBUG_OUTPUT_MEMORY    // enables eeprom.cpp debugging, showing memory access
//#define DEBUG_OUTPUT_KEYS      // enable keys.cpp debugging, showing key press/release events and keycode lookup
//#define DEBUG_OUTPUT_IR      	 // enable infrared.cpp debugging, showing whats happening on IR recv/send
//#define DEBUG_OUTPUT_SENSORS 	 // enable sensors.cpp debugging, showing whats happening on sensor reading & init
//#define DEBUG_DELAY_STARTUP 	 // enable a 3s delay after Serial.begin and before all the other stuff.
//#define DEBUG_NO_TONE          // disable tones, to avoid annoying other passengers when programming on the train :-)


/**
   global constant definitions
*/
#define UPDATE_INTERVAL     5    // update interval for performing HID actions (in milliseconds)
#define DEFAULT_CLICK_TIME  8    // time for mouse click (loop iterations from press to release)
#define CALIBRATION_PERIOD  1000  // approx. 1000ms calibration time

// RAM buffers and memory constraints
#define WORKINGMEM_SIZE         300    // reserved RAM for working memory (command parser, IR-rec/play)
#define MAX_KEYSTRING_LEN (WORKINGMEM_SIZE-3)   // maximum length for AT command parameters
#define MAX_NAME_LEN  15               // maximum length for a slotname or ir name
#define MAX_KEYSTRINGBUFFER_LEN 500    // maximum length for all string parameters of one slot


/**
   SlotSettings struct
   contains parameters for current slot
*/
struct SlotSettings {

  char slotName[MAX_NAME_LEN];   // slotname (@warning: must be always the first element, storing relies on that!)
  uint16_t keystringBufferLen;   
  
  uint16_t ts;     // threshold sip
  uint16_t tp;     // threshold puff
  uint8_t  ws;     // wheel stepsize
  uint16_t sp;     // threshold strong puff
  uint16_t ss;     // threshold strong sip
  uint8_t  bt;     // bt-mode (0,1,2)
  uint32_t sc;     // slotcolor (0x: rrggbb)
  char kbdLayout[6];
};

/**
   SensorData structs
   contain working data of sensors (raw and processed values)
*/
struct SensorData {
  int pressure;
};

struct I2CSensorValues {
  int pressure;
  uint16_t calib_now;
  mutex_t sensorDataMutex; // for synchronization of data access between cores
};

/**
   extern declarations of functions and data structures 
   which can be accessed from different modules
*/
extern char moduleName[];
extern uint8_t actSlot;
extern uint8_t addonUpgrade;
extern struct SensorData sensorData;
extern struct I2CSensorValues sensorValues;
extern struct SlotSettings slotSettings; 
extern const struct SlotSettings defaultSlotSettings;
extern uint8_t workingmem[WORKINGMEM_SIZE];            // working memory  (command parser, IR-rec/play)
extern char keystringBuffer[MAX_KEYSTRINGBUFFER_LEN];  // storage for all button string parameters of a slot

/**
   set the correct strcpy/strcmp functions for TeensyLC / ARM)
*/
#define strcpy_FM   strcpy
#define strcmp_FM   strcmp
typedef char* uint_farptr_t_FM;

/**
 * Some define checks to warn from building in debug settings
 */
#ifdef DEBUG_OUTPUT_MEMORY
  #warning "DEBUG_OUTPUT_MEMORY active! (GUI might not work)"
#endif
#ifdef DEBUG_OUTPUT_FULL
  #warning "DEBUG_OUTPUT_FULL active! (GUI might not work)"
#endif
#ifdef DEBUG_OUTPUT_KEYS
  #warning "DEBUG_OUTPUT_KEYS active! (GUI might not work)"
#endif
#ifdef DEBUG_OUTPUT_IR
  #warning "DEBUG_OUTPUT_IR active! (GUI might not work)"
#endif
#ifdef DEBUG_OUTPUT_SENSORS
  #warning "DEBUG_OUTPUT_SENSORS active! (GUI might not work)"
#endif
#ifdef DEBUG_DELAY_STARTUP
  #warning "DELAY_STARTUP is active, do not release this way!"
#endif
#ifdef DEBUG_NO_TONE
  #warning "DEBUG_NO_TONE is defined, do not release this way!"
#endif

#endif
