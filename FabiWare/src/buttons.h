
/*
      FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: buttons.h - header file for the button handling

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/


#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include "gpio.h"

// Constants and Macro definitions
#define DEFAULT_DEBOUNCING_TIME 5   // debouncing interval for button-press / release

/**
   definition of Button numbers/indices of the "virtual buttons"
   (these are not pin numbers but button function identifiers!)
*/

  #define NUMBER_OF_BUTTONS     (NUMBER_OF_PHYSICAL_BUTTONS + 16)     // number of physical + virtual buttons. Note: if higher than 32, change buttonStates to uint64_t!
  #define UP_BUTTON               (NUMBER_OF_PHYSICAL_BUTTONS +0)
  #define DOWN_BUTTON             (NUMBER_OF_PHYSICAL_BUTTONS +1)
  #define LEFT_BUTTON             (NUMBER_OF_PHYSICAL_BUTTONS +2)
  #define RIGHT_BUTTON            (NUMBER_OF_PHYSICAL_BUTTONS +3)
  
  #define SIP_BUTTON              (NUMBER_OF_PHYSICAL_BUTTONS +4)
  #define STRONGSIP_BUTTON        (NUMBER_OF_PHYSICAL_BUTTONS +5)
  #define PUFF_BUTTON             (NUMBER_OF_PHYSICAL_BUTTONS +6)
  #define STRONGPUFF_BUTTON       (NUMBER_OF_PHYSICAL_BUTTONS +7)
  
  #define STRONGSIP_UP_BUTTON     (NUMBER_OF_PHYSICAL_BUTTONS +8)
  #define STRONGSIP_DOWN_BUTTON   (NUMBER_OF_PHYSICAL_BUTTONS +9)
  #define STRONGSIP_LEFT_BUTTON   (NUMBER_OF_PHYSICAL_BUTTONS +10)
  #define STRONGSIP_RIGHT_BUTTON  (NUMBER_OF_PHYSICAL_BUTTONS +11)
  
  #define STRONGPUFF_UP_BUTTON    (NUMBER_OF_PHYSICAL_BUTTONS +12)
  #define STRONGPUFF_DOWN_BUTTON  (NUMBER_OF_PHYSICAL_BUTTONS +13)
  #define STRONGPUFF_LEFT_BUTTON  (NUMBER_OF_PHYSICAL_BUTTONS +14)
  #define STRONGPUFF_RIGHT_BUTTON (NUMBER_OF_PHYSICAL_BUTTONS +15)

/**
   slotButtonSettings struct
   contains button mode identfier and value
   multiple of these constitute the button settings for the active slot
*/
struct slotButtonSettings {
  uint16_t mode;
  int16_t value;
};

/**
   buttonDebouncerType struct
   contains state and timing values for button press / release actions
   needed for debouncing, anti-tremor calculations and longpress detection
*/
struct buttonDebouncerType {
  uint8_t bounceCount;
  uint8_t bounceState;
  uint8_t stableState;
  uint8_t longPressed;
  uint32_t timestamp;
} ;

/**
   extern declarations of data structures 
   which can be accessed from different modules
*/
extern struct slotButtonSettings buttons[NUMBER_OF_BUTTONS];
extern char* buttonKeystrings[NUMBER_OF_BUTTONS];
extern uint32_t buttonStates;


/**
   @name initButtons
   @brief initialize button data structure and default values
   @return none
*/
void initButtons();


/**
   @name initButtonKeystrings
   @brief initialize/clear keystrings parameters of current slot
   @return none
*/
void initButtonKeystrings();

/**
   @name getButtonKeystring
   @brief get n-th keystring parameter of current slot
   @return char pointer to the keystring
*/
char * getButtonKeystring(int num);


/**
   @name getButtonKeystring
   @brief set n-th keystring parameter of current slot
   @param buttonIndex: number of button (index of keystring in keystring buffer)
   @param text: pointer to string which shall be copied to keystring buffer
   @return number of free bytes remaining in keystring buffer
*/
uint16_t setButtonKeystring(uint8_t buttonIndex, char const * text);

/**
   @name handlePress
   @brief handles a press detection of button n
   @param buttonIndex: number of button which was pressed
   @return none
*/
void handlePress (int buttonIndex);      // a button was pressed


/**
   @name handleRelease
   @brief handles a release detection of button n
   @param buttonIndex: number of button which was released
   @return none
*/
void handleRelease (int buttonIndex);    // a button was released

/**
   @name handleButton
   @brief process press and release actions for a particular button
   @param i: number of button to be processed
   @param state: current state (1:pressed, 0:released) raw value (without debouncing)
   @return none
*/
uint8_t handleButton(int i, uint8_t state);    // button debouncing and longpress detection


/**
   @name inHoldMode
   @brief returns if a button is currently in a state where an action remains held during press
   @param i: number of button to be checked
   @return true or false
*/
uint8_t inHoldMode (int i);

/**
   @name initDebounces
   @brief initialises / clears the debounciong data structures for all buttons
   @return none
*/
void initDebouncers();

#endif
