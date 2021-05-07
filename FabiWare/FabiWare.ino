
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
       https://github.com/cpldcpu/light_ws2812
       https://github.com/lexus2k/ssd1306

*/

#include "fabi.h"
#include "eeprom.h"
#include "keys.h"
#include "buttons.h"
#include "mouse.h"
#include "display.h"
#include "NeoPixel.h"


#define PCB_checkPin 14     // Input Pin to be checked: Grounded == FABI PCB Version (with LCD / Buzzer / Neopixel)

int8_t  led_map[NUMBER_OF_LEDS] = {14, 15, 16};

struct settingsType settings = {      // type definition see fabi.h
  "slot1", DEFAULT_WHEEL_STEPSIZE, DEFAULT_TRESHOLD_TIME,
  DEFAULT_SIP_THRESHOLD, DEFAULT_PUFF_THRESHOLD,
  DEFAULT_ANTITREMOR_PRESS, DEFAULT_ANTITREMOR_RELEASE, DEFAULT_ANTITREMOR_IDLE,
  DEFAULT_BT_MODE, DEFAULT_DOUBLEPRESS_TIME, DEFAULT_AUTODWELL_TIME, 0xFFFFFF
};

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
  Serial.begin(9600);
  Mouse.begin();
  Keyboard.begin();
  initDebouncers();
  
  //check if PCB or old (floating wire) FABI is used (checkPin to ground = PCB):
  pinMode(PCB_checkPin, INPUT_PULLUP);

  if (!digitalRead(PCB_checkPin)) {            //PCB Version detected
    PCBversion = 1;
    #ifdef DEBUG_OUTPUT
        Serial.println("FABI PCB Version");
    #endif

    // turn off built-in LEDs
    pinMode(LED_BUILTIN_RX, INPUT);
    pinMode(LED_BUILTIN_TX, INPUT);

    // start HW-Serial (used for communication with BT-AddOn)
    Serial1.begin(9600);

    // init peripherals 
    initDisplay();
    delay(100);
    initNeoPixel();
    initBuzzer();
  }
  else {    // no PCB Version:
    TXLED1;    //turn on TX_LED

    // configure the leds for slot indication as output
    for (int i = 0; i < NUMBER_OF_LEDS; i++)
      pinMode (led_map[i], OUTPUT);  
  }

  // initialise button pins and debouncers
  initButtons();

  // read button modes from first EEPROM slot (if available)
  readFromEEPROM(0);

  // initialise peripheral HW for PCB version
  if (PCBversion) {
    initBluetooth();      //initialise BT module, if available
    writeSlot2Display();
    updateNeoPixelColor(1); 
  }

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

  while (Serial.available() > 0) {
    // get incoming bytes for Serial
    int inByte = Serial.read();
    parseByte (inByte);      // implemented in parser.cpp
  }

  // if incoming data from BT-addOn: forward it to host serial interface
  while (Serial1.available() > 0) {
    Serial.write(Serial1.read());
  }

  // update button states and perform periodic mouse updates
  if (millis() - updateTimestamp >= waitTime) {
    
    updateTimestamp = millis();
    updateButtons();
    updateMouse();

    if (PCBversion) {
      UpdateBuzzer();       // generate tones (indicating slot change)
      UpdateNeoPixel();     // update the brightness of the NeoPixel if slotchange occured
    }
    else UpdateLeds();      // update slot indication leds in case no PCB version
  }
}

/**
   @name UpdateLeds
   @param none
   @return none

   indicates active configuration slot via 3 external LEDs.
   Note that the LEDs must be wired as "active low".
   (only for non-PCB version)
*/
void UpdateLeds()
{
  if (!PCBversion) {
    if (actSlot == 1) digitalWrite (led_map[0], LOW); else digitalWrite (led_map[0], HIGH);
    if (actSlot == 2) digitalWrite (led_map[1], LOW); else digitalWrite (led_map[1], HIGH);
    if (actSlot == 3) digitalWrite (led_map[2], LOW); else digitalWrite (led_map[2], HIGH);
  }
}

/**
   @name freeRam
   @param none
   @return none

   returns currently free RAM bytes.
*/
int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
