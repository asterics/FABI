#include "NeoPixel.h"

#define PIXELS_PIN 15       // Input Pin for WS2812 ("NeoPixels")


uint8_t neoPix_r = 0;
uint8_t neoPix_g = 0;
uint8_t neoPix_b = 0;
uint8_t LEDDimm_factor = 3; //reduces the brightnes of the LED: 1 = full, 2 = half brightness CAUTION: this can lead to changes in color!
uint8_t DimmState = 0;       //State 0: idle; State 1: dimm down; State 2: dimm up new color 


WS2812 pixels(1);
cRGB pixColor;


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



    //Pixel & buzzer startup sequenc:
    for (uint8_t i = 0; i < 60; i++) {
      pixColor.r = i;
      pixels.set_crgb_at(0, pixColor);
      pixels.sync();
      delay(10);
    }

    
    for (uint8_t i = 60; i > (50); i--) {
      pixColor.r = i;
      pixels.set_crgb_at(0, pixColor);
      pixels.sync();
      delay(10);
    }


}

void UpdateNeoPixel(){

    
      //NeoPixel:
      if (DimmState == 1){

        if(pixColor.r > 1)   pixColor.r -= 2;
        if(pixColor.g > 1)   pixColor.g -= 2;
        if(pixColor.b > 1)   pixColor.b -= 2;

        pixels.set_crgb_at(0, pixColor);
        pixels.sync();

        if((pixColor.r + pixColor.g + pixColor.b) <= 10)
          DimmState = 2;
      }
      else if (DimmState == 2){

        if(pixColor.r < (neoPix_r/LEDDimm_factor))   pixColor.r++;
        if(pixColor.g < (neoPix_g/LEDDimm_factor))   pixColor.g++;
        if(pixColor.b < (neoPix_b/LEDDimm_factor))   pixColor.b++;

        pixels.set_crgb_at(0, pixColor);
        pixels.sync();
      }


}




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