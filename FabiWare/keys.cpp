
#include "fabi.h"

 #define KBD_DE 1
 #define KBD_US 2
 
 #define KEYBOARD_LAYOUT KBD_DE 
 
 #define MOD_ALTGR 256
 #define MOD_SHIFT 512
 
 #ifdef ARDUINO_PRO_MICRO
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
#endif

#define KEY_ADD     0
#define KEY_RELEASE 1

int keyAction=KEY_ADD;

void updateKey(int key)
{
   if (keyAction==KEY_ADD)
      Keyboard.press(key);     // press keys individually   
   else
      Keyboard.release(key);   // release keys individually    
}


void releaseKeys (char * text)
{
   keyAction=KEY_RELEASE; 
   setKeyValues(text);
   keyAction=KEY_ADD; 
}

int getKeycode(char* acttoken)
{
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_SHIFT")))  return(KEY_LEFT_SHIFT);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_CTRL")))  return(KEY_LEFT_CTRL);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_ALT")))  return(KEY_LEFT_ALT);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_RIGHT_ALT")))  return(KEY_RIGHT_ALT);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_GUI")))  return(KEY_LEFT_GUI);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_RIGHT_GUI")))  return(KEY_RIGHT_GUI);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_UP"))) return(KEY_UP);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_DOWN"))) return(KEY_DOWN);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_LEFT"))) return(KEY_LEFT);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_RIGHT"))) return(KEY_RIGHT);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_ENTER"))) return(KEY_ENTER);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_SPACE"))) return(KEY_SPACE);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_ESC"))) return(KEY_ESC);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_BACKSPACE"))) return(KEY_BACKSPACE);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_TAB"))) return(KEY_TAB);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_CAPS_LOCK"))) return(KEY_CAPS_LOCK);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_F1"))) return(KEY_F1);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_F2"))) return(KEY_F2);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_F3"))) return(KEY_F3);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_F4"))) return(KEY_F4);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_F5"))) return(KEY_F5);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_F6"))) return(KEY_F6);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_F7"))) return(KEY_F7);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_F8"))) return(KEY_F8);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_F9"))) return(KEY_F9);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_F10"))) return(KEY_F10);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_F11"))) return(KEY_F11);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_F12"))) return(KEY_F12);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_INSERT"))) return(KEY_INSERT);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_HOME"))) return(KEY_HOME);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_PAGE_UP"))) return(KEY_PAGE_UP);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_DELETE"))) return(KEY_DELETE);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_END"))) return(KEY_END);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_PAGE_DOWN"))) return(KEY_PAGE_DOWN);

    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_A"))) return(KEY_A);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_B"))) return(KEY_B);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_C"))) return(KEY_C);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_D"))) return(KEY_D);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_E"))) return(KEY_E);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_F"))) return(KEY_F);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_G"))) return(KEY_G);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_H"))) return(KEY_H);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_I"))) return(KEY_I);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_J"))) return(KEY_J);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_K"))) return(KEY_K);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_L"))) return(KEY_L);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_M"))) return(KEY_M);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_N"))) return(KEY_N);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_O"))) return(KEY_O);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_P"))) return(KEY_P);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_Q"))) return(KEY_Q);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_R"))) return(KEY_R);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_S"))) return(KEY_S);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_T"))) return(KEY_T);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_U"))) return(KEY_U);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_V"))) return(KEY_V);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_W"))) return(KEY_W);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_X"))) return(KEY_X);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_Y"))) return(KEY_Y);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_Z"))) return(KEY_Z);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_1"))) return(KEY_1);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_2"))) return(KEY_2);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_3"))) return(KEY_3);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_4"))) return(KEY_4);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_5"))) return(KEY_5);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_6"))) return(KEY_6);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_7"))) return(KEY_7);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_8"))) return(KEY_8);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_9"))) return(KEY_9);
    if (!strcmp_FM(acttoken,(uint_farptr_t_FM)F("KEY_0"))) return(KEY_0);
    
    #ifdef TEENSY     // for Teensy2.0++
      if (!strcmp(acttoken,"KEY_SCROLL_LOCK")) return(KEY_SCROLL_LOCK);
      if (!strcmp(acttoken,"KEY_PAUSE")) return(KEY_PAUSE);
      if (!strcmp(acttoken,"KEY_NUM_LOCK")) return(KEY_NUM_LOCK);
      if (!strcmp(acttoken,"KEY_PRINTSCREEN")) return(KEY_PRINTSCREEN);
    #endif

    return(0);
}

// press all supported keys 
// text is a string which contains the key identifiers eg. "KEY_CTRL KEY_C" for Ctrl-C
void setKeyValues(char* text)
{
  char tmptxt[MAX_KEYSTRING_LEN];   // for parsing keystrings
  char * acttoken;
  int modifiers=0;

  strcpy(tmptxt, text); 
  acttoken = strtok(tmptxt," ");

  while (acttoken)
  {
    int kc=getKeycode(acttoken);
    if (kc) updateKey(kc);
    acttoken = strtok(NULL," ");
  }
}


void writeTranslatedKeys( char * p1)
{
  int k;
     while (*p1) {
          if (KEYBOARD_LAYOUT == KBD_DE)
             k=pgm_read_word_near(&(usToDE[(uint8_t)*p1]));  // get the translated keycode (DE layout)
          else k=*p1;
          
         // Serial.print ("char:"); Serial.print(*p1); Serial.print(" -> ");Serial.print(k);Serial.print(" (");
         // if (k&MOD_ALTGR) Serial.print("AltGr + "); if (k&MOD_SHIFT) Serial.print("Shift + "); 
         // Serial.print((char)(k&0xff)); Serial.println(")");

          if (k&MOD_ALTGR) Keyboard.press(KEY_RIGHT_ALT); 
          if (k&MOD_SHIFT) Keyboard.press(KEY_LEFT_SHIFT); 
          Keyboard.press(k&0xff); 
          Keyboard.release(k&0xff); 
          if (k&MOD_SHIFT) Keyboard.release(KEY_LEFT_SHIFT); 
          if (k&MOD_ALTGR) Keyboard.release(KEY_RIGHT_ALT);
          p1++;
       }
}

void sendToKeyboard(char * writeKeystring)
{
    char tmp[MAX_KEYSTRING_LEN]; 
    char * p1, *p2, *p3;
    strcpy (tmp,writeKeystring);
    p1=tmp;
    p2=strstr(p1,"KEY_");
    while (p2)
    { 
       p3=p2; while ((*p3!=' ') && (*p3)) p3++; 
       if (*p3) {*p3=0; p3++;}
       int kc= getKeycode(p2);
       *p2=0; 
       if (p1!=p2) writeTranslatedKeys (p1);
       if (kc)  Keyboard.write(kc); 
       p1=p3; if (*p1) p2=strstr(p1,"KEY_"); else p2=0;
    }
    writeTranslatedKeys(p1);
}


// here comes a character translation table - this works only for DE by now ...

const int usToDE[] PROGMEM = 
{
//  0,  0,  0,  0,  0,  0,  0,  0, BS, TB, CR,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  8,  9, 10,  0,  0, 13,  0,  0,

    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

// BL,  !,  Ä,  §,     $,  %,  /,       ä,        ),  =,  (,  `,  ,,  ß,  .,  -,
//          "                  &        /         (   )   *   +       -       /
   32, 33, 64,  '\\', 36, 37, 94, MOD_SHIFT+'\\', 42, 40,125,184, 44, 47, 46, 38,

//  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  Ö,  ö, ;,  ´,  :,  _,
//                                          :   ;  <  |    >   =        
   48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 62, 60, 0, 41,  0, 95,

//      ",          A,  B,  C,  D,  E,  F,  G,  H,  I,  J,  K,  L,  M,  N,  O,
//      @
    MOD_ALTGR+'q', 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,

//  P,  Q,  R,  S,  T,  U,  V,  W,  X,  Z,  Y,      ü,                #,                +,          &,  ?,
//                                      Y   Z       [                 \                 ]           ^   _
   80, 81, 82, 83, 84, 85, 86, 87, 88, 90, 89,  MOD_ALTGR+KEY_8,  MOD_ALTGR+'-',  MOD_ALTGR+KEY_9, 96, 63,

//  ^,  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,
//  `
   43, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,

//  p,  q,  r,  s,  t,  u,  v,  w,  x,  z,  y,         Ü,         ,           *,        °,  0,
//                                      y   z          {          |           }         ~
  112,113,114,115,116,117,118,119,120,122,121,  MOD_ALTGR+KEY_7,  0,  MOD_ALTGR+KEY_0,  0,  0,


// 
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  '\"',  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  ':',  0,  0,  0,  0,  0,  MOD_SHIFT+'[',  0,  0,  '-',
    0,  0,  0,  0,  '\'',  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  ';',  0,  0,  0,  0,  0,  '[',  0,  0,  0,

};


