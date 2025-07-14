/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: tone.h - functions for tone/audio feedback 

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _TONE_H_
#define _TONE_H_

#include "FlipWare.h"
#include "gpio.h"

#define SOUND_FOLDER "/sounds/"
#define MAX_PATH_LEN 32
#define WAIT_FOR_TRANSFER_TIMEOUT 10000  

/**
   constant definitions for tone generation 
*/ 
#define TONE_CALIB            1
#define TONE_CHANGESLOT       2
#define TONE_ENTER_STRONGSIP  3
#define TONE_EXIT_STRONGSIP   4
#define TONE_ENTER_STRONGPUFF 5
#define TONE_EXIT_STRONGPUFF  6
#define TONE_INDICATE_SIP     7
#define TONE_INDICATE_PUFF    8
#define TONE_IR               9
#define TONE_BT_PAIRING      10
#define TONE_IR_REC          11
#define TONE_STARTUP         12

/**
   @name updateTones
   @brief updates state/timing of running tone signals
   @return none
*/
void updateTones();

/**
   @name makeTone
   @brief initiates a new tone signal
   @return none
*/
void makeTone(uint8_t kind, uint8_t param);

/**
   @name initAudio
   @brief initialize the audio playback system (if available)
   @return none
*/
void initAudio();

/**
   @name audioList
   @brief print a list of available audio files
   @return none
*/
void audioList();

/**
   @name audioRemove
   @param fn: filename for the audio file
   @brief removes the audio file from LitteFS (if it exists)
   @return none
*/
void audioRemove(char * fn);

/**
   @name audioPlayback
   @param fn: filename for the audio file
   @brief starts playback of an audio wav file from LitteFS
   @return none
*/
void audioPlayback(char * fn);

/**
   @name audioTransfer
   @param fn: filename for the audio file
   @brief receive binary wav data and store it into a local file in LitteFS (timeout: 10 seconds)
   @return true if a file was received, false otherwise
*/
uint8_t audioTransfer(char * fn);

#endif
