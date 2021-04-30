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

#define PIXELS_PIN 15       // Input Pin for WS2812 ("NeoPixels")


uint8_t neoPix_r = 0;
uint8_t neoPix_g = 0;
uint8_t neoPix_b = 0;
uint8_t LEDDimm_factor = 4; //reduces the brightnes of the LED: 1 = full, 2 = half brightness CAUTION: this can lead to changes in color!
uint8_t DimmState = 0;       //State 0: idle; State 1: dimm down; State 2: dimm up new color 


WS2812 pixels(1);
cRGB pixColor;


/* initialize "NeoPixel" WS2812 on the given PIXELS_PIN, 1 Pixel use! startup sequenc of this LED; use of delay!! --> just use before loop! */
void initNeoPixel(){
        
    pixels.setOutput(PIXELS_PIN);
    pixels.setColorOrderGRB();

    pixColor.r = 0; pixColor.g = 0; pixColor.b = 0; // RGB Value
    pixels.set_crgb_at(0, pixColor);    // set defined color
    pixels.sync(); // Sends the value to the LED

    uint32_t NeoPixelHEX = 0x000000;   // rrggbb
/*
    Serial.println((int)((NeoPixelHEX >> 16) & 0xFF));
    Serial.println((int)((NeoPixelHEX >> 8) & 0xFF));
    Serial.println((int)(NeoPixelHEX & 0xFF));
*/

    pixColor.r = (int)((NeoPixelHEX >> 16) & 0xFF);
    pixColor.g = (int)((NeoPixelHEX >> 8) & 0xFF);
    pixColor.b = (int)(NeoPixelHEX & 0xFF);
    
    pixels.set_crgb_at(0, pixColor);

    pixels.sync();



    //Pixel startup sequenc:
    for (uint8_t i = 0; i < 255; i++) {
      pixColor.r = i;
      pixels.set_crgb_at(0, pixColor);
      pixels.sync();
      delay(4);
    }

    
    for (uint8_t i = 255; i > (255/LEDDimm_factor); i--) {
      pixColor.r = i;
      pixels.set_crgb_at(0, pixColor);
      pixels.sync();
      delay(2);
    }


}


/*  dimm the NeoPixel up or down according to DimmState (set to 1 in case of slotchange); dimm down the current color and subsequently dimm up the new color; each call = one dimmStep */
void UpdateNeoPixel(){
  
  
  if (DimmState == 1){    // dimm color down

      if(pixColor.r > 1)   pixColor.r -= 2;
      if(pixColor.g > 1)   pixColor.g -= 2;
      if(pixColor.b > 1)   pixColor.b -= 2;

      pixels.set_crgb_at(0, pixColor); pixels.sync(); // set & apply color
      

      if((pixColor.r + pixColor.g + pixColor.b) <= 10)    // if overall color < 10 -> end dimm down and continue with dimm up of new color in next call
        DimmState = 2;
    }
    else if (DimmState == 2){   // dimm color up

      if(pixColor.r < (neoPix_r/LEDDimm_factor))   pixColor.r++;
      if(pixColor.g < (neoPix_g/LEDDimm_factor))   pixColor.g++;
      if(pixColor.b < (neoPix_b/LEDDimm_factor))   pixColor.b++;

      pixels.set_crgb_at(0, pixColor); pixels.sync(); // set & apply color
    }
}


/* update the slotcolor, called in case of slotchange; set DimmState to start dimming */
void updateNeoPixelColor(uint8_t newSlotNumber){
    DimmState = 1;
    pixels.set_crgb_at(0, pixColor);
    pixels.sync();

    switch (newSlotNumber) {
        case 1:
        neoPix_r = 255; neoPix_g = 0; neoPix_b = 0;
        break;
        case 2:
        neoPix_r = 0; neoPix_g = 255; neoPix_b = 0;
        break;
        case 3:
        neoPix_r = 0; neoPix_g = 0; neoPix_b = 255;
        break;
        case 4:
        neoPix_r = 255; neoPix_g = 255; neoPix_b = 0;
        break;
        default:
        neoPix_r = 255; neoPix_g = 0; neoPix_b = 255;
        break;
    }
}