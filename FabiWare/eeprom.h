
/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: eeprom.h - eeprom memory management
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/


#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <EEPROM.h>


#define EEPROM_SIZE        1023     // maximum size of EEPROM storage for Arduino Pro Micro

#define REPORT_NONE  0  
#define REPORT_ONE_SLOT  1
#define REPORT_ALL_SLOTS 2

uint16_t getfreeEEPROM();
void saveToEEPROM(char * slotname);
void readFromEEPROM(char * slotname);
void listSlots();
void deleteSlots();
void printCurrentSlot();

#endif
