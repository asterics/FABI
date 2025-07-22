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
#include "reporting.h"
#include "parser.h"
#include "sensors.h"
#include <LittleFS.h>

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
 * @brief Print current slot settings to given stream
 * @param S Stream to send the AT commands to; in our case Serial or a File
 */
void printCurrentSlot(Stream *S)
{
  char tmp[10];
  S->println(slotSettings.slotName);
  
  S->print("AT AX "); S->println(slotSettings.ax);
  S->print("AT AY "); S->println(slotSettings.ay);
  S->print("AT DX "); S->println(slotSettings.dx);
  S->print("AT DY "); S->println(slotSettings.dy);
  S->print("AT MS "); S->println(slotSettings.ms);
  S->print("AT AC "); S->println(slotSettings.ac);
  S->print("AT MM "); S->println(slotSettings.stickMode);
  S->print("AT RO "); S->println(slotSettings.ro);
  S->print("AT SB "); S->println(slotSettings.sb);
  //IR timeout only if not default value
  if(get_IR_timeout() != IR_EDGE_TIMEOUT_US) {
    S->print("AT IT "); S->println(get_IR_timeout());
  }
  S->print("AT TS "); S->println(slotSettings.ts);
  S->print("AT TP "); S->println(slotSettings.tp);
  S->print("AT WS "); S->println(slotSettings.ws);
  S->print("AT SP "); S->println(slotSettings.sp);
  S->print("AT SS "); S->println(slotSettings.ss);
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


/** 
 * @brief Print global settings to given stream
 * @param S Stream to send the AT commands to; in our case Serial or a File
 */
void printGlobalSettings(Stream *S)
{
  S->print("AT AV "); S->println(globalSettings.audioVolume);
  S->print("AT AB "); S->println(globalSettings.buzzerMode);
  S->print("AT AD "); S->println(globalSettings.thresholdAutoDwell);
  S->print("AT LP "); S->println(globalSettings.thresholdLongPress);
  S->print("AT MP "); S->println(globalSettings.thresholdMultiPress);
  S->flush();
}

void reportValues()
{
  static uint8_t valueReportCount = 0;
  
  if (!reportRawValues)   return;

  // update / override sip and puff button states for reporting
  if (sensorData.pressure < slotSettings.ts) buttonStates |= (1<<SIP_BUTTON);
  else buttonStates &= ~(1<<SIP_BUTTON);
  if (sensorData.pressure < slotSettings.ss) buttonStates |= (1<<STRONGSIP_BUTTON);
  else buttonStates &= ~(1<<STRONGSIP_BUTTON);
  if (sensorData.pressure > slotSettings.tp) buttonStates |= (1<<PUFF_BUTTON);
  else buttonStates &= ~(1<<PUFF_BUTTON);
  if (sensorData.pressure > slotSettings.sp) buttonStates |= (1<<STRONGPUFF_BUTTON);
  else buttonStates &= ~(1<<STRONGPUFF_BUTTON);

  if (valueReportCount++ > 50/UPDATE_INTERVAL) {      // report raw values approx. every 50ms !
    int32_t u=sensorData.yRaw+512; int32_t d=512-sensorData.yRaw;   // just for GUI compatibility with V2 (bar displays up/down)
    int32_t l=sensorData.xRaw+512; int32_t r=512-sensorData.xRaw;   // just for GUI compatibility with V2 (bar displays left/right)
    Serial.print("VALUES:"); Serial.print(sensorData.pressure); Serial.print(",");
    Serial.print(u); Serial.print(","); Serial.print(d); Serial.print(",");
    Serial.print(l); Serial.print(","); Serial.print(r); Serial.print(",");
    Serial.print(sensorData.xRaw); Serial.print(","); Serial.print(sensorData.yRaw); Serial.print(",");
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

void deleteUnusedFiles() {
    Dir dir = LittleFS.openDir("/"); // Open directory
    while (dir.next()) { // Iterate over files & subdirectories
        if ((!dir.isDirectory()) && (dir.fileName() != "rev.bin"))
        LittleFS.remove(dir.fileName());
    }
}

void listFilesRecursive(String dirName, int level) {
    Dir dir = LittleFS.openDir(dirName); // Open directory
    while (dir.next()) { // Iterate over files & subdirectories
        for (int i = 0; i < level; i++) Serial.print("  "); // Indentation

        Serial.print(dir.isDirectory() ? "[DIR] " : "File: ");
        Serial.print(dir.fileName());
        Serial.print(" - Size: ");
        Serial.print(dir.fileSize());
        Serial.println(" bytes");

        // If it's a directory, call this function recursively
        if (dir.isDirectory()) {
            listFilesRecursive(dir.fileName(), level + 1);
        }
    }
}

/**
   @name reportFiles
   @brief prints all files and directories present in the LitteFS flash file system
   @return none
*/
void reportFiles() {
  // deleteUnusedFiles();
  listFilesRecursive("/", 0);
}


/**
   @name reportCapabilities
   @brief prints the current board, VID and PID, free memory, or available functions (selectable via bitmask)
   @return none
*/
void reportCapabilities(uint16_t mask) {
  Serial.print(moduleName); Serial.print(" ");
  Serial.print(VERSION_STRING);

  if (mask & CAP_PRESSURESENSOR) {
    Serial.print(", PressureSensor=");
    switch (getPressureSensorType()) {
      case PRESSURE_NONE: Serial.print("None"); break;
      case PRESSURE_DPS310: Serial.print("DSP310"); break;
      case PRESSURE_MPRLS: Serial.print("MPRLS"); break;
      case PRESSURE_INTERNAL_ADC: Serial.print("InternalADC"); break;
    }
  }

  if (mask & CAP_FORCESENSOR) {
    Serial.print(", ForceSensor=");
    switch (getForceSensorType()) {
      case FORCE_NONE: Serial.print("None"); break;
      case FORCE_NAU7802: Serial.print("NAU7802"); break;
      case FORCE_INTERNAL_ADC: Serial.print("InternalADC"); break;
    }
  }

  if (mask & CAP_BOARD) {
    #ifdef ARDUINO_RASPBERRY_PI_PICO
      Serial.print(", Board=Raspberry_Pi_Pico");
    #elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
      Serial.print(", Board=Raspberry_Pi_Pico_W");
    #elif defined(ARDUINO_RASPBERRY_PI_PICO_2)
      Serial.print(", Board=Raspberry_Pi_Pico_2");
    #elif defined(ARDUINO_RASPBERRY_PI_PICO_2W)
      Serial.print(", Board=Raspberry_Pi_Pico_2W");
    #elif defined(ARDUINO_NANO_RP2040_CONNECT)
      Serial.print(", Board=ARDUINO_NANO_RP2040_CONNECT");
    #else
      Serial.print(", Board=UNKNOWN");
    #endif
  }

  // Get USB Device Descriptor
  const tusb_desc_device_t* desc = (const tusb_desc_device_t*) tud_descriptor_device_cb();
  if (mask & CAP_VID) {
    Serial.print(", VID=0x"); Serial.print(desc->idVendor, HEX);
  }

  if (mask & CAP_PID) {
    Serial.print(", PID=0x"); Serial.print(desc->idProduct, HEX);
  }

  if (!LittleFS.begin()) {
      return;
  }
  // Get filesystem information
  FSInfo fs_info;
  LittleFS.info(fs_info);

  if (mask & CAP_TOTALBYTES) {
    Serial.print(", TotalBytes="); Serial.print(fs_info.totalBytes);
  }
  if (mask & CAP_USEDBYTES) {
    Serial.print(", UsedBytes="); Serial.print(fs_info.usedBytes);
  }
  if (mask & CAP_FREEBYTES) {
    Serial.print(", FreeBytes="); Serial.print(fs_info.totalBytes - fs_info.usedBytes);
  }
  if (mask & CAP_MEMORYUSED) {
    Serial.print(", MemoryUsed=");  Serial.print((int)(fs_info.usedBytes * 100 / fs_info.totalBytes)); Serial.print("%");
  }

  Serial.println("");

    /*
    Serial.println("Detecting Board Type...");
    // Check RP2040 version (B0 or B1)
    uint32_t version = rp2040_chip_version();
    bool is_b1 = (version == 2);

    // Check for Wi-Fi chip
    bool has_wifi = (cyw43_arch_init() == 0);
    // init_cyw43_wifi(); // Initialize WiFi and check board type

    // Identify the board
    if (!is_b1 && !has_wifi) {
        Serial.println("Board: Raspberry Pi Pico");
    } else if (!is_b1 && has_wifi) {
        Serial.println("Board: Raspberry Pi Pico W");
    } else if (is_b1 && !has_wifi) {
        Serial.println("Board: Raspberry Pi Pico 2");
    } else if (is_b1 && has_wifi) {
        Serial.println("Board: Raspberry Pi Pico 2W");
    } else {
        Serial.println("Unknown Board");
    }
    */
}


