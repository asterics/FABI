
/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: Buzzer.cpp - tone generation / buzzer control
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include <Arduino.h>
#include "buzzer.h"

uint8_t beepCounter = 0;    // number of beeps
uint16_t beepTime = BEEP_duration * 2;     // duration of beep and following same times without sound

/**
   @name initBuzzer
   @param none
   @return none

   initialises the buzzer pin and creates initial tone
*/
void initBuzzer() {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, HIGH);
    _delay_ms(100);
    digitalWrite(BUZZER_PIN, LOW);
}

/**
   @name setBeepCount
   @param uint16_t count
   @return none

   sets the beep counter in order to create a number of tones
*/
void setBeepCount(uint16_t count) {
  beepCounter = count;
}


/**
   @name UpdateBuzzer
   @param none
   @return none

   updates timings and creates "beepCounter" tones with the buzzer
   individual tone lenght / pause time is defined via BEEP_duration (see buzzer.h)
   note that this function must be colled periodically from the main loop
*/
void UpdateBuzzer() { 
  if (beepCounter > 0) {                // tone
  	if (beepTime > BEEP_duration) {
  	  digitalWrite(BUZZER_PIN, HIGH);
  	  beepTime--;
  	}
  	else if (beepTime > 0) {            // no tone
  	  digitalWrite(BUZZER_PIN, LOW);
  	  beepTime--;
  	}
  	else {
  	  beepTime = BEEP_duration * 2;
  	  beepCounter--;
  	}
  }
}
