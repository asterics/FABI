/*
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: parser.cpp - serial command processing

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html
 
*/

#include "fabi.h"

char   cmdstring[MAX_CMDLEN];                 // buffer for incoming AT commands

/**
   @name get_uint
   @param char * str
   @param int16_t * result   
   @return uint8_t
   
   translates a (part of) an ASCII string to an integer variable
   only numeric values are allowed (-> only unsigned values)
   the result is stored into * result 
   the return value is true (1) if all characters were valid and the result was stored,
   else false(0) is returned.

*/
uint8_t get_uint(char * str, int16_t * result)
{
    int num=0;
    if ((str==0)||(*str==0)) return (0);
    while (*str)
    {
      if ((*str >= '0') && (*str<='9'))
         num=num*10+(*str - '0'); 
      else return(0);
      str ++;
    }
    *result=num;
    return(1);    
}

/**
   @name get_int
   @param char * str
   @param int16_t * result   
   @return uint8_t
   
   translates a (part of) an ASCII string to an integer variable
   only numeric values or a leading '-' are allowed (-> signed values possible)
   the result is stored into * result 
   the return value is true (1) if all characters were valid and the result was stored,
   else false(0) is returned.

*/
uint8_t get_int(char * str, int16_t * result)
{
    int16_t num,fact;
    if (str==0) return(0);
    if (*str =='-') {fact=-1; str++;} else fact=1;
    if (!get_uint(str,&num)) return(0);
    *result=num*fact;
    return(1);    
}


/**
   @name charUp
   @param char c
   @return char
   
   converts a character to uppercase
*/
char charUp (char c) {
  if ((c >= 'a') && (c<='z'))  
    c=c-'a'+'A';
  return(c);
}

/**
   @name strup
   @param char * str
   @return none
   
   converts a string to uppercase
*/
void strup (char * str)   // convert to upper case letters
{
  if (!str) return;
  while (*str)
  {
    *str=charUp(*str);
    str++;
  }
}


/**
   @name parseCommand
   @param char * cmdstr
   @return none
   
   parse AT commands
   this function checks if a command string matches a valid AT-commands (command identifier with arguments of correct type, eg. "AT MX 10")
   if the AT command is valid, the function performCommand is called (executing the AT command)
   if not, '?' is printed to the Serial interface
*/
void parseCommand (char * cmdstr)
{
    int8_t cmd=-1;
    int16_t num=0;

    
    cmdstr[strlen(cmdstr)+1]=0;  // to prevent exceeing the actual commandstring (when emptry string parameters are passed!)
    // Serial.print("parseCommand:"); Serial.println(cmdstr); 
    //char * actpos = strtok(cmdstr," ");   // see a nice explaination of strtok here:  http://www.reddit.com/r/arduino/comments/2h9l1l/using_the_strtok_function/

    // skip leading spaces
    char * actpos = cmdstr;
    while ( *actpos  == ' ') actpos++;

    if (strlen (actpos) > 1) {
                  
      for (int i=0;(i<NUM_COMMANDS)&&(cmd==-1);i++)
      {
        if ((charUp(actpos[0]) == pgm_read_byte_near(&(atCommands[i].atCmd[0]))) &&
            (charUp(actpos[1]) == pgm_read_byte_near(&(atCommands[i].atCmd[1])))) {

          // Serial.print ("partype="); Serial.println (pgm_read_byte_near(&(atCommands[i].partype)));
          switch (pgm_read_byte_near(&(atCommands[i].partype))) 
          {
             case PARTYPE_UINT: actpos+=2; while(*actpos==' ') actpos++;
                                if (get_uint(actpos, &num)) cmd=i ; actpos=0; break;
             case PARTYPE_INT:  actpos+=2; while(*actpos==' ') actpos++;  
                                if (get_int(actpos, &num)) cmd=i ; actpos=0; break;
             case PARTYPE_STRING: actpos+=3; cmd=i ;  break;
             default: cmd=i; actpos=0; break;
          }
        }
      }          
    } 
       
    if (cmd>-1)  performCommand(cmd,num,actpos,0);        
    else Serial.println('?');
}

/**
   @name parseByte
   @param int newByte
   @return none
   
   parse a single byte 
   this function receives single bytes in order to assemble AT command strings
   an AT command must start with the sequence "AT " and additional characters for the AT command identifier and arguments, followed by '\n' or '\r'
   if an AT command was found, it is forwarded to the AT command parser (parseCommand)
   if just "AT\r" is received, "OK" is printed to the Serial interface,
   else,  '?' is printed to the Serial interface
*/
void parseByte (int newByte)  // parse an incoming commandbyte from serial interface, perform command if valid
{
   static uint8_t readstate=0;
   static uint8_t cmdlen=0;
  
      switch (readstate) {
        case 0: 
                if (charUp(newByte)=='A') readstate++;
             break;
        case 1: 
                if (charUp(newByte)=='T') readstate++; else readstate=0;
            break;
        case 2: 
                if ((newByte==13) || (newByte==10))  // AT reply: "OK" 
                {  Serial.println(F("OK"));  readstate=0;       }
                else if (newByte==' ') { cmdlen=0; readstate++; } 
                else goto err;
            break;
        case 3: 
                if ((newByte==13) || (newByte==10))
                {  cmdstring[cmdlen]=0;  parseCommand(cmdstring); 
                  readstate=0; }
                else if(cmdlen<MAX_CMDLEN-2) cmdstring[cmdlen++]=newByte; 
            break;   
        default: 
            err: Serial.println('?');readstate=0;
   }
}
