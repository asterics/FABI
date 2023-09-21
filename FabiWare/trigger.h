
/*
    FabiWare / FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: trigger.h - header file for the trigger handling

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "FlipWare.h"

#define TRIGGER_MAX_COUNT 30

/***
 * @brief Trigger type, different input types
 * 
 * Select an input option for the trigger,
 * basic ones are defined here, might be extended in the future
 * 
 * 
 */
typedef enum {
  BUTTON_PRESS = 0,
  BUTTON_RELEASE,
  PAD_STICK,
  PRESSURE,
  IMU
} trigger_type;

/***
 * @brief One fully defined trigger
 * 
 * Contains all necessary settings for a trigger:
 * * [id] Consecutive number as ID, starting with 0
 * * [preTrigger] ID of trigger which needs to be fired before this one can be triggered. -1 if not necessary
 * * [type] Type of trigger, see trigger_type
 * * [subtype] Subtype, see trigger_type for explanation
 * * [count] How often needs to be triggered before action is handled (e.g. double press)
 * * [debounce] Debounce timeout [ms] for this trigger. If very long, it is used for longpress.
 * * [timeout_multi] If count is >1, this timeout is considered as maximum time for the given count
 * * [action] AT command from commands.h which will be handled if this is triggered
 */
typedef struct {
  int8_t id,
  int8_t preTrigger,
  trigger_type type,
  int8_t subtype,
  int8_t count,
  int16_t debounce,
  int16_t timeout_multi, 
  int16_t timeout_long,
  String action
} trigger_t;

/*** Setup the trigger structs */
void trigger_setup(void);
/*** Service the trigger functionality, should be called periodically */
void trigger_service(void);

/***
 * @brief Add a trigger in compatibility mode
 * 
 * This function adds a trigger, with the old AT BM / AT ... format.
 * Example:
 * "AT BM 01"
 * "AT CL"
 * -> "AT TI 0:-1:0:1:1:50:0:0:CL"
 */
void trigger_add_compatibility(uint8_t bm, String action);

/*** Get the old compatiblity action string for a "AT BM" */
String trigger_get_compatibility(uint8_t bm);

/*** Get the trigger string for a given ID, e.g. "AT TI 0:-1:0:1:1:50:0:0:CL" */
String trigger_get(int8_t id);

/*** Get the last set trigger ID */
int8_t trigger_get_last_id(void);

/*** Set a trigger. Will be overwritten if already set 
 * Returns true if OK, false on error */
bool trigger_add(String trigger);

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
uint32_t get_capabilities(void);
