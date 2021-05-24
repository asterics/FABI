
/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: eeprom.cpp - load/store settings to/from eeprom

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "fabi.h"
#include "eepromStorage.h"

#define SLOT_VALID 0x26  // "magic byte" for valid EEPROM content

int nextSlotAddress=0;
int EmptySlotAddress = 0;
int EmptyKeystringAddress=EEPROM_SIZE-1;
uint16_t freeEEPROMbytes = 0;


/**
   @name getfreeEEPROM
   @param none
   @return none

   returns free EEPROM memory in percent
*/
uint16_t getfreeEEPROM() {
  return ((uint32_t) freeEEPROMbytes * 100 / EEPROM_SIZE);
}



/**
   @name saveToEEPROM
   @param char * slotname
   @return none

   saves the configuration slot (identified by slotname) to the EEPROM
   cancels if not enough EEPROm memory free
*/
void saveToEEPROM(char * slotname)
{
   char act_slotname[MAX_SLOTNAME_LEN];
   int address = 0;
   int tmpStartAddress=0;
   uint8_t found=0;
   uint8_t * p;

   address=EmptySlotAddress;
   
   #ifdef DEBUG_OUTPUT   
     Serial.print(F("Writing slot ")); if (slotname) Serial.print(slotname);
     Serial.print(F(" starting from EEPROM address ")); Serial.println(address);
     Serial.print(F("We need ")); Serial.print(sizeof(settingsType)+NUMBER_OF_BUTTONS*sizeof(buttonType)+1);
     Serial.print(F(" bytes for the config and ")); Serial.print(keystringMemUsage(0));
     Serial.println(F(" bytes for keystrings."));

     Serial.print(EmptyKeystringAddress-address);
     Serial.println(F(" bytes are free.")); 
   #endif
  
   if ( sizeof(settingsType)+NUMBER_OF_BUTTONS*sizeof(buttonType)+1 + keystringMemUsage(0)
        >= (EmptyKeystringAddress-address)) {
     Serial.print(F(" EEPROM too small, aborting! ")); 
     return;
   }
 
   // start with new slot 
   EEPROM.update(address,SLOT_VALID);     
   address++;

   // update slotname
   if (!slotname) settings.slotname[0]=0;
   else strcpy(settings.slotname,slotname);
      
   // write general settings 
   p = (uint8_t*) &settings;
   for (int t=0;t<sizeof(settingsType);t++)
      EEPROM.update(address++,*p++);

   // write all buttons
   p = (uint8_t*) buttons;
   for (int i=0;i<NUMBER_OF_BUTTONS*sizeof(buttonType);i++)
        EEPROM.update(address++,*p++);

   if (EmptySlotAddress<=address) {
     EEPROM.update(address,0);
     EmptySlotAddress=address;
   }

   // write keystrings
   // Note that the keystrings are stored "top down", starting at the highest EEPROM adress
   for (int i=0;i<keystringMemUsage(0);i++)
        EEPROM.update(EmptyKeystringAddress--,keystringBuffer[i]);

   // update free bytes
   freeEEPROMbytes=EmptyKeystringAddress-address;
}

/**
   @name readFromEEPROM
   @param char * slotname
   @return none

   loads the configuration slot (identified by slotname) from the EEPROM

   in case reportSlotParameters==REPORT_ONE_SLOT, the slot configuration is printed (if slotname is found)
   in case reportSlotParameters==REPORT_ALL_SLOTS, all slot configurations are printed
   
*/
void readFromEEPROM(char * slotname)
{
   char act_slotname[MAX_SLOTNAME_LEN];
   int address=0;
   int tmpSlotAddress=0;
   int tmpStartAddress=0;
   int tmpKeystringAddress=EEPROM_SIZE-1;
   uint8_t done;
   uint8_t numSlots=0;
   uint8_t stringCount=0;
   uint8_t* p;

   // iterate over all valid slots in EEPROM
   while (EEPROM.read(address)==SLOT_VALID)  // indicates valid eeprom content !
   {
      uint8_t found=0;
      
      if ((!slotname) && (address==nextSlotAddress)) found=1;
      address++;

      tmpStartAddress=address;
      uint8_t i=0;
      while ((act_slotname[i++]=EEPROM.read(address++)) != 0) ; 
      
      #ifdef DEBUG_OUTPUT   
         Serial.print(F("processing slot ")); Serial.println(act_slotname);
      #endif

      // if slotname matches the slot stored in EEPROM
      if (slotname)  {
        if (!strcmp(act_slotname, slotname)) found=1;  
      }
      
      address=tmpStartAddress;
      if ((found) || (reportSlotParameters==REPORT_ALL_SLOTS))  {       
        #ifdef DEBUG_OUTPUT  
           Serial.print(F("LOADING slot ")); Serial.println(act_slotname);
        #endif

        // load settings structure
        p = (uint8_t*) &settings;
        for (int t=0;t<sizeof(settingsType);t++)
            *p++=EEPROM.read(address++);
        
        // load button array
        p = (uint8_t*) buttons;
        for (int i=0;i<NUMBER_OF_BUTTONS*sizeof(buttonType);i++) 
           *p++=EEPROM.read(address++);
           
        // load keystrings
        // Note that the keystrings are stored "top down", starting at the highest EEPROM adress
        p = (uint8_t*) keystringBuffer;
        stringCount=0;
        do {
           uint8_t c=EEPROM.read(tmpKeystringAddress--);
           *p++=c;
           if (!c) stringCount++;
        } while (stringCount < NUMBER_OF_BUTTONS);

        if (reportSlotParameters!=REPORT_NONE)  
          printCurrentSlot();

        actSlot=numSlots+1; 
        tmpSlotAddress=address;
      } 
      else {
        address += (sizeof(settingsType) + NUMBER_OF_BUTTONS * sizeof(buttonType));   // skip this slot  
        stringCount=0;
        do {
           if (!EEPROM.read(tmpKeystringAddress--)) stringCount++;
        } while (stringCount < NUMBER_OF_BUTTONS);
      }
      numSlots++;
   }

   // update top address (for storing a new slot) and free bytes
   EmptySlotAddress=address;
   if (tmpSlotAddress) nextSlotAddress=tmpSlotAddress;
   if (nextSlotAddress==EmptySlotAddress) nextSlotAddress=0;

   freeEEPROMbytes=tmpKeystringAddress-address;
   
   #ifdef DEBUG_OUTPUT   
       Serial.print(numSlots); Serial.print(F(" slots were found in EEPROM, occupying "));
       Serial.print(address+ EEPROM_SIZE-1-tmpKeystringAddress); Serial.print(F(" bytes ("));
       Serial.print(F("config: ")); Serial.print(address); Serial.print(F(", keystrings: "));
       Serial.print(EEPROM_SIZE-1-tmpKeystringAddress); Serial.println(F(")"));
       Serial.print(freeEEPROMbytes); Serial.println(F(" bytes are free.")); 
   #endif
   
   if (reportSlotParameters) 
     Serial.println(F("END"));   // important: end marker for slot parameter list (command "load all" - AT LA)

}

/**
   @name deleteSlots
   @param none
   @return none

   deletes all slots and keystrings from EEPROM
   setting the first valid indicators to 0
*/
void deleteSlots()
{
   EmptySlotAddress=0;
   EmptyKeystringAddress=EEPROM_SIZE-1;
   freeEEPROMbytes=EEPROM_SIZE-1;
   nextSlotAddress=0;
   EEPROM.update(0,0);
}

/**
   @name listSlots
   @param none
   @return none

   lists all stored slotnames  from EEPROM
*/
void listSlots()
{
   int address=0,tmpStartAddress=0;
   uint8_t numSlots=0;
   uint8_t b;
   
   while (EEPROM.read(address)==SLOT_VALID)  // indicates valid eeprom content !
   {
     numSlots++;
     address++;
     tmpStartAddress=address;
     Serial.print(F("Slot")); Serial.print(numSlots); Serial.print(":"); 
     while ((b=EEPROM.read(address++)) != 0)   // print slot name
         Serial.write(b);
     Serial.println();
     
     address=tmpStartAddress+sizeof(settingsType)+NUMBER_OF_BUTTONS*sizeof(buttonType);         
   }
}
