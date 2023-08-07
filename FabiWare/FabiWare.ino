
/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org
     https://github.com/asterics/FLipMouse

     Module: FLipWare.ino  (main module)

        This is the firmware for the FabiWare module, 
        it supports HID device emulation via USB and/or Bluetooth via connected sensors and/or serial AT-commands
        For a description of the supported commands see: commands.h

        HW-requirements:
                  Raspberry Pi PicoW
                  (optional) MPRLS I2C pressure sensor
                  up to 5 external switches connected to GPIO pins
                  Neopixel LED
                  1 TSOP 38kHz IR-receiver
                  1 high current IR-LED, driven with a MOSFET
                  SSD1306 display

        SW-requirements:
                  arduino-pico core (https://github.com/earlephilhower/arduino-pico), installable via board manager
                  Adafruit Neopixel library, installable via library manager
                  https://github.com/ChrisVeigl/LoadcellSensor
                  SSD1306Ascii-library by Bill Greiman, see https://github.com/greiman/SSD1306Ascii
                  Arduino settings: Tools->Board:"Raspberry Pi Pico W",  "Tools->Flash Size: "1792kB Sketch, 256kB FS", Tools->IP/Bluetooth Stack:"IPv4+Bluetooth"

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "FlipWare.h"
#include "gpio.h"      
#include "sensors.h"      
#include "infrared.h"      
#include "display.h"       // for SSD1306 I2C-Oled display
#include "modes.h"
#include "tone.h"
#include "parser.h"  
#include "reporting.h"
#include "keys.h"
#include <hardware/watchdog.h>


/**
   device name for ID string & BT-pairing
*/
char moduleName[]="FLipmouse";   

/**
   default values for empty configuration slot 
*/
const struct SlotSettings defaultSlotSettings = {      // default slotSettings valus, for type definition see Flipware.h
  "mouse",                          // initial slot name
  0,                                // initial keystringbuffer length
  400, 600, 3,                      // threshold sip, threshold puff, wheel step,
  800, 10,                          // threshold strong puff, threshold strong sip
  1,                                // bt-mode 1: USB, 2: Bluetooth, 3: both (2 & 3 need daughter board))
  0x0000FF,                         // default slot color: blue
  "en_US",                          // en_US as default keyboard layout.
};


/**
   static variables and data structures for settings and sensor data management
*/
struct SensorData sensorData {        
  .pressure=0, 
};

struct I2CSensorValues sensorValues {        
  .pressure=512, 
  .calib_now=CALIBRATION_PERIOD     // calibrate sensors after startup !
};


struct SlotSettings slotSettings;             // contains all slot settings
uint8_t workingmem[WORKINGMEM_SIZE];          // working memory (command parser, IR-rec/play)
uint8_t actSlot = 0;                          // number of current slot
unsigned long lastInteractionUpdate;          // timestamp for HID interaction updates


/**
   @name setup
   @brief setup function, program execution of core0 starts here
   @return none
*/
void setup() {

  // prepare synchronizsation of sensor data exchange between cores
  mutex_init(&(sensorValues.sensorDataMutex));

 //load slotSettings
  memcpy(&slotSettings,&defaultSlotSettings,sizeof(struct SlotSettings));

  // initialize peripherals
  Serial.begin(115200);
  
  #ifdef DEBUG_DELAY_STARTUP
    delay(3000);  // allow some time for serial interface to come up
  #endif
  
  MouseBLE.begin("FABI");
  KeyboardBLE.begin("");
  JoystickBLE.begin("");
  
  initGPIO();
  initIR();
  initButtons();
  initDebouncers();
  initStorage();   // initialize storage if necessary
  readFromEEPROMSlotNumber(0, true); // read slot from first EEPROM slot if available !

  // NOTE: changed for RP2040!  TBD: why does setBTName damage the console UART TX ??
  // setBTName(moduleName);             // if BT-module installed: set advertising name 

  setKeyboardLayout(slotSettings.kbdLayout); //load keyboard layout from slot
  
  // displayInstalled=displayInit(0);   // check if i2c-display connected   TBD: missing i2c core2 synchronisation!
  // displayUpdate();
  
#ifdef DEBUG_OUTPUT_FULL 
  Serial.print(moduleName); Serial.println(" ready !");
#endif
  lastInteractionUpdate = millis();  // get first timestamp

}

/**
   @name loop
   @brief loop function, periodically called from core0 after setup()
   @return none
*/
void loop() {
  // handle incoming serial data (AT-commands)
  while (Serial.available() > 0) {
    // send incoming bytes to parser
    parseByte (Serial.read());      // implemented in parser.cpp
  }

  // perform periodic updates  
  if (millis() >= lastInteractionUpdate + UPDATE_INTERVAL)  {
    lastInteractionUpdate = millis();

    // get current sensor data from core1
    mutex_enter_blocking(&(sensorValues.sensorDataMutex));
    sensorData.pressure=sensorValues.pressure;
    mutex_exit(&(sensorValues.sensorDataMutex));
    handleUserInteraction();                    // handle all mouse / joystick / button activities

    reportValues();   // send live data to serial
    updateLeds();     // mode indication via front facing neopixel LEDs
    updateBTConnectionState(); // check if BT is connected (for pairing indication LED animation)
    updateTones();    // mode indication via audio signals (buzzer)
  }
  delay(1);  // core0: sleep a bit ...  
}


//
//   Following code is running on second core (core1)
//

/**
   @name setup1
   @brief setup1 function, program execution of core1 starts here (for I2C sensor updates)
   @return none
*/
void setup1() {

  Wire1.begin();
  Wire1.setClock(400000);  // use 400kHz I2C clock
  initSensors();
  initBlink(10,20);  // first signs of life!
}

/**
   @name loop1
   @brief loop1 function, periodically called from core1 after setup1(), performs I2C sensor updates
   @return none
*/
void loop1() {
  static uint32_t lastMPRLS_ts=0;

  // if desired sampling period for MPRLS pressure sensor passed: get pressure sensor value
  if (millis()-lastMPRLS_ts >= 1000/MPRLS_SAMPLINGRATE) {
    lastMPRLS_ts=millis();
    readPressure(&sensorValues);
  }

  // if calibration running: update calibration counter
  if (sensorValues.calib_now) {
    sensorValues.calib_now--;  
    // calibrate sensors in the middle of the calibration period
    if(sensorValues.calib_now==CALIBRATION_PERIOD/2) {
      calibrateSensors();
    }     
  }


  // reset FlipMouse if sensors don't deliver data for several seconds (interface hangs?)
  if (!checkSensorWatchdog()) {
    //Serial.println("WATCHDOG !!");
    watchdog_reboot(0, 0, 10);
    while(1);
  }
  
  delay(1);  // core1: sleep a bit ...  
}
