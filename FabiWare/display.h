/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: display.h - implementation of the Oled display functions 

     Note: Oled display connects to I2C-1 (Wire) for FlipMouse
     The utilized library is https://github.com/greiman/SSD1306Ascii

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/**
   @name initDisplay
   @brief initalizes the SSD1306 LCD using I2C interface Wire0 or Wire1
   @return true if LCD could be initialized, false if not
*/
uint8_t initDisplay();

/**
   @name isDisplayAvailable
   @brief returns true if LC Display is available at interface Wire0 or Wire1
   @return true if LCD was initialized, false if not
*/
uint8_t isDisplayAvailable();


/**
   @name displayClear
   @brief clears content of SSD1306 LCD
   @return none
*/
void displayClear(void);

/**
   @name displayUpdate
   @brief clears content of SSD1306 LCD according to slot settings
   @return none
*/
void displayUpdate(void);

/**
   @name pauseDisplayUpdates
   @param pause: if true, display updates will be paused
   @brief allows to bypass display updates
   @return none
*/
void pauseDisplayUpdates(uint8_t pause);


/**
   @name displayUpdate
   @brief displays a message on the SSD1306 LCD 
   @param msg message string 
   @return none
*/
void displayMessage(char * msg);

#endif
