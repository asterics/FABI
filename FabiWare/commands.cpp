
#include "fabi.h"

uint8_t actButton=0;
extern void parseCommand (char * cmdstr);


const struct atCommandType atCommands[] PROGMEM = {
    {"ID"  , PARTYPE_NONE },  {"BM"  , PARTYPE_UINT }, {"CL"  , PARTYPE_NONE }, {"CR"  , PARTYPE_NONE },
    {"CM"  , PARTYPE_NONE },  {"CD"  , PARTYPE_NONE }, {"PL"  , PARTYPE_NONE }, {"PR"  , PARTYPE_NONE },
    {"PM"  , PARTYPE_NONE },  {"RL"  , PARTYPE_NONE }, {"RR"  , PARTYPE_NONE }, {"RM"  , PARTYPE_NONE },
    {"WU"  , PARTYPE_NONE },  {"WD"  , PARTYPE_NONE }, {"WS"  , PARTYPE_UINT }, {"MX"  , PARTYPE_INT  },
    {"MY"  , PARTYPE_INT  },  {"KW"  , PARTYPE_STRING},{"KP"  , PARTYPE_STRING},{"KR"  , PARTYPE_STRING},
    {"RA"  , PARTYPE_NONE },  {"SA"  , PARTYPE_STRING},{"LO"  , PARTYPE_STRING},{"LA"  , PARTYPE_NONE },
    {"LI"  , PARTYPE_NONE },  {"NE"  , PARTYPE_NONE }, {"DE"  , PARTYPE_NONE }, {"NC"  , PARTYPE_NONE }, 
    {"E1"  , PARTYPE_NONE },  {"E0"  , PARTYPE_NONE }, {"SR"  , PARTYPE_NONE }, {"ER"  , PARTYPE_NONE },
    {"TS"  , PARTYPE_UINT },  {"TP"  , PARTYPE_UINT }, {"MA"  , PARTYPE_STRING},{"WA"  , PARTYPE_UINT  }
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
        Serial.print(F("Slot:"));  Serial.println(settings.slotname);
        Serial.print(F("AT WS ")); Serial.println(settings.ws);
        Serial.print(F("AT TS ")); Serial.println(settings.ts);
        Serial.print(F("AT TP ")); Serial.println(settings.tp);
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
               case PARTYPE_STRING: Serial.print(' ');Serial.print(buttons[i].keystring); break;
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
        if (DebugOutput==1) {
          Serial.print(F("got new mode for button ")); Serial.print(actButton);Serial.print(':');
          Serial.print(cmd);Serial.print(',');Serial.print(par1);Serial.print(',');Serial.println(keystring);
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
               Serial.println(F(VERSION_STRING)); 
            break;
        case CMD_BM:
               release_all();
               if (DebugOutput==1)
                  Serial.print(F("set mode for button ")); Serial.println(par1);
               
               if ((par1>0) && (par1<=NUMBER_OF_BUTTONS))
                   actButton=par1;
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
                  Mouse.move(0,0,-settings.ws); 
               #else
                  Mouse.scroll(-settings.ws); 
               #endif
            break;
        case CMD_WD:
               if (DebugOutput==1)
                 Serial.println(F("wheel down"));
              
               #ifdef ARDUINO_PRO_MICRO
                  Mouse.move(0,0,settings.ws); 
               #else
                  Mouse.scroll(settings.ws); 
               #endif
            break;
        case CMD_WS:
               if (DebugOutput==1)
                 Serial.println(F("wheel step"));
               
               settings.ws=par1;
            break;
        case CMD_MX:
               if (DebugOutput==1) 
               {  Serial.print(F("mouse move x ")); Serial.println(par1); }
               if (periodicMouseMovement) moveX=par1;
               else Mouse.move(par1, 0);
            break;
        case CMD_MY:
               if (DebugOutput==1)   
               {  Serial.print(F("mouse move y ")); Serial.println(par1); }
               if (periodicMouseMovement) moveY=par1;
               else Mouse.move(0, par1);
            break;
        case CMD_KW:
               if (DebugOutput==1)   
               {  Serial.print(F("keyboard write: ")); Serial.println(keystring); }
               writeKeystring=keystring;
               break;
        case CMD_KP:
               if (DebugOutput==1)   
               {  Serial.print(F("key press: ")); Serial.println(keystring); }
               if (keystring[strlen(keystring)-1] != ' ') strcat(keystring," ");
               setKeyValues(keystring);
               break;
        case CMD_KR:
               if (DebugOutput==1)   
               {  Serial.print(F("key release: "));  Serial.println(keystring); }
               strcat(keystring," ");
               releaseKeys(keystring);             
               break;
        case CMD_RA:
               if (DebugOutput==1) 
                 Serial.print(F("release all"));
               
               release_all();             
               break;
              
        case CMD_SA:
               if (DebugOutput==1) 
               {  Serial.print(F("save slot "));  Serial.println(keystring); }
               release_all();
               saveToEEPROM(keystring); 
            break;
        case CMD_LO:
               if (DebugOutput==1) 
               {  Serial.print(F("load slot: ")); Serial.println(keystring); }
               if (keystring) {
                 release_all();
                 reportSlotParameters=REPORT_ONE_SLOT;
                 readFromEEPROM(keystring);
                 reportSlotParameters=REPORT_NONE;
               }
            break;
        case CMD_LA:
               if (DebugOutput==1) 
                 Serial.println(F("load all slots"));
               
               release_all();
               reportSlotParameters=REPORT_ALL_SLOTS;
               readFromEEPROM(keystring);
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
               settings.ts=par1;
            break;
        case CMD_TP:
               if (DebugOutput==1)  
                 Serial.println(F("set threshold puff"));
               settings.tp=par1;
            break;
        case CMD_MA:
               {
                 char current[MAX_KEYSTRING_LEN];  // TBD: save memory here via improved command extraction ...
                 char *cmd_copy_ptr, backslash;
                 uint8_t len;

                 // do the macro stuff: feed single commands to parser, seperator: ';'
                 cmd_copy_ptr=keystring;
                 while (*cmd_copy_ptr)
                 {
                    len=0;backslash=0;
                    while ((*cmd_copy_ptr) && ((*cmd_copy_ptr != ';') || backslash) && (len<MAX_KEYSTRING_LEN-1))
                    {
                       if ((*cmd_copy_ptr == '\\') && (!backslash))   // check for escape character
                         backslash=1; 
                       else  {
                        current[len++] = *cmd_copy_ptr;
                        backslash=0;
                      }
                      cmd_copy_ptr++;
                    }
                    current[len]=0; 
                    parseCommand(current);
                    if (*cmd_copy_ptr) cmd_copy_ptr++;
                 }
               }
               break;
        case CMD_WA:
                delay(par1);
               break;
    
    }
}
