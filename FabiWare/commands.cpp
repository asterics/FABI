
#include "fabi.h"


const struct atCommandType atCommands[] PROGMEM = {
    {"ID"  , PARTYPE_NONE },  {"BM"  , PARTYPE_UINT }, {"CL"  , PARTYPE_NONE }, {"CR"  , PARTYPE_NONE },
    {"CM"  , PARTYPE_NONE },  {"CD"  , PARTYPE_NONE }, {"PL"  , PARTYPE_NONE }, {"PR"  , PARTYPE_NONE },
    {"PM"  , PARTYPE_NONE },  {"RL"  , PARTYPE_NONE }, {"RR"  , PARTYPE_NONE }, {"RM"  , PARTYPE_NONE },
    {"WU"  , PARTYPE_NONE },  {"WD"  , PARTYPE_NONE }, {"WS"  , PARTYPE_UINT }, {"MX"  , PARTYPE_INT  },
    {"MY"  , PARTYPE_INT  },  {"KW"  , PARTYPE_STRING},{"KP"  , PARTYPE_STRING},{"KR"  , PARTYPE_STRING},
    {"RA"  , PARTYPE_NONE },  {"SA"  , PARTYPE_STRING},{"LO"  , PARTYPE_STRING},{"LA"  , PARTYPE_NONE },
    {"LI"  , PARTYPE_NONE },  {"NE"  , PARTYPE_NONE }, {"DE"  , PARTYPE_NONE }, {"NC"  , PARTYPE_NONE }, 
    {"E1"  , PARTYPE_NONE },  {"E0"  , PARTYPE_NONE }, {"SR"  , PARTYPE_NONE }, {"ER"  , PARTYPE_NONE },
    {"TS"  , PARTYPE_UINT },  {"TP"  , PARTYPE_UINT }, {"MA"  , PARTYPE_STRING},{"WA"  , PARTYPE_UINT  },
    {"TT"  , PARTYPE_UINT },  {"AP"  , PARTYPE_UINT }, {"AR"  , PARTYPE_UINT},  {"AI"  , PARTYPE_UINT  },
    {"FR"  , PARTYPE_NONE }
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
        if (DebugOutput==1) {
          Serial.print(F("got new mode for button ")); Serial.print(actButton);Serial.print(':');
          Serial.print(cmd);Serial.print(',');Serial.print(parNum);Serial.print(',');Serial.println(parString);
        }
        buttons[actButton-1].mode=cmd;
        buttons[actButton-1].value=parNum;
        if (parString==0) setKeystring(actButton-1,"");
        else setKeystring(actButton-1,parString);
        actButton=0;
        if (DebugOutput==1) {
           printKeystrings();
        }
        return;  // do not actually execute the command (just store it)
    }
    
    switch(cmd) {
        case CMD_ID:
               Serial.println(F(VERSION_STRING)); 
            break;
        case CMD_BM:
               release_all();
               if (DebugOutput==1)
               {   Serial.print(F("set mode for button ")); Serial.println(parNum); }
               
               if ((parNum>0) && (parNum<=NUMBER_OF_BUTTONS))
                   actButton=parNum;
               else  Serial.println('?');
            break;
        
        case CMD_CL:
               if (DebugOutput==1)
                 Serial.println(F("click left"));
               
               leftMouseButton=1;  leftClickRunning=DEFAULT_CLICK_TIME;
               break;
        case CMD_CR:
               if (DebugOutput==1)
                 Serial.println(F("click right"));
               
               rightMouseButton=1; rightClickRunning=DEFAULT_CLICK_TIME;
               break;
        case CMD_CD:
               if (DebugOutput==1)
                 Serial.println(F("click double"));
               
               leftMouseButton=1;  doubleClickRunning=DEFAULT_CLICK_TIME*DOUBLECLICK_MULTIPLIER;
               break;
        case CMD_CM:
               if (DebugOutput==1)
                 Serial.println(F("click middle"));
               
               middleMouseButton=1; middleClickRunning=DEFAULT_CLICK_TIME;
              break;
        case CMD_PL:
               if (DebugOutput==1)
                 Serial.println(F("press left"));
              
               leftMouseButton=1; 
               break;
        case CMD_PR:
               if (DebugOutput==1)
                 Serial.println(F("press right"));
               
               rightMouseButton=1; 
               break;
        case CMD_PM:
               if (DebugOutput==1)
                 Serial.println(F("press middle"));
               
               middleMouseButton=1; 
               break;
        case CMD_RL:
               if (DebugOutput==1)
                 Serial.println(F("release left"));
              
               leftMouseButton=0;
               break; 
        case CMD_RR:
               if (DebugOutput==1)
                 Serial.println(F("release right"));
               
               rightMouseButton=0;
               break; 
        case CMD_RM:
               if (DebugOutput==1)
                 Serial.println(F("release middle"));
               
               middleMouseButton=0;
               break; 
        case CMD_WU:
               if (DebugOutput==1)
                 Serial.println(F("wheel up"));
               
               #ifdef ARDUINO_PRO_MICRO
				  if(settings.ws != 0) Mouse.move(0,0,-settings.ws); 
				  else Mouse.move(0,0,-DEFAULT_WHEEL_STEPSIZE); 
               #else
				  if(settings.ws != 0) Mouse.scroll(-settings.ws); 
				  else Mouse.scroll(-DEFAULT_WHEEL_STEPSIZE); 
               #endif
            break;
        case CMD_WD:
               if (DebugOutput==1)
                 Serial.println(F("wheel down"));
              
               #ifdef ARDUINO_PRO_MICRO
				  if(settings.ws != 0) Mouse.move(0,0,settings.ws); 
				  else Mouse.move(0,0,DEFAULT_WHEEL_STEPSIZE); 
               #else
				  if(settings.ws != 0) Mouse.scroll(settings.ws); 
				  else Mouse.scroll(DEFAULT_WHEEL_STEPSIZE); 
               #endif
            break;
        case CMD_WS:
               if (DebugOutput==1)
                 Serial.println(F("wheel step"));
               
               settings.ws=parNum;
            break;
        case CMD_MX:
               if (DebugOutput==1) 
               {  Serial.print(F("mouse move x ")); Serial.println(parNum); }
               if (periodicMouseMovement) moveX=parNum;
               else Mouse.move(parNum, 0);
            break;
        case CMD_MY:
               if (DebugOutput==1)   
               {  Serial.print(F("mouse move y ")); Serial.println(parNum); }
               if (periodicMouseMovement) moveY=parNum;
               else Mouse.move(0, parNum);
            break;
        case CMD_KW:
               if (DebugOutput==1)   
               { Serial.print(F("keyboard write: ")); Serial.println(parString); } 
               sendToKeyboard(parString);
               break;
        case CMD_KP:
               if (DebugOutput==1)   
               {  Serial.print(F("key press: ")); Serial.println(parString); }
               pressSingleKeys(parString);
               break;
        case CMD_KR:
               if (DebugOutput==1)   
               {  Serial.print(F("key release: "));  Serial.println(parString); }
               releaseSingleKeys(parString);             
               break;
        case CMD_RA:
               if (DebugOutput==1) 
                 Serial.print(F("release all"));
               
               release_all();             
               break;
              
        case CMD_SA:
               if (DebugOutput==1) 
               {  Serial.print(F("save slot "));  Serial.println(parString); }
               release_all();
               saveToEEPROM(parString); 
            break;
        case CMD_LO:
               if (DebugOutput==1) 
               {  Serial.print(F("load slot: ")); Serial.println(parString); }
               if (parString) {
                 release_all();
                 reportSlotParameters=REPORT_ONE_SLOT;
                 readFromEEPROM(parString);
                 reportSlotParameters=REPORT_NONE;
               }
            break;
        case CMD_LA:
               if (DebugOutput==1) 
                 Serial.println(F("load all slots"));
               
               release_all();
               reportSlotParameters=REPORT_ALL_SLOTS;
               readFromEEPROM(parString);
               reportSlotParameters=REPORT_NONE;
               readFromEEPROM(0);
            break;
        case CMD_LI:
               if (DebugOutput==1) 
                 Serial.println(F("list slots: "));
               
               release_all();
               listSlots();
            break;
        case CMD_NE:
               if (DebugOutput==1) {
                 Serial.println(F("load next slot"));
                 reportSlotParameters=REPORT_ONE_SLOT;
               }
               release_all();
               readFromEEPROM(0);
               reportSlotParameters=REPORT_NONE;
               break;
        case CMD_DE:
               if (DebugOutput==1)   
                 Serial.println(F("delete slots")); 
               
               release_all();
               deleteSlots(); 
            break;
        case CMD_NC:
               if (DebugOutput==1) 
                 Serial.println(F("no command")); 
            break;
        case CMD_E1:
               DebugOutput=1; 
               Serial.println(F("echo on")); 
            break;
        case CMD_E0:
               DebugOutput=0; 
            break;
        case CMD_SR:
              reportRawValues=1;
            break;
        case CMD_ER:
              reportRawValues=0;
            break;
        case CMD_TS:
               if (DebugOutput==1)  
                 Serial.println(F("set threshold sip"));
               settings.ts=parNum;
            break;
        case CMD_TP:
               if (DebugOutput==1)  
                 Serial.println(F("set threshold puff"));
               settings.tp=parNum;
            break;
        case CMD_TT:
               if (DebugOutput==1)  
                 Serial.println(F("set threshold time"));
               settings.tt=parNum;
            break;
        case CMD_AP:
               if (DebugOutput==1)  
                 Serial.println(F("set antitremor press"));
               settings.ap=parNum;
            break;
        case CMD_AR:
               if (DebugOutput==1)  
                 Serial.println(F("set antitremor release"));
               settings.ar=parNum;
            break;
        case CMD_AI:
               if (DebugOutput==1)  
                 Serial.println(F("set antitremor idle"));
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
    
    }
}
