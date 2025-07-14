
/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: FlipWare.h  - main header file
     V3.1: FABI V3 and FLipMouse V3 code fusion

   This firmware allows control of HID functions via FABI module and/or AT-commands
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

// uncomment the target device (FLIPMOUSE or FABI or FLIPPAD):
#define FABI
#define MODULE_NAME "FABI"

//#define FLIPMOUSE
//#define MODULE_NAME "FLipmouse"

// #define FLIPAD
// #define MODULE_NAME "FLipPad"

// #define FABIJOYSTICK_ENABLED


// update the version string with every firmware change:
#define VERSION_STRING "v3.7"

#include <Arduino.h>
#include <Wire.h>
#ifndef FLIPMOUSE
  // needed for RP PICO boards
  #include <MouseBLE.h>
  #include <KeyboardBLE.h>
  #ifdef FABIJOYSTICK_ENABLED
    #include <JoystickBLE.h>
  #endif
  #include "lpwFuncs.h"
#endif
#include <Mouse.h>
#include <Keyboard.h>
#include <Joystick.h>
#include <EEPROM.h>
#include <string.h>
#include <stdint.h>
#include "commands.h"
#include "eeprom.h"
#include "buttons.h"
#include "infrared.h"
#include "bluetooth.h"
#include "hid_hal.h"


// Optional Debug Output Control

//#define DEBUG_OUTPUT_FULL      // if full debug output is desired
//#define DEBUG_OUTPUT_MEMORY    // enables eeprom.cpp debugging, showing memory access
//#define DEBUG_OUTPUT_KEYS      // enable keys.cpp debugging, showing key press/release events and keycode lookup
//#define DEBUG_OUTPUT_IR      	 // enable infrared.cpp debugging, showing whats happening on IR recv/send
//#define DEBUG_OUTPUT_SENSORS 	 // enable sensors.cpp debugging, showing whats happening on sensor reading & init
//#define DEBUG_DELAY_STARTUP 	 // enable a 3s delay after Serial.begin and before all the other stuff.
#define DEBUG_ACTIVITY_LED 	   // enable blinking internal led signaling activity (in sensor loop, core1).
//#define DEBUG_PRESSURE_RAWVALUES // raw output of pressure values and filtered output
//#define DEBUG_MPRLS_ERRORFLAGS // continously print error flags of MPRLS
//#define DEBUG_BATTERY_MANAGEMENT 	 // enable a debug output for battery state detection and management.


/**
   global constant definitions
*/
#ifdef FLIPMOUSE
  #define UPDATE_INTERVAL     8    // update interval for performing HID actions (in milliseconds)
#else
  #define UPDATE_INTERVAL     5    // update interval for performing HID actions (in milliseconds)
#endif

#define DEFAULT_CLICK_TIME  8    // time for mouse click (loop iterations from press to release)
#define CALIBRATION_PERIOD  1000  // approx. 1000ms calibration time
#define BATTERY_UPDATE_INTERVAL  500  // update interval for battery management functions (in milliseconds)
#define GAMEPAD_MINIMUM_SEND_INTERVAL 15 // minimum time between two gamepad axis updates (in milliseconds)

// RAM buffers and memory constraints
#define WORKINGMEM_SIZE         300    // reserved RAM for working memory (command parser, IR-rec/play)
#define MAX_KEYSTRING_LEN (WORKINGMEM_SIZE-3)   // maximum length for AT command parameters
#define MAX_NAME_LEN  15               // maximum length for a slotname or ir name
#define MAX_KEYSTRINGBUFFER_LEN 500    // maximum length for all string parameters of one slot

// direction identifiers
#define DIR_E   1   // east
#define DIR_NE  2   // north-east
#define DIR_N   3   // north
#define DIR_NW  4   // north-west
#define DIR_W   5   // west
#define DIR_SW  6   // sout-west
#define DIR_S   7   // south
#define DIR_SE  8   // south-east

//supported I2C addresses, these are scanned for changes during runtime (plugging / unplugging devices)
//@note End this list with 0x00!
const uint8_t supported_devices[] = {0x3C /*OLED*/, 0x77 /*DPS310*/, 0x18 /*MPRLS*/, 0x2A /* NAU78ß02 */, 0x00 /* END */};

/**
   GlobalSettings struct
   contains general parameters which apply to all slots
*/
struct GlobalSettings {
  uint8_t  buzzerMode;   // tone output mode via internal buzzer: 0=disable, 1=only height, 2=height and count
  uint16_t audioVolume;  // gain for audio sample values (0-200%, 0=deactivate audio output)
  uint16_t thresholdAutoDwell;     // threshold time for automatic creation of a left mouse click after mouse movement
  uint16_t thresholdLongPress;     // threshold time for button long-press
  uint16_t thresholdMultiPress;    // threshold time for button multi-press

  /*  TBD: include those here ??
  uint16_t ts;     // threshold sip
  uint16_t tp;     // threshold puff
  uint16_t sp;     // threshold strong puff
  uint16_t ss;     // threshold strong sip
  uint16_t ro;     // orientation (0,90,180,270)
  uint16_t ap;     // antitremor press time 
  uint16_t ar;     // antitremor release time 
  uint16_t ai;     // antitremor idle time
  */
};

/**
   SlotSettings struct
   contains parameters for current slot
*/
struct SlotSettings {

  char slotName[MAX_NAME_LEN];   // slotname (@warning: must be always the first element, storing relies on that!)
  uint16_t keystringBufferLen;   
  
  uint8_t  stickMode;  // alternative(0), mouse(1), joystick (2,3,4)
  uint8_t  ax;     // acceleration x
  uint8_t  ay;     // acceleration y
  int16_t  dx;     // deadzone x
  int16_t  dy;     // deadzone y
  uint16_t ms;     // maximum speed
  uint16_t ac;     // acceleration time
  uint16_t ts;     // threshold sip
  uint16_t tp;     // threshold puff
  uint8_t  ws;     // wheel stepsize
  uint16_t sp;     // threshold strong puff
  uint16_t ss;     // threshold strong sip
  uint16_t ro;     // orientation (0,90,180,270)
  uint8_t  bt;     // bt-mode (0,1,2)
  uint8_t  sb;     // sensorboard-profileID (0,1,2,3)
  uint32_t sc;     // slotcolor (0x: rrggbb)
  char kbdLayout[6];
};


/**
   SensorData structs
   contain working data of sensors (raw and processed values)
*/
struct SensorData {
  int x, y, xRaw,yRaw;
  int pressure;
  float deadZone, force, forceRaw, angle;
  uint8_t dir;
  int8_t autoMoveX,autoMoveY;
  uint32_t mouseMoveTimestamp;
  uint32_t clickReleaseTimestamp;
  int xLocalMax, yLocalMax;  
  int8_t currentBattPercent, MCPSTAT;
  bool usbConnected;
};

/**
   extern declarations of functions and data structures 
   which can be accessed from different modules
*/

extern alarm_pool_t *app_alarm_pool;
extern char moduleName[];
extern uint8_t actSlot;
extern uint8_t addonUpgrade;
extern struct GlobalSettings globalSettings;
extern struct SlotSettings slotSettings; 
extern struct SensorData sensorData;
extern struct CurrentSensorDataCore1 currentSensorDataCore1;
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
 * Non-blocking delay helper using millis(), but in a single line
 * Usage:
 *  NB_DELAY_START(suffix, interval)
 *    //some code
 *  NB_DELAY_END
 */
#define NB_DELAY_START(suffix, interval) \
  unsigned long currentMillis##suffix = millis(); \
  static unsigned long previousMillis##suffix = 0; \
  if (currentMillis##suffix - previousMillis##suffix >= interval) { \
    previousMillis##suffix = currentMillis##suffix;
#define NB_DELAY_END }

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
#ifdef DEBUG_PRESSURE_RAWVALUES
  #warning "DEBUG_PRESSURE_RAWVALUES is defined, do not release this way!"
#endif
#ifdef DEBUG_MPRLS_ERRORFLAGS
  #warning "DEBUG_MPRLS_ERRORFLAGS is defined, do not release this way!"
#endif

#endif
