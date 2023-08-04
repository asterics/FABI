/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: toneFABI.cpp - tone generation / buzzer control
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include <Arduino.h>
#include "toneFABI.h"

void toneFABI(uint16_t frequency, uint16_t time)
{
  tone(BUZZER_PIN,frequency,time);
}

/**
   @name initBuzzer
   @param none
   @return none

   initialises the buzzer pin and creates initial tone
*/
void initBuzzer() {
  toneFABI(1,100);
  delay(300);
  toneFABI(1,200);  delay(200);
  toneFABI(3,200);  delay(200);
  toneFABI(5,100);

}
