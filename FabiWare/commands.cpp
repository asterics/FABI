
#include "fabi.h"

uint8_t actButton=0;

const struct atCommandType atCommands[] PROGMEM = {
    {"ID"  , PARTYPE_NONE },  {"BM"  , PARTYPE_UINT }, {"CL"  , PARTYPE_NONE }, {"CR"  , PARTYPE_NONE },
    {"CM"  , PARTYPE_NONE },  {"CD"  , PARTYPE_NONE }, {"PL"  , PARTYPE_NONE }, {"PR"  , PARTYPE_NONE },
    {"PM"  , PARTYPE_NONE },  {"RL"  , PARTYPE_NONE }, {"RR"  , PARTYPE_NONE }, {"RM"  , PARTYPE_NONE },
    {"WU"  , PARTYPE_NONE },  {"WD"  , PARTYPE_NONE }, {"WS"  , PARTYPE_UINT }, {"MX"  , PARTYPE_INT  },
    {"MY"  , PARTYPE_INT  },  {"KW"  , PARTYPE_STRING},{"KP"  , PARTYPE_STRING},{"KR"  , PARTYPE_STRING},
    {"RA"  , PARTYPE_NONE },  {"SA"  , PARTYPE_STRING},{"LO"  , PARTYPE_STRING},{"LA"  , PARTYPE_NONE },
    {"LI"  , PARTYPE_NONE },  {"NE"  , PARTYPE_NONE }, {"DE"  , PARTYPE_NONE }, {"NC"  , PARTYPE_NONE }, 
};


void initButtons() {
     buttons[0].mode=CMD_NE;  // default function for first button: switch to next slot
     buttons[1].mode=CMD_KP; strcpy(buttons[1].keystring,"KEY_ESC ");;
     buttons[2].mode=CMD_NC;  // no command
     buttons[3].mode=CMD_KP; strcpy(buttons[3].keystring,"KEY_UP ");
     buttons[4].mode=CMD_KP; strcpy(buttons[4].keystring,"KEY_DOWN ");
     buttons[5].mode=CMD_KP; strcpy(buttons[5].keystring,"KEY_LEFT ");
     buttons[6].mode=CMD_KP; strcpy(buttons[6].keystring,"KEY_RIGHT ");
     buttons[7].mode=CMD_PL;   // press left mouse button
     buttons[8].mode=CMD_NC;   // no command 
     buttons[9].mode=CMD_CD;   // click double                        
     buttons[10].mode=CMD_CR;  // click right      
     buttons[11].mode=CMD_NC;  // no command      
}


void printCurrentSlot()
{
        Serial.print("Slot:");  Serial.println(settings.slotname);
        Serial.print("AT WS "); Serial.println(settings.ws);
        for (int i=0;i<NUMBER_OF_BUTTONS;i++) 
        {
           Serial.print("AT BM "); 
           if (i<9) Serial.print("0");
           Serial.println(i+1); 
           Serial.print("AT "); 
           int actCmd = buttons[i].mode;
           char cmdStr[4];
           strcpy_FM(cmdStr,(uint_farptr_t_FM) atCommands[actCmd].atCmd);
           Serial.print(cmdStr);
            switch (pgm_read_byte_near(&(atCommands[actCmd].partype))) 
            {
               case PARTYPE_UINT: 
               case PARTYPE_INT:  Serial.print(" ");Serial.print(buttons[i].value); break;
               case PARTYPE_STRING: Serial.print(" ");Serial.print(buttons[i].keystring); break;
            }
            Serial.println("");
        }
}



// perform a command  (called from parser.cpp)
//   cmd: command identifier
//   par1: optional numeric parameter
//   periodicMouseMovement: if true, mouse will continue moving - if false: only one movement
void performCommand (uint8_t cmd, int16_t par1, char * keystring, int8_t periodicMouseMovement)
{
    if (actButton != 0)  // if last command was BM (set buttonmode): store current command for this button !!
    {
        if (DebugOutput==DEBUG_FULLOUTPUT)
        {  
          Serial.print("got new mode for button "); Serial.print(actButton);Serial.print(":");
          Serial.print(cmd);Serial.print(",");Serial.print(par1);Serial.print(",");Serial.println(keystring);
        }
        buttons[actButton-1].mode=cmd;
        buttons[actButton-1].value=par1;
        if (keystring==0) buttons[actButton-1].keystring[0]=0;
        else strcpy(buttons[actButton-1].keystring,keystring);
        actButton=0;
        return;  // do not actually execute the command (just store it)
    }
    
    switch(cmd) {
        case CMD_ID:
               Serial.println(VERSION_STRING); 
            break;
        case CMD_BM:
               release_all();
               //if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.print("set mode for button "); Serial.println(par1);
               if ((par1>0) && (par1<=NUMBER_OF_BUTTONS))
                   actButton=par1;
               else  Serial.println("?");
            break;
        
        case CMD_CL:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("click left");
               leftMouseButton=1;  leftClickRunning=DEFAULT_CLICK_TIME;
               break;
        case CMD_CR:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("click right");
               rightMouseButton=1; rightClickRunning=DEFAULT_CLICK_TIME;
               break;
        case CMD_CD:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("click double");
               leftMouseButton=1;  doubleClickRunning=DEFAULT_CLICK_TIME*DOUBLECLICK_MULTIPLIER;
               break;
        case CMD_CM:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("click middle");
               middleMouseButton=1; middleClickRunning=DEFAULT_CLICK_TIME;
              break;
        case CMD_PL:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("press left");
               leftMouseButton=1; 
               break;
        case CMD_PR:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("press right");
               rightMouseButton=1; 
               break;
        case CMD_PM:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("press middle");
               middleMouseButton=1; 
               break;
        case CMD_RL:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("release left");
               leftMouseButton=0;
               break; 
        case CMD_RR:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("release right");
               rightMouseButton=0;
               break; 
        case CMD_RM:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("release middle");
               middleMouseButton=0;
               break; 
        case CMD_WU:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("wheel up");
                 #ifdef ARDUINO_PRO_MICRO
                    Mouse.move(0,0,-settings.ws); 
                 #else
                    Mouse.scroll(-settings.ws); 
                 #endif
            break;
        case CMD_WD:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("wheel down");
                 #ifdef ARDUINO_PRO_MICRO
                    Mouse.move(0,0,settings.ws); 
                 #else
                    Mouse.scroll(settings.ws); 
                 #endif
            break;
        case CMD_WS:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("wheel step");
               settings.ws=par1;
            break;
        case CMD_MX:
               if (DebugOutput==DEBUG_FULLOUTPUT) 
               {  Serial.print("mouse move x "); Serial.println(par1); }
               Mouse.move(par1, 0);
               if (periodicMouseMovement) moveX=par1;
            break;
        case CMD_MY:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
               {  Serial.print("mouse move y "); Serial.println(par1); }
               Mouse.move(0, par1);
               if (periodicMouseMovement) moveY=par1;
            break;
        case CMD_KW:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
               {  Serial.print("keyboard write: "); Serial.println(keystring); }
               writeKeystring=keystring;
               break;
        case CMD_KP:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
               {  Serial.print("key press: "); Serial.println(keystring); }
               if (keystring[strlen(keystring)-1] != ' ') strcat(keystring," ");
               setKeyValues(keystring);
               break;
        case CMD_KR:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
               {  Serial.print("key release: ");  Serial.println(keystring); }
               strcat(keystring," ");
               releaseKeys(keystring);             
               break;
        case CMD_RA:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.print("release all");
               release_all();             
               break;
              
        case CMD_SA:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
               {  Serial.print("save slot ");  Serial.println(keystring); }
               release_all();
               saveToEEPROM(keystring); 
            break;
        case CMD_LO:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
               {  Serial.print("load slot: "); Serial.println(keystring); }
               if (keystring) {
                 release_all();
                 readFromEEPROM(keystring);
               }
            break;
        case CMD_LA:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("load all slots");
               release_all();
               reportSlotParameters=1;
               readFromEEPROM(keystring);
               reportSlotParameters=0;
               // readFromEEPROM(0);
            break;
        case CMD_LI:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("list slots: ");
               release_all();
               // reportSlotParameters=1;   // connection to host: start reporting slot parameters !
               listSlots();
            break;
        case CMD_NE:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.print("load next slot");
               release_all();
               readFromEEPROM(0);
               break;
        case CMD_DE:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("delete slots"); 
               release_all();
               deleteSlots(); 
            break;
        case CMD_NC:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("no command"); 
            break;
    
    }
}
