/*
	FabiWare - AsTeRICS Foundation
	Copyright (c) Benjamin Aigner
	For more info please visit: https://www.asterics-foundation.org

	Module: bluetooth.cpp - using external Bluetooth addon for mouse/keyboard control

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; See the GNU General Public License:
	http://www.gnu.org/licenses/gpl-3.0.en.html
*/

//TBD: include Fliümouse version of this module

#include "bluetooth.h"
#include <BTstackLib.h>
//necessary to include keyboard / keyboardlayout.h to have access to the key definitions

static char macaddress[] = "00 00 00 00 00 00";  // len: 18
uint8_t bt_connected = 0;

/**
   @name updateBTConnectionState
   @return none

   periodically polls the BT modue for connections
*/
void updateBTConnectionState () {

}


/**
   @name isBluetoothConnected
   @param none
   @return true, if the BT module is connected (paired) false if not

   This method returns true, if the BT module is currently paired to a host device
   False will be returned otherwise
*/
bool isBluetoothConnected() {
  return(true);
}


/**
   @name setBTName
   @param char * BTName: module name for BT-advertising
   @return none

   sets the BT module name for advertising/pairing.

*/
void setBTName(char * BTName) {

}

/**
   @name unpairAllBT
   @return none

   forget all paired devices

*/
void unpairAllBT() {


}


/**
   @name isBluetoothAvailable
   @param none
   @return true, if the BT module is available, false if not

   This method returns true, if the BT module is available and delivered
   a valid version string
   False will be returned otherwise
*/
bool isBluetoothAvailable()
{
  return true;
}

/**
   @name startBTPairing
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

/**
   @name stopBTPairing
   @param none
   @return none
*/
bool stopBTPairing()
{
  return true;
}
