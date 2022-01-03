## Basic information
This configuration is dedicated for controlling the MAME game emulation software via USB keyboard/mouse (running e.g. on a RaspiberryPi computer).
You will need 8 buttons (connected to inputs 1-8) to access the essential functions for UI/menu and game control.
For more information about the MAME key mapping, see: [MAME default keys](https://docs.mamedev.org/usingmame/defaultkeys.html)

The first FABI slot provides standard mouse pointer control via USB, and the Escape key on button 7.
Slot 2 and 3 provide keyboard keys for menu and game navigation.
Button 8 changes between the three slots.


## Function assignment

### mouse (slot1, color: yellow):
- Button1 = move mouse left
- Button2 = move mouse up
- Button3 = move mouse right
- Button4 = move mouse down
- Button5 = click left mouse button
- Button6 = click right mouse button
- Button7 = hold escape key (KEY_ESCAPE)
- Button8 = load next slot

### mame1 (slot2, color: red):
- Button1 = hold cursor up key (KEY_LEFT)
- Button2 = hold cursor down key (KEY_UP)
- Button3 = hold cursor left key (KEY_RIGHT)
- Button4 = hold cursor right key (KEY_DOWN)
- Button5 = hold ALT key (KEY_ALT)
- Button6 = hold control key (KEY_CTRL)
- Button7 = hold 1 key (KEY_1)
- Button8 = load next slot


### mame2 (slot3, color: green):
- Button1 = hold cursor up key (KEY_LEFT)
- Button2 = hold cursor down key (KEY_UP)
- Button3 = hold cursor left key (KEY_RIGHT)
- Button4 = hold cursor right key (KEY_DOWN)
- Button5 = hold A key (KEY_A)
- Button6 = hold F1 key (KEY_F1)
- Button7 = hold enter key (KEY_ENTER)
- Button8 = load next slot

