/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: NeoPixel.h - WS2812 ("NeoPixel") control
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/


#ifndef _NEOPIXEL_H_
#define _NEOPIXEL_H_

#include "fabi.h"
#include <Adafruit_NeoPixel.h>     //  light_ws2812 library


void initNeoPixel();
void UpdateNeoPixel();
void updateNeoPixelColor(uint8_t newSlotNumber);

#endif
