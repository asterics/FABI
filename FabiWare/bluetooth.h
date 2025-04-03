/*
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: bluetooth.h - using external Bluetooth addon for mouse/keyboard control
     Copyright (c) Benjamin Aigner

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html
 
*/

#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "fabi.h"

//declarations used from Arduinos Keyboard library
#include <Keyboard.h>
#include "KeyboardLayout.h"


#define BTMODULE_UPGRADE_IDLE 0
#define BTMODULE_UPGRADE_START 1
#define BTMODULE_UPGRADE_RUNNING 2


//RX/TX3 are used to communicate with an addon board (mounted on AUX header)
#define Serial_AUX Serial3

/**

   name: mouseBT
   @þaram x relative movement x axis
   @þaram y relative movement y axis
   @þaram scroll relative scroll actions
   @return

   this method sends a mouse command via the Bluetooth module.
   Mouse movements, buttons and scroll wheel.
   The limit for the movement is +127/-127
*/
void mouseBT(int x, int y, uint8_t scroll);


/**
   @name mouseBTPress
   @param mousebutton uint8_t contains all buttons to be pressed (masked): (1<<0) left; (1<<1) right; (1<<2) middle
   @return none
*/
void mouseBTPress(uint8_t mousebutton);

/**
   @name mouseBTRelease
   @param mousebutton uint8_t contains all buttons to be release (masked): (1<<0) left; (1<<1) right; (1<<2) middle
   @return none
*/
void mouseBTRelease(uint8_t mousebutton);


/**
   @name isMouseBTPressed
   @param mousebutton buttons which should be polled
   @return boolan
*/
boolean isMouseBTPressed(uint8_t mousebutton);

/**

   name: initBluetooth
   @param none
   @return none

   Initialize the Bluetooth module on the external serial port.
   If the module returns a valid version string, BT communication is
   enabled (bt_enable is set to 1).

   @see bt_enable

*/
void initBluetooth();

/**

   name: keyboardBTPrint
   @param char* writeString	string to typed by the Bluetooth HID keyboard
   @return none

   This method prints out an ASCII string (no modifiers available!!!) via the
   Bluetooth module

   @todo We should use the keyboard maps from ESP32, can store all of them. But how to handle any multibyte strings?
*/
void keyboardBTPrint(char * writeString);

/**

   name: keyboardBTReleaseAll
   @param none
   @return none

   Release all previous pressed keyboard keys
*/
void keyboardBTReleaseAll();

/**

   name: keyboardBTPress
   @param int key	Keycode which should be pressed. Keycodes are in Teensy format
   @return none

    Press a defined key code.
    keycodes and modifier codes are extracted and sent to EZ-Key module via UART
    for keylayouts see: https://github.com/PaulStoffregen/cores/blob/master/teensy/keylayouts.h
*/
void keyboardBTPress(int key);

/**

   name: keyboardBTRelease
   @param int key	Keycode which should be released. Keycodes are in Teensy format (16bit, divided into consumer keys, systemkeys & keyboard keys)
   @return none

   Release a defined key code.
*/
void keyboardBTRelease(int key);

/**

   name: isBluetoothAvailable
   @param none
   @return true, if the BT module is available, false if not

   This method returns true, if the BT module is available and delivered
   a valid version string
   False will be returned otherwise
*/
bool isBluetoothAvailable();

/**

   name: startBTPairing
   @param none
   @return none
   @note Not implemented
*/
bool startBTPairing();


/**

   name: performAddonUpgrade
   @param none
   @return none

   handle states and data transfer for BT-Addon firmware update
*/
void performAddonUpgrade();

#endif
