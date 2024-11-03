/*
	FabiWare - AsTeRICS Foundation
	Copyright (c) Benjamin Aigner
	For more info please visit: https://www.asterics-foundation.org

	Module: bluetooth.h - using external Bluetooth addon for mouse/keyboard control

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; See the GNU General Public License:
	http://www.gnu.org/licenses/gpl-3.0.en.html
*/

//TBD: include Flipmouse version of this module

#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "FlipWare.h"


/**
   @name updateBTConnectionState
   @return none

   periodically polls the BT modue for connections
*/
void updateBTConnectionState();


/**
   @name setBTName
   @param char * BTName: module name for BT-advertising
   @return none

   sets the BT module name for advertising/pairing.

*/
void setBTName(char * BTName);

/**
   @name unpairAllBT
   @return none

   forget all paired devices

*/
void unpairAllBT();

/**
   @name isBluetoothConnected
   @param none
   @return true, if the BT module is connected, false if not

   This method returns true, if the BT module is currently paired+connected to a host device
   False will be returned otherwise
*/
bool isBluetoothConnected();

/**
   @name startBTPairing
   @param none
   @return none
*/
bool startBTPairing();

/**
   @name stopBTPairing
   @param none
   @return none
*/
bool stopBTPairing();
#endif
