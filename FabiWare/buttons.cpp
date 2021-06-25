
/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: buttons.cpp - button mapping and press/release handling
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "fabi.h"
#include "keys.h"
#include "buttons.h"

int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS_NOPCB] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
int8_t  input_map_PCB[NUMBER_OF_PHYSICAL_BUTTONS_PCB] = {10, 16, 19, 5, 6, 7, 8, 9};

struct buttonType buttons [NUMBER_OF_BUTTONS];                     // array for all buttons - type definition see fabi.h
struct buttonDebouncerType buttonDebouncers [NUMBER_OF_BUTTONS];   // array for all buttonsDebouncers - type definition see fabi.h
char   keystringBuffer[KEYSTRING_BUFFER_LEN]; // buffer for all string parameters for the buttons of a slot

uint8_t NUMBER_OF_PHYSICAL_BUTTONS;
uint16_t buttonStates = 0;
uint16_t pressure = 0;
uint8_t reportRawValues = 0;
uint8_t reportSlotParameters = 0;
uint8_t valueReportCount = 0;

/**
   @name initButtons
   @param none
   @return none

   initialise button pins and default modes
*/
void initButtons() {
  
  // update pin mapping for PCB version 
  if (PCBversion)  {
    NUMBER_OF_PHYSICAL_BUTTONS = NUMBER_OF_PHYSICAL_BUTTONS_PCB;
    memcpy(input_map, input_map_PCB, NUMBER_OF_PHYSICAL_BUTTONS);
  }
  else NUMBER_OF_PHYSICAL_BUTTONS = NUMBER_OF_PHYSICAL_BUTTONS_NOPCB;

  for (int i = 0; i < NUMBER_OF_PHYSICAL_BUTTONS; i++) // initialize physical buttons and bouncers
    pinMode (input_map[i], INPUT_PULLUP);   // configure the pins for input mode with pullup resistors

  // initialize button array
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)  { 
    buttons[i].mode = CMD_HL;   // set default command for every button to "left mouse click"
    buttons[i].value = 0;
    keystringBuffer[i] = 0;
  }
}

/**
   @name updateButtons
   @param none
   @return none

   update buttons states and call handling functions
   handle sip/puff pressure sensor values
   perform live value reports
*/
void updateButtons() {
  pressure = analogRead(PRESSURE_SENSOR_PIN);

  // update button press / release events
  for (int i = 0; i < NUMBER_OF_PHYSICAL_BUTTONS; i++)
    handleButton(i, i + 6, digitalRead(input_map[i]) == LOW ? BUTTON_PRESSED : BUTTON_RELEASED);

  // handle pressure sensor and perform sip/puff actions if enabled
  if (settings.ts > 0)    handleButton(SIP_BUTTON, -1, pressure < settings.ts ? BUTTON_PRESSED : BUTTON_RELEASED);
  if (settings.tp < 1023) handleButton(PUFF_BUTTON, -1, pressure > settings.tp ? BUTTON_PRESSED : BUTTON_RELEASED);

  // if value report is active: send live values over serial
  if (reportRawValues)   {
    if (valueReportCount++ > 10) {      // report raw values !
      Serial.print("VALUES:"); Serial.print(pressure); Serial.print(",");
      for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++)
      {
        if (buttonStates & (1 << i)) Serial.print("1");
        else Serial.print("0");
      }
      Serial.print(",");
      Serial.print(actSlot-1);
      Serial.println("");
      valueReportCount = 0;
    }
  }   
}


/**
   @name handlePress
   @param int buttonIndex represents the pressed physical (or virtual) button 
   @return none

   calls action commands according to button mode
   takes care for double press actions / slot change 
*/
void handlePress (int buttonIndex)
{
  static uint32_t doublePressTimestamp = 0;
  #ifdef DEBUG_OUTPUT
    Serial.print("press button "); Serial.print(buttonIndex);
  #endif

  // check if double press condition is met
  if (settings.dp > 0) { 
    if ((millis() - doublePressTimestamp) < settings.dp) {
      // Serial.println("skip to next Slot!");
      performCommand(CMD_NE, 0, 0, 0); // activate next slot
      while (digitalRead(input_map[buttonIndex]) ==LOW) ;  // wait until button is released
      return;
    }
  }
  buttonStates |= (1 << buttonIndex);  // save for reporting
  doublePressTimestamp = millis();     // remember timestamp in order to detect double presses

  // perform the associated button action 
  performCommand(buttons[buttonIndex].mode, buttons[buttonIndex].value, getKeystring(buttonIndex), 1);
}



/**
   @name handleRelease
   @param int buttonIndex represents the pressed physical (or virtual) button 
   @return none

   stops (sticky / hold) commands according to button mode
   when a button is released 
*/
void handleRelease (int buttonIndex)
{
  #ifdef DEBUG_OUTPUT
    Serial.print("release button "); Serial.print(buttonIndex);
  #endif
  buttonStates &= ~(1 << buttonIndex); //save for reporting
  switch (buttons[buttonIndex].mode) {
    case CMD_HL: leftMouseButton = 0; break;
    case CMD_HR: rightMouseButton = 0; break;
    case CMD_HM: middleMouseButton = 0; break;
    case CMD_MX: moveX = 0; break;
    case CMD_MY: moveY = 0; break;
    case CMD_KH: releaseSingleKeys(getKeystring(buttonIndex)); break;
  }
}


/**
   @name initDebouncers
   @param none 
   @return none

   initialises the button debouncing structures / states  
*/
void initDebouncers()
{
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) // initialize button array
  {
    buttonDebouncers[i].pressCount = 0;
    buttonDebouncers[i].releaseCount = 0;
    buttonDebouncers[i].idleCount = 0;
    buttonDebouncers[i].pressState = BUTTONSTATE_NOT_PRESSED;
  }
}


/**
   @name handleButton
   @param int i index of the buttonaction for press 
   @param int l index of the buttonaction for long press 
   @param uint8_t actstate (e.g. BUTTON_PRESSED, see buttons.h)
   @return none

   performs the button debouncing and performs longpress detection
   calls press / long press actions 
   (if button i is pressed long and index l>=0, virtual button l is activated !)
*/
void handleButton(int i, int l, uint8_t actState)
{
  // antitremor idle count check:
  // keeps the button inactive a minimum time (settings.ai) after it was pressed
  if (buttonDebouncers[i].pressState == BUTTONSTATE_IDLE) {
    buttonDebouncers[i].idleCount++;
    if (buttonDebouncers[i].idleCount >= settings.ai) {
      buttonDebouncers[i].idleCount = 0;
      buttonDebouncers[i].pressCount = 0;
      buttonDebouncers[i].releaseCount = 0;
      buttonDebouncers[i].pressState = BUTTONSTATE_NOT_PRESSED;
    }
    return;
  }

  // handle button press detection
  if ((actState == BUTTON_PRESSED))  {
    buttonDebouncers[i].releaseCount = 0;
    if ((buttonDebouncers[i].pressCount <= settings.tt >> 2) || (settings.tt == 0))
      buttonDebouncers[i].pressCount++;

    // antitremor press check: hold button a minimum time before press is valid (settings.ap)  
    if (buttonDebouncers[i].pressCount == settings.ap) {
      handlePress(i);   // issue the short press action !
      buttonDebouncers[i].pressState = BUTTONSTATE_SHORT_PRESSED;
    }

    // check for long press action 
    if ((buttonDebouncers[i].pressCount == settings.tt >> 2) && (settings.tt != 0)  
         && (l >= 0) && (l < NUMBER_OF_BUTTONS) && (buttons[l].mode != CMD_NC)) {

      // in case pressed duration reaches threshold  settings.tt/4:
      // release short press and issue long press function!
      handleRelease(i);
      handlePress(l);
      buttonDebouncers[i].pressCount = settings.tt >> 2;  // update pressCount in case slot has changed!
                                                          //(this avoids glitches if settings.tt differs)
      buttonDebouncers[i].pressState = BUTTONSTATE_LONG_PRESSED;
    }
  }

  // handle button release detection
  if ((actState == BUTTON_RELEASED)) {
    buttonDebouncers[i].pressCount = 0;

    // antitremor release count check:
    // release the button only if it stays released for a minimum time (settings.ar)
    if (buttonDebouncers[i].releaseCount <= settings.ar)
      buttonDebouncers[i].releaseCount++;
    if (buttonDebouncers[i].releaseCount == settings.ar) {
      if (buttonDebouncers[i].pressState == BUTTONSTATE_SHORT_PRESSED) {
        handleRelease(i);   // stop ongoing press action
        buttonDebouncers[i].pressState = BUTTONSTATE_IDLE;
      }
      else if (buttonDebouncers[i].pressState == BUTTONSTATE_LONG_PRESSED) {
        handleRelease(l);  // stop ongoing longpress action
        buttonDebouncers[i].pressState = BUTTONSTATE_IDLE;
      }
    }
  }
}

/**
   @name getKeystring
   @param uint8_t button  the button index
   @return char *  the keystring

   returns the key parameter string for a given button index
*/
char * getKeystring (uint8_t button)
{
  char *s = keystringBuffer;

  // bypass first n strings in the keystrinBuffer array
  for (int i = 0; i < button; i++) {
    while (*s) s++;
    s++;
  }
  return (s);
}

/**
   @name keystringMemUsage
   @param uint8_t button  the button index
   @return uint16_t memory usage (length)

   returns the length of all key parameter strings, starting with index "button"
   keystringMemUsage(0) returns the lenght of all currently stored keystings in the keystringBuffer array
*/
uint16_t keystringMemUsage(uint8_t button)
{
  uint16_t sum = 0;
  for (int i = button; i < NUMBER_OF_BUTTONS; i++)
    sum += strlen(getKeystring(i)) + 1;
  return (sum);
}

/**
   @name setKeystring
   @param uint8_t button  the button index
   @param char * text  the text to be stored
   @return none

   stores a new string/ASCII-text (for the given button) into the keyStringBuffer array
   all individual strings in the array are zero-terminated
*/
void setKeystring (uint8_t button, char * text)
{

  // check if new string fits into memory, cancel if not!
  if (keystringMemUsage(0) - strlen(getKeystring(button)) + strlen(text) >= KEYSTRING_BUFFER_LEN)
    return;

  if (button < NUMBER_OF_BUTTONS - 1) {
    uint16_t bytesToCopy = keystringMemUsage(button + 1);
    int16_t delta = strlen(text) - strlen(getKeystring(button));

    // move other strings in order to fit in the new one !
    memmove(getKeystring(button + 1) + delta, getKeystring(button + 1), bytesToCopy);
  }
  strcpy(getKeystring(button), text);
}
