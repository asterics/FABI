
/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: buttons.h - button mapping and press/release handling
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/


#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#define NUMBER_OF_PHYSICAL_BUTTONS_NOPCB 9  // number of connectable switches for no-PCB version
#define NUMBER_OF_PHYSICAL_BUTTONS_PCB   8  // number of connectable switches for PCB version
#define NUMBER_OF_LEDS     3                // number of connectable leds (no-PCB verion)

#define SIP_BUTTON    9
#define PUFF_BUTTON  10
#define PRESSURE_SENSOR_PIN A0

#define BUTTON_PRESSED  1
#define BUTTON_RELEASED 0
#define BUTTONSTATE_NOT_PRESSED   0
#define BUTTONSTATE_SHORT_PRESSED 1
#define BUTTONSTATE_LONG_PRESSED  2
#define BUTTONSTATE_IDLE          3

void initButtons();
void initDebouncers();
void updateButtons();
void handlePress (int buttonIndex);      // a button was pressed
void handleRelease (int buttonIndex);    // a button was released
void handleButton(int i, int l, uint8_t b);  // button debouncing
uint8_t allButtonsReleased();

#endif
