/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: modes.cpp: implementation of stick operation and special modes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "FlipWare.h"
#include "modes.h"
#include "gpio.h"
#include "tone.h"
#include "utils.h"

/**
   static variables for mode handling
 * */
uint8_t strongSipPuffState = STRONG_MODE_IDLE;
uint8_t mouseMoveCount = 0;
unsigned long currentTime;
unsigned long previousTime = 0;


/**
   forward declarations of module-internal functions
*/
void handleMovement(); 

void handleUserInteraction()
{
  static uint8_t pressureRising = 0, pressureFalling = 0;
  static int previousPressure = 512;
  static int waitStable = 0;
  static int checkPairing = 0;
  static uint8_t puffState = SIP_PUFF_STATE_IDLE, sipState = SIP_PUFF_STATE_IDLE;
  static uint8_t puffCount = 0, sipCount = 0;
  uint8_t isHandled = 0;
  int strongDirThreshold;

  // check sip/puff activities
  if (sensorData.pressure > previousPressure) pressureRising = 1; else pressureRising = 0;
  if (sensorData.pressure < previousPressure) pressureFalling = 1; else pressureFalling = 0;
  previousPressure = sensorData.pressure;

  strongDirThreshold = STRONGMODE_MOUSE_JOYSTICK_THRESHOLD;

  // handle strong sip and puff actions
  switch (strongSipPuffState)  {

    case STRONG_MODE_IDLE:   // IDLE
      if (sensorData.pressure > slotSettings.sp) {
        strongSipPuffState = STRONG_MODE_ENTER_STRONGPUFF;
        makeTone(TONE_ENTER_STRONGPUFF, 0 );
      }
      if (sensorData.pressure < slotSettings.ss ) {
        strongSipPuffState = STRONG_MODE_ENTER_STRONGSIP;
        makeTone(TONE_ENTER_STRONGSIP, 0 );
      }
      break;

    case STRONG_MODE_ENTER_STRONGPUFF:     // puffed strong, wait for release
      if (sensorData.pressure < slotSettings.tp)
        waitStable++;
      else waitStable = 0;
      if (waitStable >= STRONGMODE_STABLETIME)
        strongSipPuffState = STRONG_MODE_STRONGPUFF_ACTIVE;
      break;

    case STRONG_MODE_STRONGPUFF_ACTIVE:    // strong puff mode active
      isHandled = 0;
      //check if strong-puff + button 2/3/4/5 pressed
      ///@note if changing, check for indices of STRONGPUFF_x_BUTTON, i & input map!
      for(uint8_t i = 1; i<=4; i++) {
        if(digitalRead(input_map[i]) == LOW) {
          makeTone(TONE_EXIT_STRONGPUFF, 0 );
          handlePress(STRONGPUFF_2_BUTTON + i - 1);
          handleRelease(STRONGPUFF_2_BUTTON + i - 1);
          strongSipPuffState = STRONG_MODE_RETURN_TO_IDLE;
          waitStable = 0;
          isHandled = 1;
          break;
        }
      }
      if(!isHandled) {
        waitStable++;
        if (waitStable > STRONGMODE_EXIT_TIME) { // no stick movement occurred: perform strong puff action
          waitStable = 0;
          handlePress(STRONGPUFF_BUTTON);
          handleRelease(STRONGPUFF_BUTTON);
          strongSipPuffState = STRONG_MODE_RETURN_TO_IDLE;
        }
      }
      break;

    case STRONG_MODE_ENTER_STRONGSIP:   // sipped strong, wait for release
      if (sensorData.pressure > slotSettings.ts)
        waitStable++;
      else waitStable = 0;
      if (waitStable >= STRONGMODE_STABLETIME)
        strongSipPuffState = STRONG_MODE_STRONGSIP_ACTIVE;
      break;

    case STRONG_MODE_STRONGSIP_ACTIVE:   // strong sip mode active
      isHandled = 0;
      //check if strong-sip + button 2/3/4/5 pressed
      ///@note if changing, check for indices of STRONGSIP_x_BUTTON, i & input map!
      for(uint8_t i = 1; i<=4; i++) {
        if(digitalRead(input_map[i]) == LOW) {
          makeTone(TONE_EXIT_STRONGSIP, 0 );
          handlePress(STRONGSIP_2_BUTTON + i - 1);
          handleRelease(STRONGSIP_2_BUTTON + i - 1);
          strongSipPuffState = STRONG_MODE_RETURN_TO_IDLE;
          waitStable = 0;
          isHandled = 1;
          break;
        }
      }
      if(!isHandled) {
        waitStable++;
        if (waitStable > STRONGMODE_EXIT_TIME) { // no stick movement occurred: perform strong puff action
          waitStable = 0;
          handlePress(STRONGSIP_BUTTON);
          handleRelease(STRONGSIP_BUTTON);
          strongSipPuffState = STRONG_MODE_RETURN_TO_IDLE;
        }
      }    
      break;

    case STRONG_MODE_RETURN_TO_IDLE:
      waitStable++;
      if (waitStable > STRONGMODE_IDLE_TIME)
      {
        waitStable = 0;
        strongSipPuffState = STRONG_MODE_IDLE;
        initDebouncers();
        puffState = 0; sipState = 0;
      }
      break;
    default: break;
  }


  if (strongSipPuffState == STRONG_MODE_IDLE)
  {

    //handle normal sip and puff actions
    switch (puffState)  {
      case SIP_PUFF_STATE_IDLE:
        if (sensorData.pressure > slotSettings.tp)   // handle single puff actions
        {
          makeTone(TONE_INDICATE_PUFF, 0);
          puffState = SIP_PUFF_STATE_STARTED; puffCount = 0;
        }
        break;

      case SIP_PUFF_STATE_STARTED:
        if (!pressureRising)
        {
          if (puffCount++ > SIP_PUFF_SETTLE_TIME)
          {
            puffCount = MIN_HOLD_TIME;
            handlePress(PUFF_BUTTON);
            puffState = SIP_PUFF_STATE_PRESSED;
          }
        } else if (puffCount) puffCount--;
        break;

      case SIP_PUFF_STATE_PRESSED:
        if (puffCount) puffCount--;
        if ((sensorData.pressure < slotSettings.tp) && (!puffCount)) {
          handleRelease(PUFF_BUTTON);
          puffState = 0;
        }
    }

    switch (sipState)  {
      case SIP_PUFF_STATE_IDLE:
        if (sensorData.pressure < slotSettings.ts)   // handle single sip actions
        {
          makeTone(TONE_INDICATE_SIP, 0);
          sipState = SIP_PUFF_STATE_STARTED; sipCount = 0;
        }
        break;

      case SIP_PUFF_STATE_STARTED:
        if (!pressureFalling)
        {
          if (sipCount++ > SIP_PUFF_SETTLE_TIME)
          {
            sipCount = MIN_HOLD_TIME;
            handlePress(SIP_BUTTON);
            sipState = SIP_PUFF_STATE_PRESSED;
          }
        } else if (sipCount) sipCount--;
        break;

      case SIP_PUFF_STATE_PRESSED:
        if (sipCount) sipCount--;
        if ((sensorData.pressure > slotSettings.ts) && (!sipCount)) {
          handleRelease(SIP_BUTTON);
          sipState = 0;
        }
    }
  }
  
  // check physical buttons 1-5 (only if not handled by any special sip/puff state)
  if(strongSipPuffState == STRONG_MODE_IDLE) {
    for (int i = 0; i < NUMBER_OF_PHYSICAL_BUTTONS; i++) { // update button press / release events
      handleButton(i, digitalRead(input_map[i]) == LOW ? 1 : 0);
    }
  }
}
