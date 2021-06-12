
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

#define MAGIC_BYTE 42  // "magic byte" for valid EEPROM content
#define SLOTSIZE (sizeof(settingsType)+NUMBER_OF_BUTTONS*sizeof(buttonType))



int nextSlotAddress=0;
int EmptySlotAddress = 0;
int EmptyKeystringAddress=EEPROM_TOP_ADDRESS-1;
uint16_t freeEEPROMbytes = EEPROM_TOP_ADDRESS;


/**
   @name getfreeEEPROM
   @param none
   @return none

   returns free EEPROM memory in percent
*/
uint16_t getfreeEEPROM() {
  return ((uint32_t) freeEEPROMbytes * 100 / EEPROM_TOP_ADDRESS);
}


/**
   move multiple bytes in EEPROM 
   targetAdr: start address for writing bytes 
   sourceAdr: start address for reading bytes 
   count: number of bytes to move
   moving overlapping regions is possible
 * */
void moveEEPROM(uint16_t targetAdr, uint16_t sourceAdr, int count) {
  if (sourceAdr > targetAdr) {  // forward copy
    while (count-- > 0) {      
      uint8_t c=EEPROM.read(sourceAdr++);
      EEPROM.update(targetAdr++,c);
    }
  }
  else {  // backward copy
    sourceAdr+=count-1;
    targetAdr+=count-1;
    while (count--) {
      EEPROM.update(targetAdr--,EEPROM.read(sourceAdr--));
    }
  }
}



/**
   @name getSlotInfos
   @param char * slotname
   @param uint16_t * s_address
   @param uint16_t * k_address
   @param uint16_t * k_len
   @return uint8_t

   get information about a slot from the EEPROM

   returns 1 if slotname was found, and 0 if not.
   if the slow was found, the slot address (s_address), 
   the keystring address (k_address) and the length of the keysting (k_len) are returned.
   Note that keystrings are stored "top-down" in EEPROM (k_address is the top address)
*/
uint8_t getSlotInfos(char * slotname, uint16_t * s_address, uint16_t * k_address, uint16_t * k_len)
{
   uint16_t address=0;
   uint16_t keystringAddress=EEPROM_TOP_ADDRESS-1;

   while (EEPROM.read(address))
   {
      uint8_t c,i=0,match=0;
      while (c=EEPROM.read(address+i)) {
        if (slotname[i] == c) i++;
        else break;
      }
      if (!slotname[i] && !c) {
          match=1; 
          *s_address = address;
          *k_address = keystringAddress;
          *k_len=0;
      }
      
      uint8_t stringCount=0; 
      while (stringCount < NUMBER_OF_BUTTONS) {
         if (!EEPROM.read(keystringAddress--)) stringCount++;
         if (match) *k_len = *k_len + 1;  // increase keystring length info 
      }
      if (match) return(1);

      // skip to next slot address
      address+=SLOTSIZE;
   }
   return(0);
}


/**
   @name saveToEEPROM
   @param char * slotname
   @return none

   saves the configuration slot (identified by slotname) to the EEPROM
   cancels if not enough EEPROm memory free
*/
int saveToEEPROM(char * slotname)
{
   char act_slotname[MAX_SLOTNAME_LEN];
   int delta=0;
   uint16_t address=0;
   uint16_t keystring_address;
   uint16_t old_keystring_len, keystring_len;
   uint8_t append=0;
   uint8_t * p;


   keystring_len=keystringMemUsage(0);  // lenght of current keystrings

   if (getSlotInfos(slotname, &address, &keystring_address, &old_keystring_len)) {
     #ifdef DEBUG_OUTPUT   
       Serial.println("Slot already exists !!");
       Serial.print(F(" starting from EEPROM address ")); Serial.println(address);
       Serial.print(F(" keystring address ")); Serial.println(keystring_address);
       Serial.print(F(" old keystring length ")); Serial.print(old_keystring_len);
       Serial.print(F(", act len: ")); Serial.println(keystring_len);
     #endif

     delta = old_keystring_len - keystring_len;
     if (delta) {  

       //  move slot data?
       #ifdef DEBUG_OUTPUT   
         Serial.print(F(" move from address ")); Serial.print(EmptyKeystringAddress);
         Serial.print(F(" to address ")); Serial.println(EmptyKeystringAddress+delta);
         Serial.print(F(" bytes: ")); Serial.println(keystring_address - old_keystring_len - EmptyKeystringAddress);
       #endif
       
       if (-delta > (int)freeEEPROMbytes) 
         return 0;
         
       moveEEPROM (EmptyKeystringAddress+delta+1, EmptyKeystringAddress+1, keystring_address - old_keystring_len - EmptyKeystringAddress);
       EmptyKeystringAddress+=delta;
     }
   }
   else {
    // start with new slot
    if ( SLOTSIZE + keystring_len > freeEEPROMbytes) 
       return 0;

    address=EmptySlotAddress;
    keystring_address=EmptyKeystringAddress;
    append=1;
  }

   
   #ifdef DEBUG_OUTPUT   
     Serial.print(F("Writing slot ")); if (slotname) Serial.print(slotname);
     Serial.print(F(" starting from EEPROM address ")); Serial.println(address);
     Serial.print(F("We need ")); Serial.print(SLOTSIZE);
     Serial.print(F(" bytes for the config and ")); Serial.print(keystring_len);
     Serial.println(F(" bytes for keystrings."));

     Serial.print(freeEEPROMbytes);
     Serial.println(F(" bytes are free.")); 
   #endif

 
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
   
   // write keystrings
   // Note that the keystrings are stored "top down", starting at the highest EEPROM adress
   for (int i=0;i<keystring_len;i++)
        EEPROM.update(keystring_address--,keystringBuffer[i]);

   // update free bytes
   if (append) {
     EEPROM.update(address,0);
     EmptySlotAddress=address;
     EmptyKeystringAddress-=keystring_len;
   }
   
   freeEEPROMbytes=EmptyKeystringAddress-EmptySlotAddress;
   return(1);
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
   int tmpKeystringAddress=EEPROM_TOP_ADDRESS-1;
   uint8_t done;
   uint8_t numSlots=0;
   uint8_t stringCount=0;
   uint8_t* p;

   // iterate over all valid slots in EEPROM
   while (EEPROM.read(address)) 
   {
      uint8_t found=0;
      
      if ((!slotname) && (address==nextSlotAddress)) found=1;

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
        address += SLOTSIZE;   // skip this slot  
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
   EmptyKeystringAddress=tmpKeystringAddress;

   freeEEPROMbytes=EmptyKeystringAddress-EmptySlotAddress;
   
   #ifdef DEBUG_OUTPUT   
       Serial.print(numSlots); Serial.print(F(" slots were found in EEPROM, occupying "));
       Serial.print(address+ EEPROM_TOP_ADDRESS-1-tmpKeystringAddress); Serial.print(F(" bytes ("));
       Serial.print(F("config: ")); Serial.print(address); Serial.print(F(", keystrings: "));
       Serial.print(EEPROM_TOP_ADDRESS-1-tmpKeystringAddress); Serial.println(F(")"));
       Serial.print(freeEEPROMbytes); Serial.println(F(" bytes are free.")); 
   #endif
   
   if (reportSlotParameters) 
     Serial.println(F("END"));   // important: end marker for slot parameter list (command "load all" - AT LA)

}

/**
   @name deleteSlots
   @param slotname
   @return none

   deletes one slot and it's keystinrg from the EEPROM
   if slotname is empty: deletes all slots and keystrings from EEPROM
   setting the first valid indicators to 0
*/
void deleteSlots(char * slotname)
{
   if (!strlen(slotname)) {
    Serial.println("deleting all slots!");
    EmptySlotAddress=0;
    EmptyKeystringAddress=EEPROM_TOP_ADDRESS-1;
    freeEEPROMbytes=EEPROM_TOP_ADDRESS-1;
    nextSlotAddress=0;
    EEPROM.update(0,0);
    return;
   }
   
   uint16_t address=0, old_keystring_len, keystring_address, keystring_len;

   if (getSlotInfos(slotname, &address, &keystring_address, &old_keystring_len)) {
     #ifdef DEBUG_OUTPUT   
       Serial.print("deleting one slot @address "); Serial.println(address);    
       Serial.print(" keystring length = "); Serial.println(old_keystring_len);    
     #endif

     moveEEPROM (address, address+SLOTSIZE, EmptySlotAddress-address);
     moveEEPROM (EmptyKeystringAddress+old_keystring_len+1, EmptyKeystringAddress+1 , keystring_address - old_keystring_len - EmptyKeystringAddress);

     EmptySlotAddress-=SLOTSIZE;
     EEPROM.update(EmptySlotAddress,0);
     EmptyKeystringAddress+=old_keystring_len;
     #ifdef DEBUG_OUTPUT   
       Serial.print("EmptySlotAddress "); Serial.println(EmptySlotAddress);    
       Serial.print("EmptyKeystringAddress "); Serial.println(EmptyKeystringAddress);    
     #endif
   }  
}

void bootstrapEEPROM()
{
  if (EEPROM.read(EEPROM_TOP_ADDRESS) != MAGIC_BYTE)
  {
    #ifdef DEBUG_OUTPUT   
      Serial.println("initializing EEPROM");
    #endif
    EEPROM.update(EEPROM_TOP_ADDRESS, MAGIC_BYTE);
    EEPROM.update(0,0);
    saveToEEPROM("default");
  }
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
   
   while (EEPROM.read(address))  
   {
     numSlots++;
     tmpStartAddress=address;
     Serial.print(F("Slot")); Serial.print(numSlots); Serial.print(":"); 
     while ((b=EEPROM.read(address++)) != 0)   // print slot name
         Serial.write(b);
     Serial.println();
     
     address=tmpStartAddress+SLOTSIZE;         
   }
}
