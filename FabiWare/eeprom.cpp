#include "fabi.h"
#include <EEPROM.h>

#define SLOT_VALID 21

int nextSlotAddress=0;


void saveToEEPROM(char * slotname)
{
   char act_slotname[MAX_SLOTNAME_LEN];
   int address = 0;
   int tmpStartAddress=0;
   uint8_t found=0;
   uint8_t * p;

   if (!slotname) address=EmptySlotAddress;
   else
   {
     if (strlen(slotname) >= MAX_SLOTNAME_LEN) slotname[MAX_SLOTNAME_LEN-1]=0;
         
     while ((EEPROM.read(address)==SLOT_VALID) && (!found) && ((address+1) < EmptySlotAddress))  // indicates valid eeprom content !
     {
       tmpStartAddress=address;
       address++;

       uint8_t i=0;
       while ((act_slotname[i++]=EEPROM.read(address++)) != 0) ;      

       // Serial.print("peeking slot "); Serial.print(act_slotname); Serial.print(" @ "); Serial.println(tmpStartAddress);
       address=tmpStartAddress;
       if (!strcmp(act_slotname, slotname)) found=1; 
       else  address+= (sizeof(settingsType)+NUMBER_OF_BUTTONS*sizeof(buttonType)+1);      
     }
   }
   
   if (DebugOutput==1) {  
     Serial.print(F("Writing slot ")); if (slotname) Serial.print(slotname);
     Serial.print(F(" starting from EEPROM address ")); Serial.println(address);
   }
   
   // start with new slot 
   EEPROM.write(address++,SLOT_VALID);  

   // update slotname
   if (!slotname) settings.slotname[0]=0;
   else strcpy(settings.slotname,slotname);
      
   // write general settings 
   p = (uint8_t*) &settings;
   for (int t=0;t<sizeof(settingsType);t++)
      EEPROM.write(address++,*p++);

   // write all buttons
   p = (uint8_t*) buttons;
   for (int i=0;i<NUMBER_OF_BUTTONS*sizeof(buttonType);i++)
        EEPROM.write(address++,*p++);

   if (EmptySlotAddress<=address) {
     EEPROM.write(address,0);
     EmptySlotAddress=address;
   }
}


void readFromEEPROM(char * slotname)
{
   char act_slotname[MAX_SLOTNAME_LEN];
   int address=0;
   int tmpSlotAddress=0;
   int tmpStartAddress=0;
   uint8_t done;
   uint8_t numSlots=0;
   uint8_t* p;
   
   while (EEPROM.read(address)==SLOT_VALID)  // indicates valid eeprom content !
   {
      uint8_t found=0;
      
      if ((!slotname) && (address==nextSlotAddress)) found=1;
      address++;

      tmpStartAddress=address;
      uint8_t i=0;
      while ((act_slotname[i++]=EEPROM.read(address++)) != 0) ; 
      
      if (DebugOutput==1) {  
         Serial.print(F("found slotname ")); Serial.println(act_slotname);
      }
     
      if (slotname)  {
        if (!strcmp(act_slotname, slotname)) found=1;  
      }
      
      address=tmpStartAddress;
      if ((found) || (reportSlotParameters==REPORT_ALL_SLOTS))  {       
        p = (uint8_t*) &settings;
        for (int t=0;t<sizeof(settingsType);t++)
            *p++=EEPROM.read(address++);
        
        p = (uint8_t*) buttons;
        for (int i=0;i<NUMBER_OF_BUTTONS*sizeof(buttonType);i++) 
           *p++=EEPROM.read(address++);

        if (reportSlotParameters!=REPORT_NONE)  
          printCurrentSlot();

        actSlot=numSlots+1; 
        tmpSlotAddress=address;
      } 
      else address += (sizeof(settingsType) + NUMBER_OF_BUTTONS * sizeof(buttonType));   // skip this slot  
      numSlots++;
   }
   
   EmptySlotAddress=address;
   if (tmpSlotAddress) nextSlotAddress=tmpSlotAddress;
   if (nextSlotAddress==EmptySlotAddress) nextSlotAddress=0;
   
   if (DebugOutput==1) {  
       Serial.print(numSlots); Serial.print(F(" slots were found in EEPROM, occupying "));
       Serial.print(address); Serial.println(F(" bytes."));
   }
   
   if (reportSlotParameters) 
     Serial.println(F("END"));   // important: end marker for slot parameter list (command "load all" - AT LA)

}

void deleteSlots()
{
   EmptySlotAddress=0;
   nextSlotAddress=0;
   EEPROM.write(0,0);
}

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

