
#ifndef _NEOPIXEL_H_
#define _NEOPIXEL_H_

#include "fabi.h"
#include <WS2812.h>     //  light_ws2812 library


void initNeoPixel();
void UpdateNeoPixel();
void updateNeoPixelColor(uint8_t newSlotNumber);

#endif