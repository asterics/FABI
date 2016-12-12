// --------------------------------------------------------------
// Project : FabiGUI_GTK
// --------------------------------------------------------------
// File : parser.c
// Author : Benjamin Aigner
// Company : FH Technikum Wien
// Last update :
// Platform :
// --------------------------------------------------------------
// Description : <A brief explanation about the unit >
// --------------------------------------------------------------

#include "parser.h"
#include "commands.h"

void gotSlotName(char *);
void parseCommand (char * cmdstr);


void parseByte (char newByte)  // parse an incoming commandbyte from serial interface, perform command if valid
{
   static uint8_t cmdlen=0;
   static uint8_t actpos=0;
   static char slotname[256];
   static char cmdstring[256];
   static uint8_t readstate=0;

      switch (readstate) {
        case 0:
                if ((newByte=='A') || (newByte=='a')) readstate=1;
                else if (newByte=='S') readstate=10;
             break;
        case 1:
                if ((newByte=='T') || (newByte=='t')) readstate++; else readstate=0;
            break;
        case 2:
                if ((newByte==13) || (newByte==10))  // AT reply: "OK"
                {  printf("OK\n");  readstate=0; }
                else if (newByte==' ') { cmdlen=0; readstate++; }
                else goto err;
            break;
        case 3:
                if ((newByte==13) || (newByte==10) || (cmdlen>=MAX_CMDLEN-1))
                {  cmdstring[cmdlen]=0;  parseCommand(cmdstring); readstate=0; }
                else cmdstring[cmdlen++]=newByte;
            break;

         case 10:  if (newByte=='l') readstate++; else readstate=0; break;
         case 11:  if (newByte=='o') readstate++; else readstate=0; break;
         case 12:  if (newByte=='t') readstate++; else readstate=0; break;
         case 13:  if (newByte==':') { printf("omitting slot report\n"); readstate=0; }
                  else if ((newByte>='0') && (newByte<='9')) {actpos=0; readstate++;}
                  else readstate=0; break;
         case 14:  if (newByte==':') readstate++; else readstate=0; break;
         case 15:  if ((newByte!=0)&&(newByte!='\r')&&(newByte!='\n'))
                   {slotname[actpos]=newByte; actpos++;}
                   else {slotname[actpos]=0; gotSlotName(slotname); readstate=0;}
                break;

        default: err: printf("?\n");readstate=0;
   }
}
