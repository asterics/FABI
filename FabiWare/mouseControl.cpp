/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: mouse.h - mouse movement and click handling
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "fabi.h"
#include "mouseControl.h"


int clickTime = DEFAULT_CLICK_TIME;
int8_t moveX = 0;
int8_t moveY = 0;
uint8_t moveXcnt = 0;
uint8_t moveYcnt = 0;
uint8_t leftMouseButton = 0, old_leftMouseButton = 0;
uint8_t middleMouseButton = 0, old_middleMouseButton = 0;
uint8_t rightMouseButton = 0, old_rightMouseButton = 0;
uint8_t leftClickRunning = 0;
uint8_t rightClickRunning = 0;
uint8_t middleClickRunning = 0;
uint8_t doubleClickRunning = 0;
uint32_t mouseMoveTimestamp = 0;

uint8_t movementCounter = 0;

/**
   @name updateMouse
   @param none
   @return none
   
   update periodic mouse movements and mouse clicks

*/
void updateMouse() {
      
    if (settings.ad && mouseMoveTimestamp) {
      if (millis() - mouseMoveTimestamp >= settings.ad) {
        #ifdef DEBUG_OUTPUT
           Serial.println("Autodwell Click");
        #endif
        leftMouseButton = 1;  leftClickRunning = DEFAULT_CLICK_TIME;
        mouseMoveTimestamp = 0;
      }
    }

    // perform mouse movement which is induced by button actions
    if (moveX == 0) moveXcnt = 0;
    if (moveY == 0) moveYcnt = 0;
    if ((moveX != 0) || (moveY != 0)) {
      if (movementCounter++ % MOUSE_MOVEMENT_PRESCALER == 0) {
        if (moveX != 0) if (moveXcnt < MOUSE_ACCELDELAY) moveXcnt++;
        if (moveY != 0) if (moveYcnt < MOUSE_ACCELDELAY) moveYcnt++;
        mouseMove(moveX * moveXcnt / MOUSE_ACCELDELAY, moveY * moveYcnt / MOUSE_ACCELDELAY);
      }
    }

    // handle running mouse clicks or double clicks
    if (leftClickRunning)
      if (--leftClickRunning == 0)  leftMouseButton = 0;

    if (rightClickRunning)
      if (--rightClickRunning == 0)  rightMouseButton = 0;

    if (middleClickRunning)
      if (--middleClickRunning == 0)  middleMouseButton = 0;

    if (doubleClickRunning) {
      doubleClickRunning--;
      if (doubleClickRunning == clickTime * 2)  leftMouseButton = 0;
      else if (doubleClickRunning == clickTime)    leftMouseButton = 1;
      else if (doubleClickRunning == 0)    leftMouseButton = 0;
    }

    // if any changes were made, update the Mouse buttons
    if (leftMouseButton != old_leftMouseButton) {
      if (leftMouseButton) mousePress(MOUSE_LEFT); else mouseRelease(MOUSE_LEFT);
      old_leftMouseButton = leftMouseButton;
    }
    if  (middleMouseButton != old_middleMouseButton) {
      if (middleMouseButton) mousePress(MOUSE_MIDDLE); else mouseRelease(MOUSE_MIDDLE);
      old_middleMouseButton = middleMouseButton;
    }
    if  (rightMouseButton != old_rightMouseButton)  {
      if (rightMouseButton) mousePress(MOUSE_RIGHT); else mouseRelease(MOUSE_RIGHT);
      old_rightMouseButton = rightMouseButton;
    }
}
