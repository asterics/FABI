
/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: keys.h - keyboard codes and key press handling
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/


#ifndef _KEYS_H_
#define _KEYS_H_

#define KBD_DE 1
#define KBD_US 2


#define KEYBOARD_LAYOUT KBD_DE 
#define HID_REPORT_KEY_COUNT 6
 
#define MOD_ALTGR 256
#define MOD_SHIFT 512
 
#define KEY_UP    KEY_UP_ARROW
#define KEY_DOWN  KEY_DOWN_ARROW
#define KEY_LEFT  KEY_LEFT_ARROW
#define KEY_RIGHT KEY_RIGHT_ARROW
#define KEY_ENTER KEY_RETURN  
#define KEY_SPACE ' '
#define KEY_A 'a'
#define KEY_B 'b'
#define KEY_C 'c'
#define KEY_D 'd'
#define KEY_E 'e' 
#define KEY_F 'f'
#define KEY_G 'g'
#define KEY_H 'h'
#define KEY_I 'i'
#define KEY_J 'j'
#define KEY_K 'k'
#define KEY_L 'l'
#define KEY_M 'm'
#define KEY_N 'n'
#define KEY_O 'o'
#define KEY_P 'p'
#define KEY_Q 'q'
#define KEY_R 'r'
#define KEY_S 's'
#define KEY_T 't'
#define KEY_U 'u'
#define KEY_V 'v'
#define KEY_W 'w'
#define KEY_X 'x'
#define KEY_Y 'y'
#define KEY_Z 'z'
#define KEY_0 '0'
#define KEY_1 '1'
#define KEY_2 '2'
#define KEY_3 '3'
#define KEY_4 '4'
#define KEY_5 '5'
#define KEY_6 '6'
#define KEY_7 '7'
#define KEY_8 '8'
#define KEY_9 '9'

#define KEY_F13 0xF0 
#define KEY_F14 0xF1
#define KEY_F15 0xF2
#define KEY_F16 0xF3
#define KEY_F17 0xF4
#define KEY_F18 0xF5
#define KEY_F19 0xF6
#define KEY_F20 0xF7
#define KEY_F21 0xF8
#define KEY_F22 0xF9
#define KEY_F23 0xFA
#define KEY_F24 0xFB

struct keymap_struct {
  char *token;
  int key;
};

int getKeycode(char*);
void sendToKeyboard( char * );
void pressSingleKeys(char* text); // presses individual keys
void releaseSingleKeys(char* text);  // releases individual keys
void toggleSingleKeys(char* text); // toggles individual keys
void release_all();            // releases all previously pressed keys and buttons

#endif
