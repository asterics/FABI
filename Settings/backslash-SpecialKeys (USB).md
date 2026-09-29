## Basic information
This configuration demonstrates how to assign the backslash character (`\`) and special punctuation keys to FABI buttons (referencing Issue #36).
It allows generating backslashes across different physical keyboard layouts (en_US and de_DE) as well as inside macros.

## Function assignment

### specialKeys (slot1, color: blue):
- Button 1 = type backslash via US keycode (`AT KP KEY_BACKSLASH`)
- Button 2 = type backslash on German layout (`AT KP KEY_RIGHT_ALT KEY_MINUS`, AltGr + ß)
- Button 3 = write backslash directly (`AT KW \`)
- Button 4 = macro executing path command with escaped backslashes (`AT MA KW C:\\path\\to\\dir;KP KEY_ENTER;`)
- Button 5 = type slash (`AT KP KEY_SLASH`)
- Button 6 = type semicolon (`AT KP KEY_SEMICOLON`)
- Button 7 = hold left mouse button (`AT HL`)
- Button 8 = hold right mouse button (`AT HR`)
