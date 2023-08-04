
/*
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands
     More Information: https://github.com/asterics/FABI

     Module: FabiWare.ino - initialisation and main loop
     (for a list of supported AT-commands see commands.h)

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

     Necessary libraries:
     * Adafruit NeoPixel (from library manager)
     * Adafruit SSD1306 (from library manager)
*/


#include "fabi.h"
#include "eepromStorage.h"
#include "mouseControl.h"
#include "keys.h"
#include "buttons.h"
#include "display.h"
#include "NeoPixel.h"

#include <Wire.h>
#include <SPI.h>



#define PCB_checkPin 14     // Input Pin to be checked: Grounded == FABI PCB Version (with LCD / Buzzer / Neopixel)

int8_t  led_map[NUMBER_OF_LEDS] = {14, 15, 16};

const struct settingsType defaultSettings = {      // type definition see fabi.h
  "slot1", DEFAULT_WHEEL_STEPSIZE, DEFAULT_TRESHOLD_TIME,
  DEFAULT_SIP_THRESHOLD, DEFAULT_PUFF_THRESHOLD,
  DEFAULT_ANTITREMOR_PRESS, DEFAULT_ANTITREMOR_RELEASE, DEFAULT_ANTITREMOR_IDLE,
  DEFAULT_BT_MODE, DEFAULT_DOUBLEPRESS_TIME, DEFAULT_AUTODWELL_TIME, 0xFFFFFF
};
struct settingsType settings;

uint8_t PCBversion = 0;        // 1 == PCB version
uint32_t updateTimestamp = 0;
uint8_t actSlot = 0;           // the index of the currently active configuration slot
                               // note: this is changes in eeprom.cpp when a new slot is loaded
int waitTime = DEFAULT_WAIT_TIME;

void UpdateLeds();
int freeRam();


/**
   @name setup
   @param none
   @return none

   Program starts here. 
   Initialisation of HW and peripherals.
*/
void setup() {
  //load settings
  memcpy(&settings,&defaultSettings,sizeof(struct settingsType));
  Serial.begin(9600);    // open serial port for AT commands / GUI communication
  Serial1.begin(9600);   // open the serial port for BT-Module 
  delay(1000);           // allow some time for the BT-Module to start ...
  
  Mouse.begin();
  Keyboard.begin();
  
  //
  
  MouseBLE.begin("FABI");
  KeyboardBLE.begin();
  initDebouncers();
  
  // init peripherals 
  initDisplay();
  delay(100);
  initNeoPixel();
  initBuzzer();

  // initialise button pins and debouncers
  initButtons();

  // read button modes from first EEPROM slot (if available)
  bootstrapEEPROM();
  readFromEEPROM(0);

  writeSlot2Display();
  updateNeoPixelColor(1); 

  #ifdef DEBUG_OUTPUT
    Serial.println("Flexible Assistive Button Interface started !");
    Serial.print(F("Free RAM:"));  Serial.println(freeRam());
  #endif
}


/**
   @name loop
   @param none
   @return none

   main program loop, processes serial commands and button actions. 
*/
void loop() {
  // get and parse incoming bytes for Serial
  while (Serial.available() > 0) {
    int inByte = Serial.read();
    parseByte (inByte);      // implemented in parser.cpp
  }

  // update button states and perform periodic mouse updates
  if (millis() - updateTimestamp >= waitTime) {
    
    updateTimestamp = millis();
    updateButtons();
    updateMouse();

    //UpdateBuzzer();       // generate tones (indicating slot change)
    UpdateNeoPixel();     // update the brightness of the NeoPixel if slotchange occured
  }
}
