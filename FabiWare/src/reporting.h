/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: reporting.h - implementation of serial reports

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _REPORTING_H_
#define _REPORTING_H_

/**
   constant definitions for reporting modes
*/
#define REPORT_NONE  0
#define REPORT_ALL_SLOTS 1

#define CAP_PRESSURESENSOR (1<<0)
#define CAP_FORCESENSOR    (1<<1)
#define CAP_BOARD          (1<<2)
#define CAP_VID            (1<<3)
#define CAP_PID            (1<<4)
#define CAP_TOTALBYTES     (1<<5)
#define CAP_USEDBYTES      (1<<6)
#define CAP_FREEBYTES      (1<<7)
#define CAP_MEMORYUSED     (1<<8)
#define CAP_ALL           0xffff

/**
   extern declaration of static variables
   which shall be accessed from other modules
*/
extern uint8_t reportRawValues;

/** 
 * @brief Print current to given stream
 * @param S Stream to send the AT commands to; in our case Serial or a File
 */
void printCurrentSlot(Stream *S);

/** 
 * @brief Print global settings to given stream
 * @param S Stream to send the AT commands to; in our case Serial or a File
 */
void printGlobalSettings(Stream *S);

/**
   @name reportValues
   @brief prints the current live movement data and button values to the serial interface
   @return none
*/
void reportValues();

/**
   @name reportCapabilities
   @brief prints the current board, VID and PID, free memory, or available functions (selectable via bitmask)
   @return none
*/
void reportCapabilities(uint16_t mask);
#endif
