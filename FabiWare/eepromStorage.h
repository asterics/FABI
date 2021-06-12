
/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: eeprom.h - eeprom memory management
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/


#ifndef _EEPROMSTORAGE_H_
#define _EEPROMSTORAGE_H_

#include <EEPROM.h>


#define EEPROM_TOP_ADDRESS        1022     // top address for EEPROM storage, one byte reserved for magic byte

#define REPORT_NONE  0  
#define REPORT_ONE_SLOT  1
#define REPORT_ALL_SLOTS 2

uint16_t getfreeEEPROM();
int saveToEEPROM(char * slotname);
void bootstrapEEPROM();
void readFromEEPROM(char * slotname);
uint8_t getSlotInfos(char * slotname, uint16_t * s_address, uint16_t * k_address, uint16_t * k_len);
void listSlots();
void deleteSlots(char * slotname);
void printCurrentSlot();

#endif
