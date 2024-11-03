
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
                  (optional) I2C pressure sensor (MPRLS or DPS310)
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

#ifdef FLIPMOUSE
  char moduleName[]="FLipmouse";   //   device name for ID string & BT-pairing
  uint8_t addonUpgrade = BTMODULE_UPGRADE_IDLE; // if not "idle": we are upgrading the addon module
#endif 
#ifdef FABI
  char moduleName[]="FABI";
#endif


/**
   default values for empty configuration slot 
*/
const struct SlotSettings defaultSlotSettings = {      // default slotSettings valus, for type definition see Flipware.h
  "slot1",                          // initial slot name
  0,                                // initial keystringbuffer length
  1,                                // stickMode: Mouse cursor movement active
  40, 40, 20, 20, 50, 20,           // accx, accy, deadzone x, deadzone y, maxspeed, acceleration time
  400, 600, 3,                      // threshold sip, threshold puff, wheel step,
  800, 10,                          // threshold strong puff, threshold strong sip
  40, 20, 40, 20 ,                  // gain and range drift compenstation( vertical, horizontal)
  0,                                // orientation
  1,                                // bt-mode 1: USB, 2: Bluetooth, 3: both (2 & 3 need daughter board))
  2,                                // default sensorboard profile ID 2
  0x0000FF,                         // default slot color: blue
  "en_US",                          // en_US as default keyboard layout.
};


/**
   static variables and data structures for settings and sensor data management
*/
struct SensorData sensorData {        
  .x=0, .y=0, .xRaw=0, .yRaw=0, .pressure=0, 
  .deadZone=0, .force=0, .forceRaw=0, .angle=0,
  .dir=0,
  .autoMoveX=0, .autoMoveY=0,
  .xDriftComp=0, .yDriftComp=0,
  .xLocalMax=0, .yLocalMax=0
};

struct I2CSensorValues sensorValues {        
  .xRaw=0, .yRaw=0, .pressure=512, 
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

  #ifdef FLIPMOUSE
    //initialise BT module, if available (must be done early!)
    initBluetooth();
  #endif

  // initialize peripherals
  Serial.begin(115200);
  
  #ifdef DEBUG_DELAY_STARTUP
    delay(3000);  // allow some time for serial interface to come up
  #endif

  #ifdef FABI
    MouseBLE.begin("FABI");
    KeyboardBLE.begin("");
    JoystickBLE.begin("");
  #endif
  #ifdef FLIPMOUSE  
    rp2040.fifo.push_nb(slotSettings.sb); // apply sensorboard settings
  #endif
  
  initGPIO();
  initIR();
  initButtons();
  initDebouncers();
  initStorage();   // initialize storage if necessary
  readFromEEPROMSlotNumber(0, true); // read slot from first EEPROM slot if available !

  // NOTE: changed for RP2040!  TBD: why does setBTName damage the console UART TX ??
  // setBTName(moduleName);             // if BT-module installed: set advertising name 

  setKeyboardLayout(slotSettings.kbdLayout); //load keyboard layout from slot

  #ifdef FABI
    if(!displayInit(0)) {
      Serial.println("Error, cannot find display");   // check if i2c-display connected   TBD: missing i2c core2 synchronisation!
    }
    displayUpdate();
  #endif
  
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

  #ifdef FLIPMOUSE
    //check if we should go into addon upgrade mode
    if(addonUpgrade != BTMODULE_UPGRADE_IDLE) {
      performAddonUpgrade();
      return;
    }
    // if incoming data from BT-addOn: forward it to host serial interface
    while (Serial_AUX.available() > 0) {
      Serial.write(detectBTResponse(Serial_AUX.read()));
    }
  #endif
  
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
    sensorData.xRaw=sensorValues.xRaw;
    sensorData.yRaw=sensorValues.yRaw;
    sensorData.pressure=sensorValues.pressure;
    mutex_exit(&(sensorValues.sensorDataMutex));

    // apply rotation if needed
    switch (slotSettings.ro) {
      int32_t tmp;
      case 90: tmp=sensorData.xRaw;sensorData.xRaw=-sensorData.yRaw;sensorData.yRaw=tmp;
              break;
      case 180: sensorData.xRaw=-sensorData.xRaw;sensorData.yRaw=-sensorData.yRaw;
                break;
      case 270: tmp=sensorData.xRaw;sensorData.xRaw=sensorData.yRaw;sensorData.yRaw=-tmp;
                break;
    }

    calculateDirection(&sensorData);            // calculate angular direction / force form x/y sensor data
    applyDeadzone(&sensorData, &slotSettings);  // calculate updated x/y/force values according to deadzone
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
  #ifdef DEBUG_DELAY_STARTUP
    delay(3000);  // allow some time for serial interface to come up
  #endif
  #ifdef FABI
    Wire1.setSDA(PIN_WIRE1_SDA_);
    Wire1.setSCL(PIN_WIRE1_SCL_);
  #endif
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
  static uint32_t lastPressure_ts=0;

  #ifdef FLIPMOUSE  
    // check if there is a message from the other core (sensorboard change, profile ID)
    if (rp2040.fifo.available()) {
        setSensorBoard(rp2040.fifo.pop());  
    }
  
    // if the Data Ready Pin of NAU chip signals new data: get force sensor values
    if (digitalRead(DRDY_PIN) == HIGH)  { 
      readForce(&sensorValues);    
    }
  #endif

  // if desired sampling period for MPRLS pressure sensor passed: get pressure sensor value
  if (millis()-lastPressure_ts >= 1000/PRESSURE_SAMPLINGRATE) {
    lastPressure_ts=millis();
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


  // reset if sensors don't deliver data for several seconds (interface hangs?)
  if (!checkSensorWatchdog()) {
    //Serial.println("WATCHDOG !!");
    watchdog_reboot(0, 0, 10);
    while(1);
  }
  
  delay(1);  // core1: sleep a bit ...  
}
