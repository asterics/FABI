
/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: mouse.h - mouse movement and click handling
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _MOUSECONTROL_H_
#define _MOUSECONTROL_H_

#define MOUSE_MOVEMENT_PRESCALER 4
#define MOUSE_ACCELDELAY   50         // steps to reach mouse move speed

void updateMouse();

#endif
