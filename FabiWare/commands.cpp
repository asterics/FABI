
/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: commands.cpp - AT command processing
     (for a list of supported AT-commands see commands.h)
           
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "fabi.h"
#include "eeprom.h"
#include "keys.h"
#include "commands.h"
#include "display.h"
#include "buzzer.h"


// AT Command list - defines all valid commands and their paramter types
// Note that the order of this list must match with the command index enum (see commands.h)

const struct atCommandType atCommands[] PROGMEM = {
    {"ID"  , PARTYPE_NONE },  {"BM"  , PARTYPE_UINT }, {"CL"  , PARTYPE_NONE }, {"CR"  , PARTYPE_NONE },
    {"CM"  , PARTYPE_NONE },  {"CD"  , PARTYPE_NONE }, {"HL"  , PARTYPE_NONE }, {"HR"  , PARTYPE_NONE },
    {"HM"  , PARTYPE_NONE },  {"RL"  , PARTYPE_NONE }, {"RR"  , PARTYPE_NONE }, {"RM"  , PARTYPE_NONE },
    {"TL"  , PARTYPE_NONE },  {"TR"  , PARTYPE_NONE }, {"TM"  , PARTYPE_NONE },
    {"WU"  , PARTYPE_NONE },  {"WD"  , PARTYPE_NONE }, {"WS"  , PARTYPE_UINT }, {"MX"  , PARTYPE_INT  },
    {"MY"  , PARTYPE_INT  },  {"KW"  , PARTYPE_STRING},{"KP"  , PARTYPE_STRING},{"KH"  , PARTYPE_STRING},
    {"KT"  , PARTYPE_STRING}, {"KR"  , PARTYPE_STRING},{"RA"  , PARTYPE_NONE }, {"SA"  , PARTYPE_STRING},
    {"LO"  , PARTYPE_STRING}, {"LA"  , PARTYPE_NONE }, {"LI"  , PARTYPE_NONE }, {"NE"  , PARTYPE_NONE }, 
    {"DE"  , PARTYPE_NONE },  {"NC"  , PARTYPE_NONE }, {"SR"  , PARTYPE_NONE }, {"ER"  , PARTYPE_NONE },
    {"TS"  , PARTYPE_UINT },  {"TP"  , PARTYPE_UINT }, {"MA"  , PARTYPE_STRING},{"WA"  , PARTYPE_UINT  },
    {"TT"  , PARTYPE_UINT },  {"AP"  , PARTYPE_UINT }, {"AR"  , PARTYPE_UINT},  {"AI"  , PARTYPE_UINT  },
    {"FR"  , PARTYPE_NONE },  {"BT"  , PARTYPE_UINT }, {"BC"  , PARTYPE_STRING},{"DP" , PARTYPE_UINT  },
    {"AD"  , PARTYPE_UINT },  {"SC"  , PARTYPE_STRING }
};

/**
   @name makehex
   @param uint32_t val 
   @param char* str - the target string
   @return none

   translates an uint32 integer value into a hex value
   the hex value is stored into a zero-terminated string (str)
   (with leading zeros, starting with "0x", eg. "0x00ACAB")
*/
void makehex(uint32_t val, char* str) {
  str[0]='0';
  str[1]='x';
  for (int i=7;i>=2;i--) {
    int digit=val %16;
    if (digit>9) str[i] = 'a'+digit-10;
    else str[i]='0'+digit;
    val>>=4;
  }
  str[8]=0;
}

/**
   @name printCurrentSlot
   @param none
   @return none

   prints the current configuration slot settings to Serial
*/
void printCurrentSlot()
{
  char tmp[10];
  Serial.print(F("Slot:"));  Serial.println(settings.slotname);
  Serial.print(F("AT WS ")); Serial.println(settings.ws);
  Serial.print(F("AT SC ")); makehex(settings.sc,tmp);Serial.println(tmp);
  Serial.print(F("AT TS ")); Serial.println(settings.ts);
  Serial.print(F("AT TP ")); Serial.println(settings.tp);
  Serial.print(F("AT TT ")); Serial.println(settings.tt);
  Serial.print(F("AT AP ")); Serial.println(settings.ap);
  Serial.print(F("AT AR ")); Serial.println(settings.ar);
  Serial.print(F("AT AI ")); Serial.println(settings.ai);
  Serial.print(F("AT BT ")); Serial.println(settings.bt);
  Serial.print(F("AT DP ")); Serial.println(settings.dp);
  Serial.print(F("AT AD ")); Serial.println(settings.ad);
  for (int i=0;i<NUMBER_OF_BUTTONS;i++) {
     Serial.print(F("AT BM ")); 
     if (i<9) Serial.print('0');
     Serial.println(i+1); 
     Serial.print(F("AT ")); 
     int actCmd = buttons[i].mode;
     char cmdStr[4];
     strcpy_FM(cmdStr,(uint_farptr_t_FM) atCommands[actCmd].atCmd);
     Serial.print(cmdStr);
      switch (pgm_read_byte_near(&(atCommands[actCmd].partype))) 
      {
         case PARTYPE_UINT: 
         case PARTYPE_INT:  Serial.print(' ');Serial.print(buttons[i].value); break;
         case PARTYPE_STRING: Serial.print(' ');Serial.print(getKeystring(i)); break;
      }
      Serial.println("");
  }
}



/**
   @name performCommand
   @param uint8_t cmd - the command identifier (index)
   @param int16_t parNum - an optional numeric parameter (eg. mouse movement)
   @param char * parString - an optional string parameter parameter (e.g keycode string)
   @param int8_t periodicMouseMovement - true if periodic mouse movement shall be started
   @return none

   perform a command / button action
   this function is either called directly from parser.cpp (in case an AT command was received from the Serial interface)
   or if a button press is valid and the associated function action shall be performed
*/
void performCommand (uint8_t cmd, int16_t parNum, char * parString, int8_t periodicMouseMovement)
{
  static uint8_t actButton=0;

  // if last command was BM (set buttonmode): 
  // store current command for this button, and do not actually execute the command!
  if (actButton != 0)
  {
      #ifdef DEBUG_OUTPUT
        Serial.print(F("got new mode for button ")); Serial.print(actButton);Serial.print(':');
        Serial.print(cmd);Serial.print(',');Serial.print(parNum);Serial.print(',');Serial.println(parString);
      #endif
      
      // store action command and numerical parameter into button array
      buttons[actButton-1].mode=cmd;
      buttons[actButton-1].value=parNum;

      // store the string parameter into the keystringBuffer array
      if (parString==0) setKeystring(actButton-1,"");
      else setKeystring(actButton-1,parString);
      actButton=0;
      #ifdef DEBUG_OUTPUT
        Serial.print("Used RAM for Keystrings:");Serial.print(keystringMemUsage(0));
        Serial.print(" (free: ");Serial.print(KEYSTRING_BUFFER_LEN-keystringMemUsage(0));
        Serial.println(")");
      
        for (int i=0;i<NUMBER_OF_BUTTONS;i++) {
          Serial.print("Keystring ");Serial.print(i);Serial.print(":");Serial.println(getKeystring(i));
        }
      #endif
      return;  
  }

  // perform the desired command/action 
  // for a command description see commands.h
  
  switch(cmd) {
      case CMD_ID:
             Serial.println(F(VERSION_STRING)); 
          break;
      case CMD_BM:
             release_all();
             #ifdef DEBUG_OUTPUT
                Serial.print(F("set mode for button ")); 
                Serial.println(parNum);
             #endif
             if ((parNum>0) && (parNum<=NUMBER_OF_BUTTONS))
                 actButton=parNum;
             else  Serial.println('?');
          break;
      
      case CMD_CL:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("click left"));
             #endif
             leftMouseButton=1;  leftClickRunning=DEFAULT_CLICK_TIME;
             break;
      case CMD_CR:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("click right"));
             #endif
             rightMouseButton=1; rightClickRunning=DEFAULT_CLICK_TIME;
             break;
      case CMD_CD:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("click double"));
             #endif
             leftMouseButton=1;  doubleClickRunning=DEFAULT_CLICK_TIME*DOUBLECLICK_MULTIPLIER;
             break;
      case CMD_CM:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("click middle"));
             #endif
             middleMouseButton=1; middleClickRunning=DEFAULT_CLICK_TIME;
            break;
      case CMD_HL:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("hold left"));
            #endif
             leftMouseButton=1; 
             break;
      case CMD_HR:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("hold right"));
             #endif
             rightMouseButton=1; 
             break;
      case CMD_HM:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("hold middle"));
             #endif
             middleMouseButton=1; 
             break;
      case CMD_RL:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("release left"));
            #endif
             leftMouseButton=0;
             break; 
      case CMD_RR:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("release right"));
             #endif
             rightMouseButton=0;
             break; 
      case CMD_RM:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("release middle"));
             #endif
             middleMouseButton=0;
             break; 
      case CMD_SC:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("slot color"));
               Serial.println((uint32_t)strtol(parString, NULL, 0));
             #endif
             settings.sc = (uint32_t)strtol(parString, NULL, 0);
             break; 
      case CMD_TL:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("toggle left"));
            #endif
             leftMouseButton^=1; 
             break;
      case CMD_TR:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("toggle right"));
             #endif
             rightMouseButton^=1; 
             break;
      case CMD_TM:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("toggle middle"));
             #endif
             middleMouseButton^=1; 
             break;
      case CMD_WU:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("wheel up"));
             #endif
  				   if(settings.ws != 0) mouseScroll(-settings.ws); 
  				   else mouseScroll(-DEFAULT_WHEEL_STEPSIZE); 
          break;
      case CMD_WD:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("wheel down"));
            #endif
  				  if(settings.ws != 0) mouseScroll(settings.ws); 
  				  else mouseScroll(DEFAULT_WHEEL_STEPSIZE); 
          break;
      case CMD_WS:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("wheel step"));
             #endif
             settings.ws=parNum;
          break;
      case CMD_MX:
             #ifdef DEBUG_OUTPUT 
               Serial.print(F("mouse move x "));
               Serial.println(parNum);
             #endif
             if (periodicMouseMovement) moveX=parNum;
             else mouseMove(parNum, 0); 
          break;
      case CMD_MY:
             #ifdef DEBUG_OUTPUT   
               Serial.print(F("mouse move y ")); 
               Serial.println(parNum);
             #endif
             if (periodicMouseMovement) moveY=parNum;
             else mouseMove(0, parNum);

          break;
      case CMD_KW:
             #ifdef DEBUG_OUTPUT   
              Serial.print(F("keyboard write: "));
              Serial.println(parString); 
             #endif 
             sendToKeyboard(parString);
             break;
      case CMD_KP:
             #ifdef DEBUG_OUTPUT   
               Serial.print(F("key press: "));
               Serial.println(parString);
             #endif
             pressSingleKeys(parString);
             releaseSingleKeys(parString);             
             break;
      case CMD_KH:
             #ifdef DEBUG_OUTPUT   
               Serial.print(F("key hold: "));
               Serial.println(parString);
             #endif
             pressSingleKeys(parString);
             break;
      case CMD_KT:
             #ifdef DEBUG_OUTPUT   
               Serial.print(F("key toggle: "));
               Serial.println(parString);
             #endif
             toggleSingleKeys(parString);
             break;
      case CMD_KR:
             #ifdef DEBUG_OUTPUT   
               Serial.print(F("key release: "));
               Serial.println(parString);
             #endif
             releaseSingleKeys(parString);             
             break;
      case CMD_RA:
             #ifdef DEBUG_OUTPUT 
               Serial.print(F("release all"));
             #endif
             release_all();             
             break;
            
      case CMD_SA:
             #ifdef DEBUG_OUTPUT 
               Serial.print(F("save slot "));
               Serial.println(parString);
             #endif
             release_all();
             saveToEEPROM(parString); 
          break;
      case CMD_LO:
             #ifdef DEBUG_OUTPUT 
               Serial.print(F("load slot: "));
               Serial.println(parString);
             #endif
             if (parString) {
               release_all();
               reportSlotParameters=REPORT_ONE_SLOT;
               readFromEEPROM(parString);
               reportSlotParameters=REPORT_NONE;
               if(PCBversion){
                 updateNeoPixelColor(actSlot);    // update the Slot color of the LED 
                 setBeepCount(actSlot);           // set some beep count -> time (dependend on loop time)
                 writeSlot2Display();             // update the info on the Display 
               }
             }
          break;
      case CMD_LA:   
             #ifdef DEBUG_OUTPUT 
               Serial.println(F("load all slots"));
             #endif
             release_all();
             reportSlotParameters=REPORT_ALL_SLOTS;
             readFromEEPROM(parString);
             reportSlotParameters=REPORT_NONE;
             readFromEEPROM(0);
          break;
      case CMD_LI:
             #ifdef DEBUG_OUTPUT 
               Serial.println(F("list slots: "));
             #endif
             release_all();
             listSlots();
          break; 
      case CMD_NE:
             #ifdef DEBUG_OUTPUT
               Serial.println(F("load next slot"));
               reportSlotParameters=REPORT_ONE_SLOT;
             #endif  

             release_all();
             readFromEEPROM(0);
             reportSlotParameters=REPORT_NONE;
             if(PCBversion){
               updateNeoPixelColor(actSlot);    // update the Slot color of the LED 
               setBeepCount(actSlot);           // set some beep count -> time (dependend on loop time)
               writeSlot2Display();             //update the info on the Display 
             }
             break;
      case CMD_DE:
             #ifdef DEBUG_OUTPUT   
               Serial.println(F("delete slots")); 
             #endif
             release_all();
             deleteSlots(); 
          break;
      case CMD_NC:
             #ifdef DEBUG_OUTPUT 
               Serial.println(F("no command"));
             #endif 
          break;
      case CMD_SR:
            reportRawValues=1;
          break;
      case CMD_ER:
            reportRawValues=0;
          break;
      case CMD_TS:
             #ifdef DEBUG_OUTPUT  
               Serial.println(F("set threshold sip"));
             #endif
             settings.ts=parNum;
          break;
      case CMD_TP:
             #ifdef DEBUG_OUTPUT  
               Serial.println(F("set threshold puff"));
             #endif
             settings.tp=parNum;
          break;
      case CMD_TT:
             #ifdef DEBUG_OUTPUT  
               Serial.println(F("set threshold time"));
             #endif
             settings.tt=parNum;
          break;
      case CMD_AP:
             #ifdef DEBUG_OUTPUT  
               Serial.println(F("set antitremor press"));
             #endif
             settings.ap=parNum;
          break;
      case CMD_AR:
             #ifdef DEBUG_OUTPUT  
               Serial.println(F("set antitremor release"));
             #endif
             settings.ar=parNum;
          break;
      case CMD_AI:
             #ifdef DEBUG_OUTPUT  
               Serial.println(F("set antitremor idle"));
             #endif
             settings.ai=parNum;
          break;
      case CMD_MA:
             { 
               // execute a command macro                
               // make a copy from keystring because processing will be destructive...
               memmove(cmdstring, parString, strlen(parString) + 1);
               uint8_t lastCmd=0;
               char * cmdStart=cmdstring, *cmdEnd;

               // do the macro stuff: feed single commands to parser, separator: ';'
               do {
                 cmdEnd=cmdStart;
                 while ((*cmdEnd!=';') && (*cmdEnd)) {
                  // use backslash for passing special characters (; or \). note: copy also 0-delimiter! 
                  if (*cmdEnd =='\\') memmove (cmdEnd, cmdEnd+1, strlen(cmdEnd));
                  cmdEnd++;
                 }
                 if (!(*cmdEnd)) lastCmd=1;
                 else *cmdEnd=0; 
                 parseCommand(cmdStart);
                 cmdStart=cmdEnd+1;
                 if (!(*cmdStart)) lastCmd=1;
               } while (!lastCmd);                 
             }
             break;
      case CMD_WA:
              delay(parNum);
             break;
      case CMD_DP:
              #ifdef DEBUG_OUTPUT  
                Serial.print(F("Next Slot on Double Press = "));
                Serial.println(parNum);
              #endif
              settings.dp=parNum;
             break;
      case CMD_AD:
              #ifdef DEBUG_OUTPUT  
                Serial.print(F("Automatic Dwell Time = "));
                Serial.println(parNum);
              #endif
              settings.ad=parNum;
             break;
      case CMD_FR:
             Serial.print(F("FREE EEPROM (%):"));
             Serial.println(getfreeEEPROM());
             break;
      case CMD_BT:
            settings.bt = parNum;
            break;    
      case CMD_BC:
            Serial1.write(parString);
            Serial1.write('\n'); //terminate command
            break;              
  }
}
