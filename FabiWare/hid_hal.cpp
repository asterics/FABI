/*
	FabiWare - AsTeRICS Foundation
	For more info please visit: https://www.asterics-foundation.org

	Module: hid_hal.cpp - covers USB HID and BT HID send routines

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; See the GNU General Public License:
	http://www.gnu.org/licenses/gpl-3.0.en.html
*/

#include "hid_hal.h"

uint8_t dragRecordingState=DRAG_RECORDING_IDLE;
int16_t dragRecordingX=0;
int16_t dragRecordingY=0;

void mouseRelease(uint8_t button)
{
  if (slotSettings.bt & 1)
    Mouse.release(button);

  if (slotSettings.bt & 2)
    #ifdef FABI 
      MouseBLE.release(button); 
    #else 
      mouseBTRelease(button);
    #endif
}

void mousePress(uint8_t button)
{
  if (slotSettings.bt & 1)
    Mouse.press(button);

  if (slotSettings.bt & 2)
    #ifdef FABI 
      MouseBLE.press(button);
    #else 
      mouseBTPress(button);
    #endif
}

void mouseToggle(uint8_t button)
{
  if (slotSettings.bt & 1) {
    if (Mouse.isPressed(button))
      Mouse.release(button); else Mouse.press(button);
  }

  if (slotSettings.bt & 2) {
    #ifdef FABI 
      if (MouseBLE.isPressed(button))
        MouseBLE.release(button); else MouseBLE.press(button);
    #else 
      if (isMouseBTPressed(button))
        mouseBTRelease(button); else mouseBTPress(button);
    #endif
  }
}


void mouseScroll(int8_t steps)
{
  if (slotSettings.bt & 1)
    Mouse.move(0,0,steps);

  if (slotSettings.bt & 2)
    #ifdef FABI 
      MouseBLE.move(0, 0, steps);
    #else 
      mouseBT(0, 0, steps);
    #endif
}

void mouseMove(int x, int y)
{
  if (dragRecordingState == DRAG_RECORDING_ACTIVE) {
    dragRecordingX+=x;
    dragRecordingY+=y;
  }
  while (x < -128) {
    if (slotSettings.bt & 1)
      Mouse.move(-128, 0, 0);
    if (slotSettings.bt & 2)
    #ifdef FABI 
      MouseBLE.move(-128, 0, 0);
    #else 
      mouseBT(-128, 0, 0);
    #endif
    x += 128;
  }
  while (x > 127) {
    if (slotSettings.bt & 1)
      Mouse.move(127, 0, 0);
    if (slotSettings.bt & 2)
    #ifdef FABI 
      MouseBLE.move(127, 0, 0);
    #else 
      mouseBT(127, 0, 0);
    #endif
    x -= 127;
  }

  while (y < -128) {
    if (slotSettings.bt & 1)
      Mouse.move(0, -128, 0);
    if (slotSettings.bt & 2)
    #ifdef FABI 
      MouseBLE.move(0, -128, 0);
    #else 
      mouseBT(0, -128, 0);
    #endif
    y += 128;
  }
  while (y > 127) {
    if (slotSettings.bt & 1)
      Mouse.move(0, 127, 0);
    if (slotSettings.bt & 2)
    #ifdef FABI 
      MouseBLE.move(0, 127, 0);
    #else 
      mouseBT(0, 127, 0);
    #endif
    y -= 127;
  }

  if (slotSettings.bt & 1)
    Mouse.move(x, y, 0);
  if (slotSettings.bt & 2)
    #ifdef FABI 
      MouseBLE.move(x, y, 0);
    #else 
      mouseBT(x, y, 0);
    #endif
}

void keyboardPrint(char * keystring)
{
  //Keyboard.print(keystring);  // improved for ISO 8859 compatibility (but: slower ..)
  for (unsigned int i = 0; i < strlen(keystring); i++)
  {
      if (slotSettings.bt & 1) Keyboard.write(keystring[i]);
      if (slotSettings.bt & 2) 
      #ifdef FABI 
        KeyboardBLE.write(keystring[i]);
      #else 
        keyboardBTPrint(keystring);  // TODO: check ISO8859-compatibility
      #endif
  }
}

void keyboardPress(int key)
{
  if (slotSettings.bt & 1)
    Keyboard.press(key);
  if (slotSettings.bt & 2)
  #ifdef FABI 
    KeyboardBLE.press(key);
  #else 
    keyboardBTPress(key);
  #endif
}

void keyboardRelease(int key)
{
  if (slotSettings.bt & 1)
    Keyboard.release(key);
  if (slotSettings.bt & 2)
  #ifdef FABI 
    KeyboardBLE.release(key);
  #else 
    keyboardBTRelease(key);
  #endif
}

void keyboardReleaseAll()
{
  if (slotSettings.bt & 1)
    Keyboard.releaseAll();
  if (slotSettings.bt & 2)
  #ifdef FABI 
    KeyboardBLE.releaseAll();
  #else 
    keyboardBTReleaseAll();
  #endif
}

void joystickAxis(int axis1, int axis2, uint8_t select)
{
  if (slotSettings.bt & 1)
  {
    switch(select)
    {
      case 0:
        Joystick.X(axis1);
        Joystick.Y(axis2);
      break;
      case 1:
        Joystick.Z(axis1);
        Joystick.Zrotate(axis2);
      break;
      case 2:
        Joystick.sliderLeft(axis1);
        Joystick.sliderRight(axis2);
      break;
      default: break;
    }
  }
  if (slotSettings.bt & 2)
  {
    #ifdef FABI
      switch(select)
      {
        case 0:
          JoystickBLE.X(axis1);
          JoystickBLE.Y(axis2);
        break;
        case 1:
          JoystickBLE.Z(axis1);
          JoystickBLE.Zrotate(axis2);
        break;
        case 2:
          JoystickBLE.sliderLeft(axis1);
          JoystickBLE.sliderRight(axis2);
        break;
        default: break;
      }
    #else
      joystickBTAxis(axis1, axis2, select);
    #endif
  }
}

void joystickButton(uint8_t nr, int val)
{
  if (slotSettings.bt & 1) 
    Joystick.button(nr,val);
  if (slotSettings.bt & 2)
  #ifdef FABI 
    JoystickBLE.button(nr,val);
  #else 
    joystickBTButton(nr,val);
  #endif
}

void joystickHat(int val)
{
  if (slotSettings.bt & 1) 
    Joystick.hat(val);
  if (slotSettings.bt & 2)
  #ifdef FABI 
    JoystickBLE.hat(val);
  #else 
    joystickBTHat(val);
  #endif
}
