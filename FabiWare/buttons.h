
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

// Constants and Macro definitions
#define NUMBER_OF_BUTTONS  18         // number of physical + virtual switches. Note: if higher than 32, change buttonStates to uint64_t!

#define DEFAULT_DEBOUNCING_TIME 5   // debouncing interval for button-press / release

 
// (buttons 0-2 are the physical switches on the device)
/**
   definition of Button numbers/indices of the "virtual buttons"
   (these are not pin numbers but button function identifiers!)
*/
#define BUTTON_1                0
#define BUTTON_2                1
#define BUTTON_3                2
#define BUTTON_4                3
#define BUTTON_5                4

#define SIP_BUTTON               6
#define STRONGSIP_BUTTON         7
#define PUFF_BUTTON              8
#define STRONGPUFF_BUTTON        9

#define STRONGSIP_2_BUTTON     	10
#define STRONGSIP_3_BUTTON   	11
#define STRONGSIP_4_BUTTON   	12
#define STRONGSIP_5_BUTTON  	13

#define STRONGPUFF_2_BUTTON    	14
#define STRONGPUFF_3_BUTTON  	15
#define STRONGPUFF_4_BUTTON  	16
#define STRONGPUFF_5_BUTTON 	17


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
