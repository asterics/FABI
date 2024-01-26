/*
	FabiWare - AsTeRICS Foundation
	For more info please visit: https://www.asterics-foundation.org

	Module: hid_hal.h - covers USB HID and BT HID send routines

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; See the GNU General Public License:
	http://www.gnu.org/licenses/gpl-3.0.en.html
*/


#ifndef _HID_HAL_H_
#define _HID_HAL_H_

#include "FlipWare.h"

#define DRAG_RECORDING_IDLE 0
#define DRAG_RECORDING_ACTIVE 1

/**
   extern declaration of static variables
   which shall be accessed from other modules
*/
extern uint8_t dragRecordingState;
extern int16_t dragRecordingX;
extern int16_t dragRecordingY;

#define MOUSE_LEFT 0
#define MOUSE_RIGHT 1
#define MOUSE_MIDDLE 2

//general HID report descriptor for every normal use case:
// Linux
// Android
// Windows
uint8_t const desc_hid_0[] =
{
  TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(1) ),
  TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(2) ),
  TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(3) ),
  TUD_HID_REPORT_DESC_GAMEPAD ( HID_REPORT_ID(4) )
};

// special case: iOS (no joystick)
uint8_t const desc_hid_1[] =
{
  TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(1) ),
  TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(2) ),
  TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(3) ),
};

// very special case: XBox adaptive controller (XAC), very picky...
uint8_t const desc_hid_2[] =
{
  TUD_HID_REPORT_DESC_GAMEPAD ( HID_REPORT_ID(1) )
};

/*
   @name setReportIDs
   @param uint8_t rid_m Report ID for the mouse, set to 0 if not used
   @param uint8_t rid_k Report ID for the keyboard, set to 0 if not used. rid_keyboard + 1 is assumed to be the consumer control
   @param uint8_t rid_j Report ID for the joystick, set to 0 if not used
   @return none
*/
void setReportIDs(uint8_t rid_m, uint8_t rid_k, uint8_t rid_j);


/*
   @name sendKeyboard
   @param uint8_t* keys Array of keycodes. Byte 0 contains modifiers, Byte 1 is ignored Byte 2-7 are keycodes.
   @return none

   Sends out a full keyboard report. It is assumed that the bytes inside
   the buffer are properly located:
   [0]   modifier keys (CTRL, GUI, ALT, SHIFT, ...)
   [1]   is ignored (detail: LED OUT in report)
   [2-7] up to 6 different keycodes. use 0 for released key
*/
void sendKeyboard(uint8_t *keys);

/*
   @name keyboardPress
   @param int key keycode to be typed by keyboard
   @return none

   This method presses a key of given keycode (modifiers available!)
*/
void keyboardPress(int key);

/*
   @name keyboardRelease
   @param int key keycode to be released by keyboard
   @return none

   This method releases a key of given keycode (modifiers available!)
*/
void keyboardRelease(int key);

/*
   @name keyboardReleaseAll
   @param none
   @return none

   This method releases all currently pressed keys
*/
void keyboardReleaseAll();

/*
   @name mousePress
   @param uint8_t button	button code which should be pressed.
   @return none

   Press a defined mouse button (MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE)
*/
void mousePress(uint8_t button);

/*
   @name mouseToggle
   @param uint8_t button  button code which should be toggled.
   @return none

   Toggle a defined mouse button (MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE)
*/
void mouseToggle(uint8_t button);

/*
   @name mouseRelease
   @param uint8_t button	button code which should be released.
   @return none

   Release a defined mouse button (MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE)
*/
void mouseRelease(uint8_t button);


/*
   @name mouseScroll
   @param int8_t steps  steps to scroll.
   @return none

   scrolls a number of steps
*/
void mouseScroll(int8_t steps);


/*
   @name mouseMove
   @param int x	 movement in x direction
   @param int y	 movement in y direction
   @return none

   moves the mouse cursor a defined nmber of steps. (if x or y are > 127, multiple HID commands are generated)
*/
void mouseMove(int x, int y);


/*
   @name joystickAxis
   @param int axis1       new value for axis 1 (either X,Z or sliderLeft; set by param select)
   @param int axis2       new value for axis 2 (either Y,Zrotate or sliderRight; set by param select)
   @param uint8_t select  define axis for values (0: X/Y; 1: Z/Zrotate; 2: sliderLeft/sliderRight)

   Updates 2 joystick axis with new values.
   Which axis are sent depends on the select parameter.
   
   @note The range for axis1 & axis2 is 0-1023.
   @note If an axis is set to -1, it will not be updated.
*/
void joystickAxis(int axis1, int axis2, uint8_t select);


/*
   @name joystickButton
   @param uint8_t nr    button number (1-32)
   @param int     val   state for button, 0 released; != 0 pressed

   Update joystick buttons.
*/
void joystickButton(uint8_t nr, int val);


/*
   @name joystickHat
   @param int     val   Hat position, 0-360 or -1

   Update joystick hat: 0-360 for position (mapped to 8 positions); -1 is rest position
*/
void joystickHat(int val);
#endif
