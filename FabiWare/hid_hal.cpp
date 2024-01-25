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

Adafruit_USBD_HID usb_hid;

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

uint8_t rid_mouse = 0;
uint8_t rid_keyboard = 0;
uint8_t rid_joystick = 0;
uint8_t report_mouse[5];
uint8_t report_kbd[8];
uint8_t report_gamepad[11];

#warning "BLE disabled, because it isn't compatible with TinyUBS. Implement here & use PicoBluetoothBLEHID"
/*
 * PicoBluetoothBLEHID.send(&report, sizeof(hid_keyboard_report_t) +1);
 * note: here we need the report ID in the first byte!
 * PicoBluetoothBLEHID.startHID(localName, hidName, __BLEGetAppearance(), desc_keyboardBLE, desc_keyboardBLE_length);
*/

/*
   @name initHID
   @param uint8_t interface Number of HID interface. Currently: 0 for mouse+kbd+joystick (general); 1 for keyboard+mouse (iOS); 2 for joystick (XAC)
   @return none

   This method prints out an ASCII string (no modifiers available!)
*/
void initHID(uint8_t interface) {
  bool init = true;
  switch(interface) {
    case 0:
      rid_mouse = 1; rid_keyboard = 2; rid_joystick = 4;
      usb_hid.setReportDescriptor(desc_hid_0,sizeof(desc_hid_0));
      usb_hid.setStringDescriptor("FABI Composite");
      break;
    case 1:
      rid_mouse = 1; rid_keyboard = 2;
      usb_hid.setReportDescriptor(desc_hid_1,sizeof(desc_hid_1));
      usb_hid.setStringDescriptor("FABI iOS");
      break;
    case 2:
      rid_joystick = 1;
      usb_hid.setReportDescriptor(desc_hid_2,sizeof(desc_hid_2));
      usb_hid.setStringDescriptor("FABI XAC");
      break;
    default:
      init = false;
      break;
  }
  if(init) {
    usb_hid.begin();
  }
}

void mouseRelease(uint8_t button)
{
  if(usb_hid.ready() && (slotSettings.bt & 1)) {
    report_mouse[0] &= ~(1<<button);
    usb_hid.sendReport(rid_mouse,report_mouse,sizeof(report_mouse));
  }
  //if (slotSettings.bt & 2) MouseBLE.release(button);
}

void mousePress(uint8_t button)
{
  if(usb_hid.ready() && (slotSettings.bt & 1)) {
    report_mouse[0] |= (1<<button);
    usb_hid.sendReport(rid_mouse,report_mouse,sizeof(report_mouse));
  }
  //if (slotSettings.bt & 2) MouseBLE.press(button);
}

void mouseToggle(uint8_t button)
{
  if (slotSettings.bt & 1) {
    if(report_mouse[0] & (1<<button)) report_mouse[0] &= ~(1<<button);
    else report_mouse[0] |= (1<<button);
    if(usb_hid.ready()) usb_hid.sendReport(rid_mouse,report_mouse,sizeof(report_mouse));
  }

  /*if (slotSettings.bt & 2) {
    if (MouseBLE.isPressed(button)) MouseBLE.release(button); 
    else MouseBLE.press(button);
  }*/
}


void mouseScroll(int8_t steps)
{
  if(usb_hid.ready() && (slotSettings.bt & 1)) {
    report_mouse[3] = (uint8_t)steps;
    usb_hid.sendReport(rid_mouse,report_mouse,sizeof(report_mouse));
    report_mouse[3] = 0;
  }
  //if (slotSettings.bt & 2) MouseBLE.move(0, 0, steps);
}

void hidMouseMove(int8_t x, int8_t y) {
  if(usb_hid.ready()) {
    report_mouse[1] = (uint8_t)x;
    report_mouse[2] = (uint8_t)y;
    usb_hid.sendReport(rid_mouse,report_mouse,sizeof(report_mouse));
    report_mouse[1] = report_mouse[2] = 0;
  }
}

void mouseMove(int x, int y)
{
  if (dragRecordingState == DRAG_RECORDING_ACTIVE) {
    dragRecordingX+=x;
    dragRecordingY+=y;
  }
  while (x < -128) {
    if (slotSettings.bt & 1) hidMouseMove(-128, 0);
    //if (slotSettings.bt & 2) MouseBLE.move(-128, 0, 0);
    x += 128;
  }
  while (x > 127) {
    if (slotSettings.bt & 1) hidMouseMove(127,0);
    //if (slotSettings.bt & 2) MouseBLE.move(127, 0, 0);
    x -= 127;
  }

  while (y < -128) {
    if (slotSettings.bt & 1) hidMouseMove(0,-128);
    //if (slotSettings.bt & 2) MouseBLE.move(0, -128, 0);
    y += 128;
  }
  while (y > 127) {
    if (slotSettings.bt & 1) hidMouseMove(0,127);
    //if (slotSettings.bt & 2) MouseBLE.move(0, 127, 0);
    y -= 127;
  }

  if (slotSettings.bt & 1) hidMouseMove(x,y);
  //if (slotSettings.bt & 2) MouseBLE.move(x, y, 0);
}


void keyboardPrint(char * keystring)
{
  //Keyboard.print(keystring);  // improved for ISO 8859 compatibility (but: slower ..)
  for (unsigned int i = 0; i < strlen(keystring); i++)
  {
      /*if (slotSettings.bt & 1) Keyboard.write(keystring[i]);*/
      //if (slotSettings.bt & 2) KeyboardBLE.write(keystring[i]);
  }
}

void keyboardPress(int key)
{
  /*if (slotSettings.bt & 1)
    Keyboard.press(key);
  if (slotSettings.bt & 2)
    KeyboardBLE.press(key);*/
}

void keyboardRelease(int key)
{
  /*if (slotSettings.bt & 1)
    Keyboard.release(key);
  if (slotSettings.bt & 2)
    KeyboardBLE.release(key);/*
}

void keyboardReleaseAll()
{
  /*if (slotSettings.bt & 1)
    Keyboard.releaseAll();
  if (slotSettings.bt & 2)
    KeyboardBLE.releaseAll();*/
}

void joystickAxis(int axis1, int axis2, uint8_t select)
{
  /*if (slotSettings.bt & 1)
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
  }*/
}

void joystickButton(uint8_t nr, int val)
{
  /*if (slotSettings.bt & 1) 
    Joystick.button(nr,val);
  if (slotSettings.bt & 2)
    JoystickBLE.button(nr,val);*/
}

void joystickHat(int val)
{
 /* if (slotSettings.bt & 1) 
    Joystick.hat(val);
  if (slotSettings.bt & 2)
    JoystickBLE.hat(val);*/
}
