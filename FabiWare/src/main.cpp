
/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: main.cpp  (main module with setup and loop functions for both cores)

        This is the universal firmware for FABI, FlipMouse and FlipPad devices (HW version 3 or higher).
        see https://github.com/asterics/FABI, https://github.com/asterics/FlipMouse, https://github.com/asterics/FlipPad

        It supports HID device emulation via USB and/or Bluetooth via connected sensors and/or serial AT-commands
        For a description of the supported commands see: commands.h
        Pin assignments can be found in gpio.h

        HW-requirements:
          Supported Microcontroller platforms:
            Raspberry Pi Pico W or Pico 2 W (for FABI or FlipPad)
            Arduino Nano 2040 Connect (for FlipMouse) 
          Optional sensors and peripherals:
            I2C pressure sensor board (MPRLS or DPS310) or analog pressure sensor (e.g. MPX7007)
            I2C force sensor board (NAU7802-based) or analog 2d force sonsor (e.g. joystick module)
            up to 5 external switches connected to GPIO pins
            Neopixel LED
            1 TSOP 38kHz IR-receiver
            1 high current IR-LED, driven with a MOSFET
            SSD1306 display
            Piezo Buzzer and/or Analog Audio amplifier (FABI only) for acoustic feedback

        SW-requirements (as defined in platformio.ini):
          For building the SW, VSCode + PlatformIO is recommended, using the platform fork by Maximilian Gerhardt,
          see: https://arduino-pico.readthedocs.io/en/latest/platformio.html 
          Arduino-pico core by Earle Philhower, version: 4.5.0
          Settings for RP Pico Core: File System Size: 1MB, Bluetooth Stack enabled
          Adafruit BusIO library, version: 1.14.1
          Adafruit Neopixel library, version: 1.11.0
          SSD1306Ascii library by Bill Greiman, version: 1.3.5
          LoadcellSensor library by Chris Veigl: https://github.com/ChrisVeigl/LoadcellSensor
          NAU7802 library by Adafruit, adapted for 2 channels by Benjamin Aigner: https://github.com/benjaminaigner/NAU7802-DualChannel

        Note that for the FlipMouse (using the Arduino Nano2040 Connect), our custom Bluetooth firwmare is used for the onboard ESP32 and the board must be prepared as described here:
          https://github.com/asterics/FLipMouse/tree/master/Documentation/Fabrication/rp2040_preparation 

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


char moduleName[]=MODULE_NAME;   //   device name for ID string & BT-pairing

#ifdef FLIPMOUSE
  uint8_t addonUpgrade = BTMODULE_UPGRADE_IDLE; // if not "idle": we are upgrading the addon module
#endif 

/**
   default values for empty configuration slot 
*/
const struct SlotSettings defaultSlotSettings = {      // default slotSettings values, for type definition see Flipware.h
  "default",                        // initial slot name
  0,                                // initial keystringbuffer length
  1,                                // stickMode: Mouse cursor movement active
  40, 40, 20, 20, 50, 20,           // accx, accy, deadzone x, deadzone y, maxspeed, acceleration time
  0,                                // automatic dwelling time (0=disabled)   
  400, 600, 3,                      // threshold sip, threshold puff, wheel step,
  800, 10,                          // threshold strong puff, threshold strong sip
  0,                                // orientation
  1,                                // bt-mode 1: USB, 2: Bluetooth, 3: both (2 & 3 need BT support)
  2,                                // default sensorboard profile ID 2 (for FlipMouse sensorboard/NAU7802)
  0x0000FF,                         // default slot color: blue
  "en_US",                          // en_US as default keyboard layout.
};


/**
   static variables and data structures for settings and sensor data management
*/
struct GlobalSettings globalSettings {
  .buzzerMode=1,    // enable tone output via internal buzzer (tone height)
  .audioVolume=50,  // audio volume 50%  (if AUDIO_SIGNAL_PIN defined)
};

struct SensorData sensorData {        
  .x=0, .y=0, .xRaw=0, .yRaw=0, .pressure=0, 
  .deadZone=0, .force=0, .forceRaw=0, .angle=0,
  .dir=0,
  .autoMoveX=0, .autoMoveY=0,
  .mouseMoveTimestamp=0,
  .clickReleaseTimestamp=0,
  .xLocalMax=0, .yLocalMax=0,
  .currentBattPercent = -1, .MCPSTAT = MCPSTAT_HIGHZ,
  .usbConnected = false
};

struct SlotSettings slotSettings;             // contains all slot settings
uint8_t workingmem[WORKINGMEM_SIZE];          // working memory (command parser, IR-rec/play)
uint8_t actSlot = 0;                          // number of current slot

/*
  Handling I2C devices for check if something changed (will reset device, e.g. if external sensors are connected)
 */
uint8_t devicesWire[8] = {0};                 // active I2C devices on Wire, see supported_devices[]
uint8_t devicesWire1[8] = {0};                 // active I2C devices on Wire1, see supported_devices[]
void testI2Cdevices(TwoWire *interface, uint8_t *device_list);

/**
   @name setup
   @brief setup function, program execution of core0 starts here
   @return none
*/
void setup() {

  // prepare synchronization of sensor data exchange between cores
  mutex_init(&(currentSensorDataCore1.sensorDataMutex));
	
  // create a seperate alarm pool for audio and IR (the default one is already crowded, maybe by BT-stack?)
	// Note/Todo: figure out why the alarm pool fills up and needs to be this big (?)
  app_alarm_pool = alarm_pool_create(2, 64);  // using hw timer2, max. 64 alarm callback functions

  // turn on power suppy for peripherals
  enable3V3();
  delay(50);  // some time to stabilize the power supply

  #ifdef FABI
    initBattery(); // init GPIOs for battery management
  #endif

  #ifdef FLIPMOUSE
    // when using Arduino Nano 2040 Connect: initialise BT module  (must be done early!)
    initBluetooth();
  #else
    // when using RP Pico variants: set I2C pins for Wire0 (internal I2C for LC-Display / NFC)  
    Wire.setSDA(PIN_WIRE0_SDA_);
    Wire.setSCL(PIN_WIRE0_SCL_);
  #endif  

  Wire.begin();

  //check the I2C bus for active devices, add to array
  testI2Cdevices(&Wire,devicesWire);

  // initialize Serial interface
  Serial.begin(115200);
  #ifdef DEBUG_DELAY_STARTUP
    delay(3000);  // allow some time for serial interface to come up
  #endif

  // load default slot settings
  memcpy(&slotSettings,&defaultSlotSettings,sizeof(struct SlotSettings));

  // initialize other peripherals interface  
  initGPIO();
  initIR();
  initButtons();
  initDebouncers();
  initStorage();   // initialize storage if necessary
  initAudio();

  // read first configuration slot from storage if possible!
  readFromEEPROMSlotNumber(0, false); 

  #ifndef FLIPMOUSE
    MouseBLE.begin(moduleName);
    KeyboardBLE.begin("");
    #ifdef FABIJOYSTICK_ENABLED
      JoystickBLE.begin("");
    #endif
  #endif

  // NOTE: changed for RP2040!  TBD: why does setBTName damage the console UART TX ??
  // setBTName(moduleName);             // if BT-module installed: set advertising name 
  setKeyboardLayout(slotSettings.kbdLayout); //load keyboard layout from slot

  initDisplay();
  displayUpdate();
  
  makeTone(TONE_STARTUP,0);  // announce readyness!
  #ifdef DEBUG_OUTPUT_FULL 
    Serial.print(moduleName); Serial.println(" ready !");
  #endif
  audioPlayback(0);
  //lastInteractionUpdate = lastBatteryUpdate = millis();  // get first timestamp
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
      return;   // in upgrade mode, no other functions or commands are processed!
    }
    updateBTConnectionState(); // check if BT is connected (for pairing indication LED animation)
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
  NB_DELAY_START(interaction, UPDATE_INTERVAL)
    // get current sensor data from core1
    mutex_enter_blocking(&(currentSensorDataCore1.sensorDataMutex));
    sensorData.xRaw=currentSensorDataCore1.xRaw;
    sensorData.yRaw=currentSensorDataCore1.yRaw;
    sensorData.pressure=currentSensorDataCore1.pressure;
    mutex_exit(&(currentSensorDataCore1.sensorDataMutex));

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
    updateTones();    // mode indication via audio signals (buzzer)
  NB_DELAY_END //lastInteraction

  #ifdef FABI
    // every now and then: check battery and power status
    NB_DELAY_START(batteryUpdate,BATTERY_UPDATE_INTERVAL)
      performBatteryManagement();
      //Update Battery level for BLE, use 99% if not valid (-1)
      if(sensorData.currentBattPercent > 0) MouseBLE.setBattery(sensorData.currentBattPercent);
      else MouseBLE.setBattery(99);
    NB_DELAY_END

    // every 1s: check for changed I2C devices. TBD: for FABI only?
    NB_DELAY_START(i2cscan,1000)
      //create a copy of list before checking again
      uint8_t olddevices[8];
      memcpy(olddevices,devicesWire,8);      
      //check again
      testI2Cdevices(&Wire,devicesWire);
      //check if they match, if not -> restart
      if(memcmp(olddevices,devicesWire,8) != 0) {
        Serial.println("Devices on Wire changed -> restarting!");
        watchdog_reboot(0, 0, 10);
        while (1) { continue; }
      }
    NB_DELAY_END
  #endif

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
  delay(50);  // some time to stabilize the power supply
  
  // enable Wire1 I2C interface (used by Core1 for sensors)
  #ifndef FLIPMOUSE
    Wire1.setSDA(PIN_WIRE1_SDA_);
    Wire1.setSCL(PIN_WIRE1_SCL_);
  #endif
  Wire1.begin();
  Wire1.setClock(400000);  // use 400kHz I2C clock

  //check the I2C bus for active devices, add to array
  testI2Cdevices(&Wire1,devicesWire1);

  #ifdef DEBUG_DELAY_STARTUP
    delay(3000);  // allow some time for serial interface to come up
  #endif

  #ifdef DEBUG_ACTIVITY_LED
    pinMode(LED_BUILTIN,OUTPUT);
  #endif

  initSensors();
  if (getForceSensorType()==FORCE_NAU7802)
    setSensorBoard(slotSettings.sb); // apply sensorboard settings

  // initBlink(10,20);  // first signs of life!
}

/**
   @name loop1
   @brief loop1 function, periodically called from core1 after setup1(), performs sensor updates
   @return none
*/
void loop1() {

  // check if there is a message from the other core (sensorboard change, profile ID)
  if (rp2040.fifo.available()) {
      setSensorBoard(rp2040.fifo.pop());  
  }

  if (getForceSensorType() != FORCE_NONE)
     readForce();    

  if (getPressureSensorType() != PRESSURE_NONE)
     readPressure();    

  checkSensorCalibration();  // perform sensor calibration if necessary

  // reset if sensors don't deliver data for several seconds (interface hangs?)
  if (!checkSensorWatchdog()) {
    //Serial.println("WATCHDOG !!");
    watchdog_reboot(0, 0, 10);
    while(1);
  }
  
  #ifdef DEBUG_ACTIVITY_LED
    static int cnt=0;   
    if (!(cnt++%200)) digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  #endif

  // every 1s: check for changed I2C devices. TBD: for FABI only?
  NB_DELAY_START(i2cscan, 1000)
    //create a copy of list before checking again
    uint8_t olddevices[8];
    memcpy(olddevices, devicesWire1, 8);
    //check again
    testI2Cdevices(&Wire1, devicesWire1);
    //check if they match, if not -> restart
    if (memcmp(olddevices, devicesWire1, 8) != 0)
    {
      Serial.println("Devices on Wire1 changed -> restarting!");
      watchdog_reboot(0, 0, 10);
      while (1)
      {
        continue;
      }
    }
  NB_DELAY_END

  delay(1); // core1: sleep a bit ...  
}

void testI2Cdevices(TwoWire *interface, uint8_t *device_list) {
  int devicenr = 0;     //currently used I2C address index of supported_devices[]
  int devicecount = 0;  //count of found devices, used as offset in active devices (devicesWire[])

  //reset device list
  memset(device_list,0,8);

  while(supported_devices[devicenr] != 0x00) { //test until address is 0x00
    interface->beginTransmission(supported_devices[devicenr]);
    uint8_t result = interface->endTransmission();
    //if found: add to array of active devices
    if (result == 0) {
      //Serial.print("Found device @0x");
      //Serial.println(supported_devices[devicenr],HEX);
      device_list[devicecount] = supported_devices[devicenr];
      devicecount++;
    }
    //next address to be tested
    devicenr++;
  }
}
