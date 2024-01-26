/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: keys.cpp - implementation of the keyboard handling

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "FlipWare.h"
#include "keys.h"


/**
   forward declarations of module-internal functions
*/
uint8_t pressed_keys[KEYPRESS_BUFFERSIZE];
uint8_t in_keybuffer(uint16_t key);
void remove_from_keybuffer(uint16_t key);
void add_to_keybuffer(uint16_t key);
void performKeyActions(char* text, uint8_t keyAction);
char kbdLayout[6] = "en_US";
//const uint8_t *kbdLayoutArray = KeyboardLayout_en_US;
const uint8_t *kbdLayoutArray;

/**
   @name printKeyboardLayout
   @brief Prints out the currently used keyboard layout (e.g. "en_US\n")
   @return none
*/
void printKeyboardLayout()
{
	Serial.println(kbdLayout);
}

/**
   @name setKeyboardLayout
   @brief Updates the currently used keyboard layout.
   @param name Name of the new keyboard layout (e.g. "en_US" or "de_DE")
   @return 1 on success, 0 if layout is not found.
   @note Currently supported: de_DE, en_US, es_ES, fr_FR, it_IT, sv_SE, da_DK
*/
int8_t setKeyboardLayout(char *name)
{
	const uint8_t *newLayout = 0;
	
	/*if(strncmp(name, "de_DE",5) == 0) newLayout = KeyboardLayout_de_DE;
	if(strncmp(name, "en_US",5) == 0) newLayout = KeyboardLayout_en_US;
	if(strncmp(name, "es_ES",5) == 0) newLayout = KeyboardLayout_es_ES;
	if(strncmp(name, "fr_FR",5) == 0) newLayout = KeyboardLayout_fr_FR;
	if(strncmp(name, "it_IT",5) == 0) newLayout = KeyboardLayout_it_IT;
	if(strncmp(name, "sv_SE",5) == 0) newLayout = KeyboardLayout_sv_SE;
	if(strncmp(name, "da_DK",5) == 0) newLayout = KeyboardLayout_da_DK;*/
	
	if(newLayout)
	{
		//Keyboard.begin(newLayout);
    #warning "TODO: tinyUSB layouts not available, if using would include Keyboard.h"
    kbdLayoutArray = newLayout;
		#ifdef DEBUG_OUTPUT_FULL
			Serial.print("Found new layout pointer for ");
			Serial.print(name);
			Serial.println(", setting in Keyboard.begin");
		#endif
    strncpy(kbdLayout,name,5); //save locally
		return 1;
	} else return 0;
}

/**
   @name getKeyboardLayout
   @brief Used to get the pointer to the current keyboard layout
   @return Pointer to keyboard layout array
*/
const uint8_t *getKeyboardLayout()
{
  return kbdLayoutArray;
}

/**
   @name updateKey
   @brief performs a new press/release action for a given keycode
   @param key the keycode
   @param keyAction action to be performed
   @return none
*/
void updateKey(int key, uint8_t keyAction)
{
  switch (keyAction)  {
    case KEY_PRESS:
    #ifdef DEBUG_OUTPUT_KEYS
      Serial.print("P+");
    #endif
    case KEY_HOLD:
      #ifdef DEBUG_OUTPUT_KEYS
        Serial.println("H");
      #endif
      add_to_keybuffer(key);
      sendKeyboard(pressed_keys);
      break;

    case KEY_RELEASE:
      #ifdef DEBUG_OUTPUT_KEYS
        Serial.println("R");
      #endif
      remove_from_keybuffer(key);
      sendKeyboard(pressed_keys);
      break;

    case KEY_TOGGLE:
      #ifdef DEBUG_OUTPUT_KEYS
        Serial.print("T-");
      #endif
      if (in_keybuffer(key))  {
        #ifdef DEBUG_OUTPUT_KEYS
          Serial.println("R");
        #endif
        remove_from_keybuffer(key);
        sendKeyboard(pressed_keys);
      } else {
        #ifdef DEBUG_OUTPUT_KEYS
          Serial.println("P");
        #endif
        add_to_keybuffer (key);
        sendKeyboard(pressed_keys);
      }
      break;
  }
  //need to delay to avoid missing keyboard actions
  delay(10);
}

void pressKeys (char * text)
{
  performKeyActions(text, KEY_PRESS);
  performKeyActions(text, KEY_RELEASE);
}

void printKeys(char * keystring)
{
  //Keyboard.print(keystring);  // improved for ISO 8859 compatibility (but: slower ..)
  for (unsigned int i = 0; i < strlen(keystring); i++)
  {
      /*if (slotSettings.bt & 1) Keyboard.write(keystring[i]);*/
      //if (slotSettings.bt & 2) KeyboardBLE.write(keystring[i]);
  }
}

void holdKeys (char * text)
{
  performKeyActions(text, KEY_HOLD);
}

void releaseKeys (char * text)
{
  performKeyActions(text, KEY_RELEASE);
}

void toggleKeys (char * text)
{
  performKeyActions(text, KEY_TOGGLE);
}

void release_all_keys()
{
  for (int i = 0; i < KEYPRESS_BUFFERSIZE; i++) pressed_keys[i] = 0;
  sendKeyboard(pressed_keys);
}


void release_all()  // releases all previously pressed keys and stop mouse actions
{
  release_all_keys();
  mouseRelease(MOUSE_LEFT);
  mouseRelease(MOUSE_MIDDLE);
  mouseRelease(MOUSE_RIGHT);
}

/**
   @name add_to_keybuffer
   @brief adds a keycode to buffer of pressed keys
   @param key the keycode
   @return none
*/
void add_to_keybuffer(uint16_t key)
{
  //check if it is a modifier
  if((key & 0xE000) == 0xE000) {
    pressed_keys[0] |= (key & 0x00FF); //set bit 0 - 7, depending on modifier.
    return;
  }
  //place other keycodes into upper 6 bytes of key array
  for (int i = 2; i < KEYPRESS_BUFFERSIZE; i++)
  {
    if (pressed_keys[i] == 0) pressed_keys[i] = key;
    if (pressed_keys[i] == key) break;
  }
}

/**
   @name remove_from_keybuffer
   @brief removes a keycode from buffer of pressed keys
   @param key the keycode
   @return none
*/
void remove_from_keybuffer(uint16_t key)
{
  //check if it is a modifier
  if((key & 0xE000) == 0xE000) {
    pressed_keys[0] &= ~(key & 0x00FF); //clear bit 0 - 7, depending on modifier.
    return;
  }
  
  //remove other keycodes from upper 6 bytes of key array
  for (int i = 2; i < KEYPRESS_BUFFERSIZE; i++)
  {
    //is this possible slot containing the keycode?
    if(pressed_keys[i] == key)
    {
      //if yes move all trailing keycodes by one position forward, overwriting old keycode
      while (i < KEYPRESS_BUFFERSIZE - 1)
      {
        pressed_keys[i] = pressed_keys[i + 1];
        i++;
      }
      //set last byte to 0
      pressed_keys[KEYPRESS_BUFFERSIZE - 1] = 0;
      break;
    }
  }
}

/**
   @name in_keybuffer
   @brief returns true if a given keycode is in the buffer of pressed keys
   @param key the keycode
   @return true if key is actually pressed, otherwise false
*/
uint8_t in_keybuffer(uint16_t key)
{
  //check if it is a modifier
  if((key & 0xE000) == 0xE000) {
    pressed_keys[0] |= (key & 0x00FF); //set bit 0 - 7, depending on modifier.
    return (pressed_keys[0] & (key & 0x00FF)) ? 1 : 0;
  }
  
  for (int i = 2; i < KEYPRESS_BUFFERSIZE; i++)
  {
    if (pressed_keys[i] == key)
      return (1);
  }
  return (0);
}


/**
   @name performKeyActions
   @brief press, release or hold multiple keys
   @param text is a string which contains the key identifiers eg. "KEY_CTRL KEY_C" for Ctrl-C
   @param keyAction the action will shall be performed
   @return none
*/
void performKeyActions(char* text,  uint8_t keyAction)
{
  //allocate new memory for working with this keystring.
  //limit to MAX_KEYSTRING_LEN
  char * tmptxt = (char *) malloc( sizeof(char) * ( strnlen(text,MAX_KEYSTRING_LEN) + 2 ) ); // for parsing keystrings
  char * acttoken;
  bool found = false;

  strncpy(tmptxt, text,MAX_KEYSTRING_LEN);
  if (tmptxt[strlen(tmptxt) - 1] != ' ') strcat(tmptxt, " ");

  acttoken = strtok(tmptxt, " ");
  while (acttoken)
  {
    if (!strncmp(acttoken, "KEY_", 4)) {
      acttoken += 4;
      uint16_t key = parseIdentifierToKeycode(acttoken);
      if(key) updateKey(key, keyAction);
    }

    //if using a KEYPAD key, search in second array
    if (!strncmp(acttoken, "KEYPAD_", 7)) {
      acttoken += 7;
      uint16_t key = parseIdentifierToKeycode(acttoken);
      if(key) updateKey(key, keyAction);
    }
    acttoken = strtok(NULL, " ");
  }
  free(tmptxt);
}
