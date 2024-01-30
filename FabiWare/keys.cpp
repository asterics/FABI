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
/**
 * from keycodes.h: keyboard_layouts
 * */
uint8_t kbdLocale = 0;

/**
   @name printKeyboardLayout
   @brief Prints out the currently used keyboard locale. (see keycodes.h)
   @return none
*/
void printKeyboardLayout()
{
	Serial.println(kbdLocale);
}

/**
   @name setKeyboardLayout
   @brief Updates the currently used keyboard layout.
   @param locale Number of the new keyboard layout (see keycodes.h)
   @return 0 on success, 1 if layout is not found.
*/
int8_t setKeyboardLayout(uint8_t locale)
{
  if(locale >= LAYOUT_MAX) return 0;
  else kbdLocale = locale;
  return 1;
}

/**
   @name getKeyboardLayout
   @brief Used to get the current keyboard locale
   @return Locale nr.
*/
uint8_t getKeyboardLayout()
{
  return kbdLocale;
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
        Serial.print("H-");
        Serial.println(key);
      #endif
      add_to_keybuffer(key);
      sendKeyboard(pressed_keys);
      break;

    case KEY_RELEASE:
      #ifdef DEBUG_OUTPUT_KEYS
        Serial.print("R-");
        Serial.println(key);
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
          Serial.print("R-");
          Serial.println(key);
        #endif
        remove_from_keybuffer(key);
        sendKeyboard(pressed_keys);
      } else {
        #ifdef DEBUG_OUTPUT_KEYS
          Serial.print("P-");
          Serial.println(key);
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
  size_t len = 0;
  uint8_t modifier = 0;
  uint8_t deadkey = 0;
  
  //we clear the full buffer for printing a string...
  release_all_keys();
  
  //as long as !=0 bytes are available & not exceeding buffers
  while(len < MAX_KEYSTRING_LEN && keystring[len] != 0)
  {
    #ifdef DEBUG_OUTPUT_KEYS
      Serial.println(keystring[len]);
    #endif
    //parse with ultra heavy mega boosted parser, which gives us:
    // - a keycode (kc) != 0 if it is found
    // - a modifier byte which is used directly in the report
    // - a possible necessary deadkey which is pressed before.
    modifier = 0;
    deadkey = 0;
    uint8_t kc = parse_for_keycode(keystring[len],kbdLocale,&modifier,&deadkey);
    #warning "TODO: still a bug here: first key press is not recognized"
    //check if successful, or maybe need next byte
    if(kc) {
      //if keycode is found, first send the deadkey (todo: I think without modifier.)
      if(deadkey) {
        #ifdef DEBUG_OUTPUT_KEYS
          Serial.print("deadkey:");
          Serial.println(deadkey);
        #endif
        pressed_keys[2] = deadkey;
        sendKeyboard(pressed_keys); delay(10);
        pressed_keys[2] = 0;
        sendKeyboard(pressed_keys); delay(10);
        delay(2);
      }
      #ifdef DEBUG_OUTPUT_KEYS
        Serial.print("keycode:");
        Serial.print(kc);
        Serial.print(",modifier:");
        Serial.println(modifier);
      #endif
      //if necessary, deadkey is pressed:
      // now to the real keycode + modifier.
      #warning "TODO: still a bug here: modifier is not handled correctly, maybe somthing with modifier mask in keylayouts.h"
      if(modifier) {
        pressed_keys[0] = modifier;
        sendKeyboard(pressed_keys); delay(10);
      }
      pressed_keys[2] = kc;
      sendKeyboard(pressed_keys); delay(10);
      pressed_keys[0] = 0;
      pressed_keys[2] = 0;
      sendKeyboard(pressed_keys); delay(10);
    }
    
    //go to next byte
    len++;
  }
  //reset the static UTF-8 buffer there.
  parse_for_keycode(0,kbdLocale,&modifier,&deadkey);
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
  uint8_t key8 = key & 0x00FF;
  
  //check if it is a modifier
  if((key & 0xFF00) == 0xE000) {
    pressed_keys[0] |= key8; //set bit 0 - 7, depending on modifier.
    return;
  }
  
  //place other keycodes into upper 6 bytes of key array
  for (int i = 2; i < KEYPRESS_BUFFERSIZE; i++)
  {
    if (pressed_keys[i] == 0) pressed_keys[i] = key8;
    if (pressed_keys[i] == key8) break;
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
  uint8_t key8 = key & 0x00FF;
  
  //check if it is a modifier
  if((key & 0xFF00) == 0xE000) {
    pressed_keys[0] &= ~key8; //clear bit 0 - 7, depending on modifier.
    return;
  }
  
  //remove other keycodes from upper 6 bytes of key array
  for (int i = 2; i < KEYPRESS_BUFFERSIZE; i++)
  {
    //is this possible slot containing the keycode?
    if(pressed_keys[i] == key8)
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
  uint8_t key8 = key & 0x00FF;
  
  //check if it is a modifier
  if((key & 0xE000) == 0xE000) {
    return (pressed_keys[0] & key8) ? 1 : 0;
  }
  
  for (int i = 2; i < KEYPRESS_BUFFERSIZE; i++)
  {
    if (pressed_keys[i] == key8)
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
  
  //copy parameter to local buffer, will be destroyed by strtok.
  strncpy(tmptxt, text,MAX_KEYSTRING_LEN);
  //add a trailing " " if not there.
  if (tmptxt[strnlen(tmptxt,MAX_KEYSTRING_LEN-1) - 1] != ' ') strcat(tmptxt, " ");

  acttoken = strtok(tmptxt, " ");
  while (acttoken)
  {
    if (!strncmp(acttoken, "KEY", 3)) {
      #ifdef DEBUG_OUTPUT_KEYS
        Serial.print("parsing: ");
        Serial.println(acttoken);
      #endif
      uint16_t key = parseIdentifierToKeycode(acttoken);
      if(key) updateKey(key, keyAction);
      else Serial.println("unknown KEY_");
    }

    acttoken = strtok(NULL, " ");
  }
  free(tmptxt);
}
