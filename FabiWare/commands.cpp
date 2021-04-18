
#include "fabi.h"


const struct atCommandType atCommands[] PROGMEM = {
    {"ID"  , PARTYPE_NONE },  {"BM"  , PARTYPE_UINT }, {"CL"  , PARTYPE_NONE }, {"CR"  , PARTYPE_NONE },
    {"CM"  , PARTYPE_NONE },  {"CD"  , PARTYPE_NONE }, {"PL"  , PARTYPE_NONE }, {"PR"  , PARTYPE_NONE },
    {"PM"  , PARTYPE_NONE },  {"RL"  , PARTYPE_NONE }, {"RR"  , PARTYPE_NONE }, {"RM"  , PARTYPE_NONE },
    {"WU"  , PARTYPE_NONE },  {"WD"  , PARTYPE_NONE }, {"WS"  , PARTYPE_UINT }, {"MX"  , PARTYPE_INT  },
    {"MY"  , PARTYPE_INT  },  {"KW"  , PARTYPE_STRING},{"KP"  , PARTYPE_STRING},{"KR"  , PARTYPE_STRING},
    {"RA"  , PARTYPE_NONE },  {"SA"  , PARTYPE_STRING},{"LO"  , PARTYPE_STRING},{"LA"  , PARTYPE_NONE },
    {"LI"  , PARTYPE_NONE },  {"NE"  , PARTYPE_NONE }, {"DE"  , PARTYPE_NONE }, {"NC"  , PARTYPE_NONE }, 
    {"SR"  , PARTYPE_NONE },  {"ER"  , PARTYPE_NONE },
    {"TS"  , PARTYPE_UINT },  {"TP"  , PARTYPE_UINT }, {"MA"  , PARTYPE_STRING},{"WA"  , PARTYPE_UINT  },
    {"TT"  , PARTYPE_UINT },  {"AP"  , PARTYPE_UINT }, {"AR"  , PARTYPE_UINT},  {"AI"  , PARTYPE_UINT  },
    {"FR"  , PARTYPE_NONE },  {"BT"  , PARTYPE_UINT }
};

void printCurrentSlot()
{
        Serial.print(F("Slot:"));  Serial.println(settings.slotname);
        Serial.print(F("AT WS ")); Serial.println(settings.ws);
        Serial.print(F("AT TS ")); Serial.println(settings.ts);
        Serial.print(F("AT TP ")); Serial.println(settings.tp);
        Serial.print(F("AT TT ")); Serial.println(settings.tt);
        Serial.print(F("AT AP ")); Serial.println(settings.ap);
        Serial.print(F("AT AR ")); Serial.println(settings.ar);
        Serial.print(F("AT AI ")); Serial.println(settings.ai);
        Serial.print(F("AT BT ")); Serial.println(settings.bt);
        for (int i=0;i<NUMBER_OF_BUTTONS;i++) 
        {
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


// perform a command  (called from parser.cpp)
//   cmd: command identifier
//   parNum: optional numeric parameter
//   parString: optional numeric parameter
//   periodicMouseMovement: if true, mouse will continue moving - if false: only one movement
void performCommand (uint8_t cmd, int16_t parNum, char * parString, int8_t periodicMouseMovement)
{
    static uint8_t actButton=0;

    if (actButton != 0)  // if last command was BM (set buttonmode): store current command for this button !!
    {
        #ifdef DEBUG_OUTPUT
          Serial.print(F("got new mode for button ")); Serial.print(actButton);Serial.print(':');
          Serial.print(cmd);Serial.print(',');Serial.print(parNum);Serial.print(',');Serial.println(parString);
        #endif
        buttons[actButton-1].mode=cmd;
        buttons[actButton-1].value=parNum;
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
        return;  // do not actually execute the command (just store it)
    }
    
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
        case CMD_PL:
               #ifdef DEBUG_OUTPUT
                 Serial.println(F("press left"));
              #endif
               leftMouseButton=1; 
               break;
        case CMD_PR:
               #ifdef DEBUG_OUTPUT
                 Serial.println(F("press right"));
               #endif
               rightMouseButton=1; 
               break;
        case CMD_PM:
               #ifdef DEBUG_OUTPUT
                 Serial.println(F("press middle"));
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
               //beepXtimes(actSlot);     //audio feedback for the current slot 
               
              if(PCBversion){
                updateSlot(actSlot);    // update the Slot color of the LED 
                setBeepCount(actSlot);  // set some beep count -> time (dependend on loop time)
                writeSlot2Display();  	//update the info on the Display 
              }

               reportSlotParameters=REPORT_NONE;
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
        case CMD_FR:
               Serial.print(F("FREE EEPROM (%):"));
               Serial.println((int)((uint32_t) freeEEPROMbytes * 100 / EEPROM_SIZE));
               break;
        case CMD_BT:
              settings.bt = parNum;
              break;    
    }
}
