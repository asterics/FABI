/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: reporting.h - implementation of serial reports

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/  


#include "FlipWare.h"
#include "parser.h"
#include "reporting.h"

/**
  static variables for report management
*/
uint8_t reportRawValues = 0;

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
  str[0] = '0';
  str[1] = 'x';
  for (int i = 7; i >= 2; i--) {
    int digit = val % 16;
    if (digit > 9) str[i] = 'a' + digit - 10;
    else str[i] = '0' + digit;
    val >>= 4;
  }
  str[8] = 0;
}


/** 
 * @brief Print current to given stream
 * @param S Stream to send the AT commands to; in our case Serial or a File
 */
void printCurrentSlot(Stream *S)
{
  char tmp[10];
  S->println(slotSettings.slotName);
  //IR timeout only if not default value
  if(get_IR_timeout() != IR_EDGE_TIMEOUT_US) {
    S->print("AT IT "); S->println(get_IR_timeout());
  }
  S->print("AT TS "); S->println(slotSettings.ts);
  S->print("AT TP "); S->println(slotSettings.tp);
  S->print("AT WS "); S->println(slotSettings.ws);
  S->print("AT SP "); S->println(slotSettings.sp);
  S->print("AT SS "); S->println(slotSettings.ss);
  S->print("AT RO "); S->println(slotSettings.ro);
  S->print("AT BT "); S->println(slotSettings.bt);
  S->print("AT KL "); S->println(slotSettings.kbdLayout);
  S->print("AT SC "); makehex(slotSettings.sc, tmp); S->println(tmp);

  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    S->print("AT BM ");
    if (i < 9) S->print("0"); // leading zero for button numbers !
    S->println(i + 1);
    int actCmd = buttons[i].mode;
    char cmdStr[4];

    if ((actCmd <0 ) || (actCmd>=NUM_COMMANDS)) {
      S->print("E: buttonmode =");
      S->println(actCmd);
      actCmd=CMD_NC;
    }
    
    S->print("AT ");
    strcpy_FM(cmdStr, (uint_farptr_t_FM)atCommands[actCmd].atCmd);
    S->print(cmdStr);
    switch (pgm_read_byte_near(&(atCommands[actCmd].partype)))
    {
      case PARTYPE_UINT:
      case PARTYPE_INT:  S->print(" "); S->print(buttons[i].value); break;
      case PARTYPE_STRING: S->print(" "); S->print(buttonKeystrings[i]); break;
    }
    S->println("");
  }
  S->flush();
}

void reportValues()
{
  static uint8_t valueReportCount = 0;
  
  if (!reportRawValues)   return;

  if (valueReportCount++ > 50/UPDATE_INTERVAL) {      // report raw values approx. every 50ms !
    Serial.print("VALUES:"); Serial.print(sensorData.pressure); Serial.print(",");
    for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
      if (buttonStates & (1 << i)) Serial.print("1");
      else Serial.print("0");
    }
    Serial.print(",");
    Serial.print(actSlot);
    Serial.println("");
    valueReportCount = 0;
  }
}
