## Basic information
This configuration is dedicated for controlling the MAME game emulation software via USB keyboard/mouse (running e.g. on a RaspiberryPi computer).
You will need 5 buttons (connected to inputs 1-5) to access the essential functions for UI/menu and game control.
For more information about the MAME key mapping, see: [MAME default keys](https://docs.mamedev.org/usingmame/defaultkeys.html)

The first FABI slot provides standard mouse pointer control via USB, and the Escape key on button 5.
Slot 2 and 3 provide keyboard keys for menu and game navigation.
Button 5 changes between the three slots.


## Function assignment

### mouse (slot1, color: yellow):
- Button1 = short-press: move mouse up; 
- Button1 = long-press: move mouse down
- Button2 = short-press: move mouse left; 
- Button2 = long-press: move mouse right
- Button3 = click left mouse button
- Button4 = click right mouse button
- Button5 = short-press: load next slot; 
- Button5 = long-press: hold escape key (KEY_ESCAPE) 


### mame1 (slot2, color: red):
- Button1 = short-press: hold cursor up key (KEY_UP); 
- Button1 = long-press: hold cursor down key (KEY_DOWN)
- Button2 = short-press: hold cursor left key (KEY_LEFT); 
- Button2 = long-press: hold cursor right key (KEY_RIGHT)
- Button3 = short-press: hold ALT key (KEY_ALT); 
- Button3 = long-press: hold 1 key (KEY_1)
- Button4 = hold control key (KEY_CTRL)
- Button5 = load next slot


### mame2 (slot3, color: green):
- Button1 = short-press: hold cursor up key (KEY_UP);
- Button1 = long-press: hold cursor down key (KEY_DOWN)
- Button2 = short-press: hold cursor left key (KEY_LEFT);
- Button2 = long-press: hold cursor right key (KEY_RIGHT)
- Button3 = short-press: hold A key (KEY_A);
- Button3 = long-press: hold F1 key (KEY_F1)
- Button4 = hold enter key (KEY_ENTER)
- Button5 = load next slot

