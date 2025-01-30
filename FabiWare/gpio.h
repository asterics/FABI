/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: gpio.h - functions for leds and buttons

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _GPIO_H_
#define _GPIO_H_

#include "FlipWare.h"

/**
   constant definitions for pin assignments
*/
#ifdef FLIPMOUSE
  #define NUMBER_OF_PHYSICAL_BUTTONS 3      // number of physical buttons for FlipMouse
  #define PHYSICAL_BUTTON_MAP {17, 28, 20}  // gpio pin assignment for buttons
  #define NEOPIXEL_PIN   15   // physical pin for neopixel LED
  #define TONE_PIN       25   // physical pin for piezo buzzer
  #define IR_LED_PIN     19   //  IR-Led output pin for FlipMouse
  #define IR_SENSOR_PIN  16   //  input pin of the TSOP IR receiver
#else 
  #define NUMBER_OF_PHYSICAL_BUTTONS 5      // number of physical switches for FABI/FlipPad
  #define PHYSICAL_BUTTON_MAP {11,9,8,4,3}
  #define NEOPIXEL_PIN   10
  #define TONE_PIN        2    
  #define IR_LED_PIN     14
  #define IR_SENSOR_PIN  16
 
  // remap pins for I2C interfaces for FABI / FlipPad
  #define PIN_WIRE0_SDA_ 12
  #define PIN_WIRE0_SCL_ 13
  #define PIN_WIRE1_SDA_ 26
  #define PIN_WIRE1_SCL_ 27
#endif



/**
   extern declaration of static variables
   which shall be accessed from other modules
*/
extern int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS];  // maps the button number to physical pin

/**
   @name initGPIO
   @brief initializes data direction of gpio pins
   @return none
*/
void initGPIO();

/**
   @name initBlink
   @brief initializes an LED blinking sequence
   @return none
*/
void initBlink(uint8_t count, uint8_t startTime);


/**
   @name updateLeds
   @brief update leds according to current slot number / blinking actions
   @return none
*/
void updateLeds();

/**
   @name setLeds
   @brief set the LEDs (or the Neopixel directly)
   @return none
   @param LEDs (bits 0-3; Neopixel equals bit 0 R; bit 1 G; bit 2 B)
   @note Only used in CIM mode.
*/
void setLeds(uint8_t leds);

#endif
