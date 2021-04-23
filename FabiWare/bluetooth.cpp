/*
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: bluetooth.cpp - using external Bluetooth addon for mouse/keyboard control
     Copyright (c) Benjamin Aigner

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html
 
*/

#include "bluetooth.h"

#define BT_MINIMUM_SENDINTERVAL 20     // reduce mouse reports in BT mode (in milliseconds) !

typedef enum {NONE, EZKEY, MINIBT01, MINIBT02} addontype_t;

uint8_t bt_available = 0;
addontype_t bt_esp32addon = NONE;
uint8_t activeKeyCodes[6];
uint8_t activeModifierKeys = 0;
uint8_t activeMouseButtons = 0;

long btsendTimestamp = millis();

#define Serial_AUX Serial1

//#define DEBUG_OUTPUT_FULL

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
void mouseBT(int x, int y, uint8_t scroll)
{
  static int oldMouseButtons = 0;
  static int sendCnt = 0;
  static int accuX = 0, accuY = 0;

#ifdef DEBUG_OUTPUT_FULL
  Serial.println("BT mouse actions:");
  Serial.print("buttons: 0x");
  Serial.println(activeMouseButtons, HEX);
  Serial.print("x/y/scroll: ");
  Serial.print(x, DEC);
  Serial.print("/");
  Serial.print(y, DEC);
  Serial.print("/");
  Serial.println(scroll, DEC);
#endif

  accuX += x;
  accuY += y;

  if ((activeMouseButtons != oldMouseButtons) ||
      (btsendTimestamp + BT_MINIMUM_SENDINTERVAL <= millis()))
  {
    btsendTimestamp = millis();

    ///@todo refactor this code here, send more efficient mouse report data.
    //starting RAW HID mouse report
    //according to:
    //https://learn.adafruit.com/introducing-bluefruit-ez-key-diy-bluetooth-hid-keyboard/sending-keys-via-serial

    Serial_AUX.write((uint8_t)0xFD);

    //stuffing...
    Serial_AUX.write((uint8_t)0x00);
    Serial_AUX.write((uint8_t)0x03);

    //masked buttons:
    //left: (1<<0)
    //right: (1<<1)
    //middle: (1<<2) (not sure)
    Serial_AUX.write(activeMouseButtons);

    //send x/y relative movement
    Serial_AUX.write((uint8_t)accuX);
    Serial_AUX.write((uint8_t)accuY);

    //maybe the wheel? Not official by Adafruit... -> not working
    Serial_AUX.write((uint8_t)0x00);
    //some additional bytes...
    Serial_AUX.write((uint8_t)0x00);
    Serial_AUX.write((uint8_t)0x00);

    sendCnt = 0;
    accuX = 0; accuY = 0;
    oldMouseButtons = activeMouseButtons;
  }
}

/**
   @name mouseBTPress
   @param mousebutton uint8_t contains all buttons to be pressed (masked): (1<<0) left; (1<<1) right; (1<<2) middle
   @return none
*/
void mouseBTPress(uint8_t mousebutton)
{
  activeMouseButtons |= mousebutton;
  //update buttons without sending relative data for x/y/wheel
  mouseBT(0, 0, 0);
}

/**
   @name mouseBTRelease
   @param mousebutton uint8_t contains all buttons to be release (masked): (1<<0) left; (1<<1) right; (1<<2) middle
   @return none
*/
void mouseBTRelease(uint8_t mousebutton)
{
  activeMouseButtons &= ~mousebutton;
  //update buttons without sending relative data for x/y/wheel
  mouseBT(0, 0, 0);
}

/**
   @name isMouseBTPressed
   @param mousebutton buttons which should be polled
   @return boolan
*/
boolean isMouseBTPressed(uint8_t mousebutton)
{
  if (activeMouseButtons & mousebutton) return true;
  return false;
}


/**

   name: sendBTKeyboardReport
   @param none
   @return none

   Sends a full keyboard report where all keys contained in activeKeyCodes
   and activeModifierKeys will be sent

   @todo Should we send with a different API here when upgrading to ESP32miniBT v0.2?
*/
void sendBTKeyboardReport()
{
#ifdef DEBUG_OUTPUT_FULL
  Serial.println("BT keyboard actions:");
  Serial.print("modifier: 0x");
  Serial.println(activeModifierKeys, HEX);
  Serial.println("activeKeyCodes: ");
  Serial.println(activeKeyCodes[0], HEX);
  Serial.println(activeKeyCodes[1], HEX);
  Serial.println(activeKeyCodes[2], HEX);
  Serial.println(activeKeyCodes[3], HEX);
  Serial.println(activeKeyCodes[4], HEX);
  Serial.println(activeKeyCodes[5], HEX);
#endif

  Serial_AUX.write(0xFD);       			//raw HID
  Serial_AUX.write(activeModifierKeys);  	//modifier keys
  Serial_AUX.write(0x00);
  Serial_AUX.write(activeKeyCodes[0]);	//key 1
  Serial_AUX.write(activeKeyCodes[1]);   	//key 2
  Serial_AUX.write(activeKeyCodes[2]);   	//key 3
  Serial_AUX.write(activeKeyCodes[3]);   	//key 4
  Serial_AUX.write(activeKeyCodes[4]);   	//key 5
  Serial_AUX.write(activeKeyCodes[5]);   	//key 6
}

/**

   name: keyboardBTPress
   @param int key	Keycode which should be pressed. Keycodes are in Arduino format
   @return none

    Press a defined key code.
    keycodes and modifier codes are extracted and sent to addon.
*/
void keyboardBTPress(int key)
{
  //this code is used from Keyboard.cpp from Arduino.
	uint8_t i;
	if (key >= 136) {			// it's a non-printing keyey (not a modifier)
		key = key - 136;
	} else if (key >= 128) {	// it's a modifier keyey
		activeModifierKeys |= (1<<(key-128));
		key = 0;
		//Serial.print("mod:");
		//Serial.println(activeModifierKeys,HEX);
	} else {				// it's a printing keyey
		key = pgm_read_byte(_asciimap + key);
		if (!key) {
			
			return 0;
		}
		//Serial.print("key:");
		//Serial.println(key,HEX);
		if (key & 0x80) {						// it's a capital letter or other character reached with shift
			activeModifierKeys |= 0x02;	// the left shift modifier
			key &= 0x7F;
		}
	}
	
	if (activeKeyCodes[0] != key && activeKeyCodes[1] != key && 
	activeKeyCodes[2] != key && activeKeyCodes[3] != key &&
	activeKeyCodes[4] != key && activeKeyCodes[5] != key) {
	
		for (i=0; i<6; i++) {
			if (activeKeyCodes[i] == 0x00) {
				activeKeyCodes[i] = key;
				break;
			}
		}
	}

  //send the new keyboard report
  sendBTKeyboardReport();
}

/**

   name: keyboardBTRelease
   @param int key	Keycode which should be released. Keycodes are in Arduino Format
   @return none

   Release a defined key code.
*/
void keyboardBTRelease(int key)
{ 
  //this code is used from Keyboard.cpp from Arduino.
	uint8_t i;
	if (key >= 136) {			// it's a non-printing key (not a modifier)
		key = key - 136;
	} else if (key >= 128) {	// it's a modifier key
		activeModifierKeys &= ~(1<<(key-128));
		key = 0;
	} else {				// it's a printing key
		key = pgm_read_byte(_asciimap + key);
		if (!key) {
			return 0;
		}
		if (key & 0x80) {							// it's a capital letter or other character reached with shift
			activeModifierKeys &= ~(0x02);	// the left shift modifier
			key &= 0x7F;
		}
	}
	
	// Test the key report to see if k is present.  Clear it if it exists.
	// Check all positions in case the key is present more than once (which it shouldn't be)
	for (i=0; i<6; i++) {
		if (0 != key && activeKeyCodes[i] == key) {
			activeKeyCodes[i] = 0x00;
		}
	}

  //send the new keyboard report
  sendBTKeyboardReport();
}

/**

   name: keyboardBTReleaseAll
   @param none
   @return none

   Release all previous pressed keyboard keys
*/
void keyboardBTReleaseAll()
{
  //reset all activeKeyCodes to 0x00
  for (uint8_t i = 0; i <= 5; i++) activeKeyCodes[i] = 0;
  //reset all modifier keys
  activeModifierKeys = 0;
  //send a keyboard report (now empty)
  sendBTKeyboardReport();
}

/**

   name: initBluetooth
   @param none
   @return none

   Initialize the Bluetooth module on the external serial port.
   If the module returns a valid version string, BT communication is
   enabled (bt_enable is set to 1).

   @see bt_enable

*/
void initBluetooth()
{
#ifdef DEBUG_OUTPUT_FULL
  Serial.println("init Bluetooth");
#endif

  //start the AUX serial port 9600 8N1
  Serial_AUX.begin(9600);
  bt_available = 1;

  ///@todo send identifier to BT module & check response. With BT addon this is much faster and reliable
  bt_esp32addon = EZKEY;
  Serial_AUX.println("$ID");
  //set BT name to FABI
  Serial_AUX.println("$NAME FABI");
}

/**

   name: isBluetoothAvailable
   @param none
   @return true, if the BT module is available, false if not

   This method returns true, if the BT module is available and delivered
   a valid version string
   False will be returned otherwise
*/
bool isBluetoothAvailable()
{
  return bt_available;
}

/**

   name: startBTPairing
   @param none
   @return none
   @note Not implemented
*/
bool startBTPairing()
{
  //we will send a command to the BT addon board here.
  ///@todo which command & implement on BT addon
  return true;
}
