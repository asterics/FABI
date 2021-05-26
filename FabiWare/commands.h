
/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: commands.h - AT command processing
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html


     Supported AT-commands:  
     (sent via serial interface, 115200 baud, using spaces between parameters.  Enter (<cr>, ASCII-code 0x0d) finishes a command)
   
          AT              returns "OK"
          AT ID           returns identification string (e.g. "Fabi V2.3")
          AT BM <uint>    puts button into programming mode (e.g. "AT BM 2" -> next AT-command defines the new function for button 2)
                          for the FABI, there are 11 buttons available (9 physical buttons, 2 virtual functions - sip / puff)
          AT MA <string>  execute a command macro containing multiple commands (separated by semicolon) 
                          example: "AT MA MX 100;MY 100;CL;"  use backslash to mask semicolon: "AT MA KW \;;CL;" writes a semicolon and then clicks left 
          AT WA <uint>    wait (given in milliseconds, useful for macro commands)

      Commands for changing settings:
          AT WS <uint>    set mouse wheel stepsize (e.g. "AT WS 3" sets the wheel stepsize to 3 rows)
          AT TS <uint>    threshold for sip action  (0-512)
          AT TP <uint>    threshold for puff action (512-1023)
          AT TT <uint>    threshold time for long press action (0=disable)
          AT DP <uint>    threshold time for double press to skip slot  (0=disable)
          AT AD <uint>    threshold time for automatic dwelling after mouse movement (0=disable)
          AT AP <uint>    antitremor press time (1-500)
          AT AR <uint>    antitremor release time (1-500)
          AT AI <uint>    antitremor idle time (1-500)
          AT SC <string>  change slot color: given string 0xRRGGBB 

      USB HID commands: 
          AT CL             click left mouse button  
          AT CR             click right mouse button  
          AT CM             click middle mouse button  
          AT CD             click double with left mouse button

          AT HL             hold the left mouse button  
          AT HR             hold the right mouse button
          AT HM             hold the middle mouse button 
  
          AT RL             release the left mouse button  
          AT RR             release the right mouse button
          AT RM             release the middle mouse button 

          AT TL             toggle the left mouse button  
          AT TR             toggle the right mouse button
          AT TM             toggle the middle mouse button 

          AT WU             move mouse wheel up  
          AT WD             move mouse wheel down  
   
          AT MX <int>       move mouse in x direction (e.g. "AT MX 4" moves cursor 4 pixels to the right)  
          AT MY <int>       move mouse in y direction (e.g. "AT MY -10" moves cursor 10 pixels up)  

          AT KW <string>    keyboard write string (e.g." AT KW Hello!" writes "Hello!")    
          AT KP <string>    key press: press keys once (automatic release after all keys were pressed)
                            (e.g. "AT KP KEY_UP" presses the "Cursor-Up" key, "AT KP KEY_CTRL KEY_ALT KEY_DELETE" presses all three keys)
          AT KH <string>    key hold: hold keys (automatic release when user button is released)
                            (e.g. "AT KH KEY_A" presses the "A" key until  "AT KR KEY_A" is sent)
          AT KT <string>    key toggle: "sticky" hold keys (key will be pressed until "AT KT" command is sent again or a "AT KR" command is sent)
                            in contrast to "AT KH" a finished user action does not release the keys
                            (e.g. "AT KT KEY_A" presses the "A" key until  "AT KT KEY_A" is sent again.)
                            for a list of supported key idientifier strings see below ! 
                            
          AT KR <string>    key release: releases all keys identified in the string    
          AT RA             release all: releases all currently pressed keys and buttons    
          
    Housekeeping commands:
          AT SA <string>  save settings and current button modes to next free eeprom slot under given name (e.g. "AT SA keys1")                          a new slot will be appended (e.g. AT SA mouse1)
          AT LO <string>  load button modes from eeprom slot (e.g. AT LO mouse1 -> loads profile named "mouse1")
          AT LA           load all slots (displays names and settings of all stored slots) 
          AT LI           list all saved slot names 
          AT NE           next slot will be loaded (wrap around after last slot)
          AT DE <string>  delete slot of given name (deletes all stored slots if no string parameter is given)
          AT NC           no command (idle operation)
          AT SR           start periodic reporting analog values (A0) over serial (starting with "VALUES:") 
          AT ER           end reporting analog values
          AT FR           report free EEPROM bytes in % (starting with "FREE:") 
          AT BT <uint>    set bluetooth mode, 1=USB only, 2=BT only, 3=both(default)
                          (e.g. AT BT 2 -> send HID commands only via BT if BT-daughter board is available)
          AT BC <string>  sends parameter to external UART (mostly ESP32 Bluetooth Addon)
          AT UG           start addon upgrade, Serial ports are transparent until ("$FIN") is received.

   supported key identifiers for key press command (AT KP):
 
    KEY_A   KEY_B   KEY_C   KEY_D    KEY_E   KEY_F   KEY_G   KEY_H   KEY_I   KEY_J    KEY_K    KEY_L
    KEY_M   KEY_N   KEY_O   KEY_P    KEY_Q   KEY_R   KEY_S   KEY_T   KEY_U   KEY_V    KEY_W    KEY_X 
    KEY_Y   KEY_Z   KEY_1   KEY_2    KEY_3   KEY_4   KEY_5   KEY_6   KEY_7   KEY_8    KEY_9    KEY_0
    KEY_F1  KEY_F2  KEY_F3  KEY_F4   KEY_F5  KEY_F6  KEY_F7  KEY_F8  KEY_F9  KEY_F10  KEY_F11  KEY_F12  
    KEY_F13 KEY_F14 KEY_F15 KEY_F16  KEY_F17 KEY_F18 KEY_F19 KEY_F20 KEY_F21 KEY_F22  KEY_F23  KEY_F24  
    
    KEY_RIGHT   KEY_LEFT       KEY_DOWN        KEY_UP      KEY_ENTER    KEY_ESC   KEY_BACKSPACE   KEY_TAB 
    KEY_HOME    KEY_PAGE_UP    KEY_PAGE_DOWN   KEY_DELETE  KEY_INSERT   KEY_END   KEY_NUM_LOCK    KEY_SCROLL_LOCK
    KEY_SPACE   KEY_CAPS_LOCK  KEY_PAUSE       KEY_SHIFT   KEY_CTRL     KEY_ALT   KEY_RIGHT_ALT   KEY_GUI 
    KEY_RIGHT_GUI

*/





#ifndef _COMMANDS_H_
#define _COMMANDS_H_

// command identifiers
enum atCommands {
  CMD_ID, CMD_BM, CMD_CL, CMD_CR, CMD_CM, CMD_CD, CMD_HL, CMD_HR, CMD_HM, CMD_RL, CMD_RR, CMD_RM,
  CMD_TL, CMD_TR, CMD_TM, CMD_WU, CMD_WD, CMD_WS, CMD_MX, CMD_MY, CMD_KW, CMD_KP, CMD_KH, CMD_KT, 
  CMD_KR, CMD_RA, CMD_SA, CMD_LO, CMD_LA, CMD_LI, CMD_NE, CMD_DE, CMD_NC, CMD_SR, CMD_ER, CMD_TS, 
  CMD_TP, CMD_MA, CMD_WA, CMD_TT, CMD_AP, CMD_AR, CMD_AI, CMD_FR, CMD_BT, CMD_BC, CMD_DP, CMD_AD,
  CMD_SC, CMD_UG, NUM_COMMANDS
};

#define PARTYPE_NONE   0
#define PARTYPE_UINT   1
#define PARTYPE_INT    2
#define PARTYPE_STRING 3

void performCommand (uint8_t cmd, int16_t par1, char * keystring, int8_t periodicMouseMovement);


#endif
