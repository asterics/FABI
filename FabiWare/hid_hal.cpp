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

uint8_t rid_mouse = 0;
uint8_t rid_keyboard = 0;
uint8_t rid_joystick = 0;
uint8_t report_mouse[5];
uint8_t report_kbd[8]; //unused, just used to determine max size of a kbd report. Data is provided when sending
uint8_t report_gamepad[11];

/*Adafruit_USBD_HID usb_hid0(desc_hid_0, sizeof(desc_hid_0), HID_ITF_PROTOCOL_NONE, 2, false);
Adafruit_USBD_HID usb_hid1(desc_hid_1, sizeof(desc_hid_1), HID_ITF_PROTOCOL_NONE, 2, false);
Adafruit_USBD_HID usb_hid2(desc_hid_2, sizeof(desc_hid_2), HID_ITF_PROTOCOL_NONE, 2, false);*/
extern Adafruit_USBD_HID usb_hid;

#warning "BLE disabled, because it isn't compatible with TinyUBS. Implement here & use PicoBluetoothBLEHID"
/*
 * PicoBluetoothBLEHID.send(&report, sizeof(hid_keyboard_report_t) +1);
 * note: here we need the report ID in the first byte!
 * PicoBluetoothBLEHID.startHID(localName, hidName, __BLEGetAppearance(), desc_keyboardBLE, desc_keyboardBLE_length);
*/

/*
   @name setReportIDs
   @param uint8_t rid_m Report ID for the mouse, set to 0 if not used
   @param uint8_t rid_k Report ID for the keyboard, set to 0 if not used. rid_keyboard + 1 is assumed to be the consumer control
   @param uint8_t rid_j Report ID for the joystick, set to 0 if not used
   @return none
*/
void setReportIDs(uint8_t rid_m, uint8_t rid_k, uint8_t rid_j)
{
  rid_mouse = rid_m;
  rid_keyboard = rid_k;
  rid_joystick = rid_j;
}

void mouseRelease(uint8_t button)
{
  if(usb_hid.ready() && (slotSettings.bt & 1) && (rid_mouse != 0)) {
    report_mouse[0] &= ~(1<<button);
    usb_hid.sendReport(rid_mouse,report_mouse,sizeof(report_mouse));
  }
  //if (slotSettings.bt & 2) MouseBLE.release(button);
}

void mousePress(uint8_t button)
{
  if(usb_hid.ready() && (slotSettings.bt & 1) && (rid_mouse != 0)) {
    report_mouse[0] |= (1<<button);
    usb_hid.sendReport(rid_mouse,report_mouse,sizeof(report_mouse));
  }
  //if (slotSettings.bt & 2) MouseBLE.press(button);
}

void mouseToggle(uint8_t button)
{
  if ((slotSettings.bt & 1) && (rid_mouse != 0)) {
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
  if(usb_hid.ready() && (slotSettings.bt & 1) && (rid_mouse != 0)) {
    report_mouse[3] = (uint8_t)steps;
    usb_hid.sendReport(rid_mouse,report_mouse,sizeof(report_mouse));
    report_mouse[3] = 0;
  }
  //if (slotSettings.bt & 2) MouseBLE.move(0, 0, steps);
}

void hidMouseMove(int8_t x, int8_t y) {
  if(usb_hid.ready() && (rid_mouse != 0)) {
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


void sendKeyboard(uint8_t *keys) {
  //always mask out the reserved byte
  keys[1] = 0;
  
  //if you want full debug output.
  //Serial.printf("kbd: 0x%02X,0x%02X; 0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X",keys[0],keys[1],keys[2],keys[3],keys[4],keys[5],keys[6],keys[7]);
  
  if ((slotSettings.bt & 1) && (rid_keyboard != 0)) usb_hid.sendReport(rid_keyboard,keys,sizeof(report_kbd));
  //TODO: send here for BT as well...
}

void joystickAxis(int axis1, int axis2, uint8_t select)
{
  //check if select has correct range
  if(select > 2) return;
  //if axis 1 should be updated (!= -1/<0), map to 8bit value
  if(axis1 >= 0) {
    report_gamepad[select*2] = map(axis1,0,1023,0,255);
  }
  //if axis 2 should be updated (!= -1/<0), map to 8bit value
  if(axis2 >= 0) {
    report_gamepad[select*2+1] = map(axis2,0,1023,0,255);
  }
  //send out reports
  if ((slotSettings.bt & 1) && (rid_joystick != 0)) usb_hid.sendReport(rid_joystick,report_gamepad,sizeof(report_gamepad));
  //TODO: here for BLE now...
  
  //setup data here
  switch(select) {
    case 0:
      if(axis1 >= 0) report_gamepad[0] = map(axis1,0,1023,0,255);
      if(axis2 >= 0) report_gamepad[2] = map(axis2,0,1023,0,255);
      break;
    case 1:
      if(axis1 >= 0) report_gamepad[0] = map(axis1,0,1023,0,255);
      if(axis2 >= 0) report_gamepad[2] = map(axis2,0,1023,0,255);
      break;
  }
}

void joystickButton(uint8_t nr, int val)
{
  //out of range
  if(nr == 0 || nr > 32) return;
  
  //first possible button byte (nr: 1-8)
  if(nr <= 8) {
    if(val) report_gamepad[7] |= (1<<(nr-1));
    else report_gamepad[7] &= ~(1<<(nr-1));
  }
  //second possible button byte (nr: 9-16)
  if(nr >= 9 && nr <= 16) {
    if(val) report_gamepad[8] |= (1<<(nr-9));
    else report_gamepad[8] &= ~(1<<(nr-9));
  }
  //third possible button byte (nr: 17-24)
  if(nr >= 17 && nr <= 24) {
    if(val) report_gamepad[9] |= (1<<(nr-17));
    else report_gamepad[9] &= ~(1<<(nr-17));
  }
  //fourth possible button byte (nr: 25-32)
  if(nr >= 25 && nr <= 32) {
    if(val) report_gamepad[10] |= (1<<(nr-25));
    else report_gamepad[10] &= ~(1<<(nr-25));
  }
  //send out reports
  if ((slotSettings.bt & 1) && (rid_joystick != 0)) usb_hid.sendReport(rid_joystick,report_gamepad,sizeof(report_gamepad));
  //TODO: here for BLE now...
}

void joystickHat(int val)
{
  report_gamepad[6] = val;
  //send out reports
   if ((slotSettings.bt & 1) && (rid_joystick != 0)) usb_hid.sendReport(rid_joystick,report_gamepad,sizeof(report_gamepad));
  //TODO: here for BLE now...
}
