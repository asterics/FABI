/*
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: hid_hal.cpp - covers USB HID and BT HID send routines

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html
 
*/


#include "hid_hal.h"

void mouseRelease(uint8_t button)
{
  if (settings.bt & 1)
    Mouse.release(button);

  if ((settings.bt & 2) && (isBluetoothAvailable()))
    mouseBTRelease(button);
}

void mousePress(uint8_t button)
{
  if (settings.bt & 1)
    Mouse.press(button);

  if ((settings.bt & 2) && (isBluetoothAvailable()))
    mouseBTPress(button);
}

void mouseToggle(uint8_t button)
{
  if (settings.bt & 1) {
    if (Mouse.isPressed(button))
      Mouse.release(button); else Mouse.press(button);
  }

  if ((settings.bt & 2) && (isBluetoothAvailable())) {
    if (isMouseBTPressed(button))
      mouseBTRelease(button); else mouseBTPress(button);
  }
}


void mouseScroll(int8_t steps)
{
  if (settings.bt & 1)
    #ifdef ARDUINO_PRO_MICRO
      Mouse.move(0,0,steps); 
    #else
      Mouse.scroll(steps); 
    #endif

  if ((settings.bt & 2) && (isBluetoothAvailable()))
    mouseBT(0, 0, steps);
}

void mouseMove(int x, int y)
{
  while (x < -128) {
    if (settings.bt & 1)
      Mouse.move(-128, 0);
    if ((settings.bt & 2) && (isBluetoothAvailable()))
      mouseBT(-128, 0, 0);
    x += 128;
  }
  while (x > 127) {
    if (settings.bt & 1)
      Mouse.move(127, 0);
    if ((settings.bt & 2) && (isBluetoothAvailable()))
      mouseBT(127, 0, 0);
    x -= 127;
  }

  while (y < -128) {
    if (settings.bt & 1)
      Mouse.move(0, -128);
    if ((settings.bt & 2) && (isBluetoothAvailable()))
      mouseBT(0, -128, 0);
    y += 128;
  }
  while (y > 127) {
    if (settings.bt & 1)
      Mouse.move(0, 127);
    if ((settings.bt & 2) && (isBluetoothAvailable()))
      mouseBT(0, 127, 0);
    y -= 127;
  }

  if (settings.bt & 1)
    Mouse.move(x, y);
  if ((settings.bt & 2) && (isBluetoothAvailable()))
    mouseBT(x, y, 0);
}

void keyboardPrint(char * keystring)
{
  if (settings.bt & 1) {
    //Keyboard.print(keystring);  // improved for ISO 8859 compatibility (but: slower ..)
    for (int i = 0; i < strlen(keystring); i++)
    {
      Keyboard.press(keystring[i]);
      Keyboard.release(keystring[i]);
    }
  }

  if ((settings.bt & 2) && (isBluetoothAvailable()))
    keyboardBTPrint(keystring);  // TODO: check ISO8859-compatibility
}

void keyboardPress(int key)
{
  if (settings.bt & 1)
    Keyboard.press(key);
  if ((settings.bt & 2) && (isBluetoothAvailable()))
    keyboardBTPress(key);
}

void keyboardRelease(int key)
{
  if (settings.bt & 1)
    Keyboard.release(key);
  if ((settings.bt & 2) && (isBluetoothAvailable()))
    keyboardBTRelease(key);
}

void keyboardReleaseAll()
{
  if (settings.bt & 1)
    Keyboard.releaseAll();
  if ((settings.bt & 2) && (isBluetoothAvailable()))
    keyboardBTReleaseAll();
}
