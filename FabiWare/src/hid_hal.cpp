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
    #ifndef FLIPMOUSE 
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
    #ifndef FLIPMOUSE 
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
    #ifndef FLIPMOUSE 
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
    #ifndef FLIPMOUSE 
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

  if (x || y) sensorData.mouseMoveTimestamp=millis();

  //as long as one of the axis is out of bounds of int8 (more than one HID report)
  while(x < -128 || x > 127 || y < -128 || y > 127 ) {
    int8_t x_now,y_now = 0;

    if(x < -128) { x_now = -128; x += 128; }
    if(x > 127) { x_now = 127; x -= 127; }

    if(y < -128) { y_now = -128; y += 128; }
    if(y > 127) { y_now = 127; y -= 127; }

    if (slotSettings.bt & 1) Mouse.move(x_now, y_now, 0);
    if (slotSettings.bt & 2) {
      #ifndef FLIPMOUSE 
        MouseBLE.move(x_now, y_now, 0);
        delay(1);
      #else 
        mouseBT(x_now, y_now, 0);
      #endif
    }
  }

  if (slotSettings.bt & 1) Mouse.move(x, y, 0);
  if (slotSettings.bt & 2) {
    #ifndef FLIPMOUSE 
      MouseBLE.move(x, y, 0);
    #else 
      mouseBT(x, y, 0);
    #endif
  }
}

void keyboardPrint(char * keystring)
{
  //Keyboard.print(keystring);  // improved for ISO 8859 compatibility (but: slower ..)
  for (unsigned int i = 0; i < strlen(keystring); i++)
  {
      if (slotSettings.bt & 1) Keyboard.write(keystring[i]);
      if (slotSettings.bt & 2) 
      #ifndef FLIPMOUSE 
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
  #ifndef FLIPMOUSE 
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
  #ifndef FLIPMOUSE 
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
  #ifndef FLIPMOUSE 
    KeyboardBLE.releaseAll();
  #else 
    keyboardBTReleaseAll();
  #endif
}

void joystickAxis(int axis, int value)
{
  // TBD: update the call to the Joystick class when BLE joystick HID report was updated in RP Pico Core
  // currently, axis0 (X) and axis1 (Y) are correct, but axis for the second joystick and sliders are mixed up
  // proposal: use only the axis index, e.g. Joystick.axis0(value) 

  if (slotSettings.bt & 1)
  {
    switch(axis)
    {
      case 0:  Joystick.X(value); break;
      case 1:  Joystick.Y(value); break;
      case 2:  Joystick.Z(value); break;
      case 3:  Joystick.sliderLeft(value); break;
      case 4:  Joystick.sliderRight(value); break;
      case 5:  Joystick.Zrotate(value); break;
    }
  }
  if (slotSettings.bt & 2)
  {
    #ifndef FLIPMOUSE
      #ifdef FABI_BLEJOYSTICK_ENABLED
      switch(select)
      {
        case 0:  JoystickBLE.X(value); break;
        case 1:  JoystickBLE.Y(value); break;
        case 2:  JoystickBLE.Z(value); break;
        case 3:  JoystickBLE.sliderLeft(value); break;
        case 4:  JoystickBLE.sliderRight(value); break;
        case 5:  JoystickBLE.Zrotate(value); break;
      }
      #endif
    #else
      joystickBTAxis(axis, value);
    #endif
  }
}

void joystickButton(uint8_t nr, int val)
{
  if (slotSettings.bt & 1) 
    Joystick.button(nr,val);
  if (slotSettings.bt & 2)
  #ifndef FLIPMOUSE 
    #ifdef FABI_BLEJOYSTICK_ENABLED
    JoystickBLE.button(nr,val);
    #endif
  #else 
    joystickBTButton(nr,val);
  #endif
  ;
}

void joystickHat(int val)
{
  if (slotSettings.bt & 1) 
    Joystick.hat(val);
  if (slotSettings.bt & 2)
  #ifndef FLIPMOUSE 
    #ifdef FABI_BLEJOYSTICK_ENABLED
    JoystickBLE.hat(val);
    #endif
  #else 
    joystickBTHat(val);
  #endif
  ;
}
