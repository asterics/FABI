/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: NeoPixel.cpp - WS2812 ("NeoPixel") control
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/


#include "NeoPixel.h"

#define PIXELS_PIN 15       // output Pin for WS2812 ("NeoPixels") data signal

uint8_t neoPix_r = 0;
uint8_t neoPix_g = 0;
uint8_t neoPix_b = 0;

uint16_t neoPix_r_16 = 0;
uint16_t neoPix_g_16 = 0;
uint16_t neoPix_b_16 = 0;

uint16_t dimmStep_r = 0;
uint16_t dimmStep_g = 0;
uint16_t dimmStep_b = 0;


uint8_t LEDDimm_factor = 1; //reduces the brightnes of the LED: 1 = full, 2 = half brightness CAUTION: this can lead to changes in color!
uint8_t DimmState = 0;       

Adafruit_NeoPixel pixels(1, PIXELS_PIN, NEO_GRB + NEO_KHZ800);



/**
   @name initNeoPixel
   @param none
   @return none

   initialize "NeoPixel" WS2812 on the given PIXELS_PIN, 1 Pixel in use! 
   creates a startup sequence with this Neopixel LED.
   Note: uses delay!! --> just use before loop! 

*/
void initNeoPixel(){
    pixels.clear();
    pixels.show();

    //Pixel startup sequenc:
    for (uint8_t i = 0; i < 255; i++) {
      pixels.setPixelColor(0, 0, 0, i);
      pixels.show();
      delay(4);
    }   
}

/**
   @name UpdateNeoPixel
   @param none
   @return none

   dimm the NeoPixel up or down according to DimmState (set to 1 in case of slotchange)
   dimm down the current color and subsequently dimm up the new color
   each call = one dimmStep

*/
void UpdateNeoPixel(){
  
  
  if (DimmState < 100){   //  DIMM DOWN the current color, until the DimmState counter var reaches 100
    if (DimmState == 0){    // calculate the dimmStep(size) for the 100 steps
      dimmStep_g = neoPix_g_16 / 100;
      dimmStep_r = neoPix_r_16 / 100;   
      dimmStep_b = neoPix_b_16 / 100;
    }
    //reduce the _16 variable by the dimmStep(size)
    neoPix_r_16 -= dimmStep_r;    
    neoPix_g_16 -= dimmStep_g;
    neoPix_b_16 -= dimmStep_b;

    //apply the color var to the pixColor (with division of 10, since the counter variables are 10* higher than the color to ensure smooth dimming with reduced brightness)
    pixels.setPixelColor(0, neoPix_r_16/10, neoPix_g_16/10, neoPix_b_16/10);
    pixels.show();
    
    DimmState++; 
  }
  else if (DimmState < 200){    //  DIMM UP the new color, in 100 steps
    if (DimmState == 100){      // in first case calculate the dimmStep(size) 
      dimmStep_r = neoPix_r / LEDDimm_factor / 10;
      dimmStep_g = neoPix_g / LEDDimm_factor / 10;
      dimmStep_b = neoPix_b / LEDDimm_factor / 10;
      
      neoPix_r_16 = neoPix_g_16 = neoPix_b_16 = 0;    // reset color to 0
    }

    //add the dimmStep(size) to the counter var
    neoPix_r_16 += dimmStep_r;
    neoPix_g_16 += dimmStep_g;
    neoPix_b_16 += dimmStep_b;

    //apply the color var to the pixColor (with division of 10)
    pixels.setPixelColor(0, neoPix_r_16/10, neoPix_g_16/10, neoPix_b_16/10);
    pixels.show();

    DimmState++; 
  }
}

/**
   @name updateNeoPixelColor
   @param uint8_t newSlotNumber
   @return none

   update the slotcolor, called in case of slotchange
   sets DimmState to "start dimming"
*/
void updateNeoPixelColor(uint8_t newSlotNumber){
    DimmState = 0;

    //set the current color to the _16 to dimm down the current color:
    #warning "this was the old thing, not sure if enough to simply set to same value"
    #if 0
    neoPix_r_16 = pixColor.r * 10;     // color values are time tens higher due to smaller possible step size (prevent using floats, cause of storage size)
    neoPix_g_16 = pixColor.g * 10;
    neoPix_b_16 = pixColor.b * 10; 
    #endif
    
    //set new color from slot settings
    neoPix_r = (int)((settings.sc >> 16) & 0xFF);
    neoPix_g = (int)((settings.sc >> 8) & 0xFF);
    neoPix_b = (int)(settings.sc & 0xFF);
    
    neoPix_r_16 = neoPix_r * 10;
    neoPix_g_16 = neoPix_g * 10;
    neoPix_b_16 = neoPix_b * 10;
}
