
/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: Buzzer.h - tone generation / buzzer control
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/


#ifndef _BUZZER_H_
#define _BUZZER_H_

#define BEEP_duration 25    // actual duration of beep = BEEP_duration * loop duration; e.g. 100 * 5ms = 500ms
#define BUZZER_PIN 4

void initBuzzer();
void setBeepCount(uint16_t count);
void UpdateBuzzer();

#endif
