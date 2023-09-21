
/*
    FabiWare / FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: trigger.c - file for the trigger handling

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "trigger.h"


/*** Setup the trigger structs */
void trigger_setup(void)
{
  
}
/*** Service the trigger functionality, should be called periodically */
void trigger_service(void)
{
  
}

/***
 * @brief Add a trigger in compatibility mode
 * 
 * This function adds a trigger, with the old AT BM / AT ... format.
 * Example:
 * "AT BM 01"
 * "AT CL"
 * -> "AT TI 0:-1:0:1:1:50:0:0:CL"
 */
void trigger_add_compatibility(uint8_t bm, String action)
{
  
}

/*** Get the old compatiblity action string for a "AT BM" */
String trigger_get_compatibility(uint8_t bm)
{
  
}

/*** Get the trigger string for a given ID, e.g. "AT TI 0:-1:0:1:1:50:0:0:CL" */
String trigger_get(int8_t id)
{
  
}

/*** Get the last set trigger ID */
int8_t trigger_get_last_id(void)
{
  
}

/*** Set a trigger. Will be overwritten if already set 
 * Returns true if OK, false on error */
bool trigger_add(String trigger)
{
  
}

/*** Get the capabilities of this device.
 * @note This function should be located somewhere else and is currently fixed.
 * 
 * Normally this should return following bitmask if functionality is given
bit 31 sip & puff
bit 30 sip & puff 2
bit 29 stick
bit 28 stick 2
bit 27 pad
bit 26 pad 2
bit 25 accel
bit 24 gyro
bit 23 magnetometer
bit 21 accel 2
bit 20 gyro 2
bit 19 magnetometer 2
bit 0-15 buttons
*/
uint32_t get_capabilities(void)
{
  //we are on FABI3
  if(String(moduleName).compareTo("FABI") == 0) {
    //TODO: we need to get the infos if pressure sensor or IMU is mounted!
    //button count is also fixed to 5 for FABI3.
    return 0x0000001F;
  }
  //we are on FLipMouse3
  if(String(moduleName).compareTo("FLipmouse") == 0) {
    //TODO: we need to get the infos if IMU is mounted (currently not implemented)
    //we have: sip&puff, stick and 3 buttons
    return 0xA0000007;
  }
}
