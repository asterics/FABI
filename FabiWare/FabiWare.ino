  
/* 
     Flexible Assistive Button Interface (FABI)  Version 2.3  - AsTeRICS Foundation - http://www.asterics-foundation.org
      allows control of HID functions via momentary switches and/or AT-commands  
   

   requirements:  USB HID capable Arduino (Leonardo / Micro / Pro Micro) - see #define in fabi.h !
                  or Teensy 2.0++ with Teensyduino AddOn setup as USB composite device (Mouse + Keyboard + Serial)
       optional:  Momentary switches connected to GPIO pins / force sensors connected to ADC pins
       
   for a list of supported AT commands, see commands.h / commands.cpp    

*/

#include "fabi.h"
#include <EEPROM.h>

// global variables

uint8_t DebugOutput=0;  // Use 1 for chatty serial output (but it won't be compatible with GUI)

#define SIP_BUTTON    9
#define PUFF_BUTTON  10
#define PRESSURE_SENSOR_PIN A0


#ifdef TEENSY
  int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS]={16,17,18,19,20,21,22,23,24};  //  map physical button pins to button index 0,1,2  
  int8_t  led_map[NUMBER_OF_LEDS]={1,2,3};                //  maps leds pins   
  uint8_t LED_PIN = 6;                                    //  Led output pin
#endif

#ifdef ARDUINO_PRO_MICRO
  int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS]={2,3,4,5,6,7,8,9,10};
  int8_t  led_map[NUMBER_OF_LEDS]={14,15,16};            
  uint8_t LED_PIN = 17;
#endif

struct settingsType settings = {      // type definition see fabi.h
    "slot1", DEFAULT_WHEEL_STEPSIZE, DEFAULT_TRESHOLD_TIME, 
    DEFAULT_SIP_THRESHOLD, DEFAULT_PUFF_THRESHOLD,
    DEFAULT_ANTITREMOR_PRESS, DEFAULT_ANTITREMOR_RELEASE, DEFAULT_ANTITREMOR_IDLE
}; 


struct buttonType buttons [NUMBER_OF_BUTTONS];                     // array for all buttons - type definition see fabi.h 
struct buttonDebouncerType buttonDebouncers [NUMBER_OF_BUTTONS];   // array for all buttonsDebouncers - type definition see fabi.h 
char   cmdstring[MAX_CMDLEN];                 // buffer for incoming AT commands
char   keystringBuffer[KEYSTRING_BUFFER_LEN]; // buffer for all string parameters for the buttons of a slot
uint16_t freeEEPROMbytes=0;

int clickTime=DEFAULT_CLICK_TIME;
int waitTime=DEFAULT_WAIT_TIME;

uint8_t reportSlotParameters = 0;
uint8_t valueReportCount=0;
uint8_t actSlot=0;

int8_t moveX=0;       
int8_t moveY=0;       
uint8_t moveXcnt=0;       
uint8_t moveYcnt=0;       
uint8_t leftMouseButton=0,old_leftMouseButton=0;
uint8_t middleMouseButton=0,old_middleMouseButton=0;
uint8_t rightMouseButton=0,old_rightMouseButton=0;
uint8_t leftClickRunning=0;
uint8_t rightClickRunning=0;
uint8_t middleClickRunning=0;
uint8_t doubleClickRunning=0;

int inByte=0;
uint16_t pressure=0;
uint8_t reportRawValues = 0;

uint8_t cnt =0,cnt2=0;


// function declarations 
void handlePress (int buttonIndex);      // a button was pressed
void handleRelease (int buttonIndex);    // a button was released
void handleButton(int i, int l, uint8_t b);  // button debouncing
void UpdateLeds();
void initDebouncers();

////////////////////////////////////////
// Setup: program execution starts here
////////////////////////////////////////

void setup() {
   Serial.begin(9600);
    // delay(5000);
    // while (!Serial) ;
   
   if (DebugOutput==1) {  
     Serial.println(F("Flexible Assistive Button Interface started !"));
   }

   #ifdef ARDUINO_PRO_MICRO   // only needed for Arduino, automatically done for Teensy(duino)
     Mouse.begin();
     Keyboard.begin();
     TXLED1;
   #endif  

   pinMode(LED_PIN,OUTPUT);

   for (int i=0; i<NUMBER_OF_PHYSICAL_BUTTONS; i++)   // initialize physical buttons and bouncers
      pinMode (input_map[i], INPUT_PULLUP);   // configure the pins for input mode with pullup resistors

   for (int i=0; i<NUMBER_OF_LEDS; i++)   // initialize physical buttons and bouncers
      pinMode (led_map[i], OUTPUT);   // configure the pins for input mode with pullup resistors

   for (int i=0; i<NUMBER_OF_BUTTONS; i++)   // initialize button array
   {
      buttons[i].mode=CMD_PL;              // default command for every button is left mouse click
      buttons[i].value=0;
      keystringBuffer[i]=0;
   }


   initDebouncers(); 

   readFromEEPROM(0);  // read button modes from first EEPROM slot if available !  
   BlinkLed();
   if (DebugOutput==1) {  
     Serial.print(F("Free RAM:"));  Serial.println(freeRam());
   }
   
   /* Enable for debugging the clock registers in any case of problems.
   delay(1000);
   Serial.print(F("Clock register:"));
   Serial.print(CLKSEL0); Serial.print(":");
   Serial.print(CLKSEL1); Serial.print(":");
   Serial.print(CLKSTA); Serial.print(":");
   Serial.print(CLKPR); Serial.print(":");
   Serial.print(PLLCSR); Serial.print(":");
   Serial.print(PLLFRQ); Serial.println(":");
   */
}

///////////////////////////////
// Loop: the main program loop
///////////////////////////////


void loop() {  

      pressure = analogRead(PRESSURE_SENSOR_PIN);

      while (Serial.available() > 0) {
        // get incoming byte:
        inByte = Serial.read();
        parseByte (inByte);      // implemented in parser.cpp
      }
    
      for (int i=0;i<NUMBER_OF_PHYSICAL_BUTTONS;i++)    // update button press / release events
          handleButton(i, i+6, digitalRead(input_map[i]) == LOW ? BUTTON_PRESSED : BUTTON_RELEASED);    

      if (settings.ts>0)    handleButton(SIP_BUTTON, -1, pressure < settings.ts ? BUTTON_PRESSED : BUTTON_RELEASED); 
      if (settings.tp<1023) handleButton(PUFF_BUTTON, -1, pressure > settings.tp ? BUTTON_PRESSED : BUTTON_RELEASED);

        
      if (moveX==0) moveXcnt=0; 
      if (moveY==0) moveYcnt=0; 
      if ((moveX!=0) || (moveY!=0))   // movement induced by button actions  
      {
        if (cnt2++%4==0)
        {
          if (moveX!=0) if (moveXcnt<MOUSE_ACCELDELAY) moveXcnt++;
          if (moveY!=0) if (moveYcnt<MOUSE_ACCELDELAY) moveYcnt++;

          Mouse.move(moveX * moveXcnt/MOUSE_ACCELDELAY, moveY * moveYcnt/MOUSE_ACCELDELAY);
        }
      }

      // handle running clicks or double clicks
      if (leftClickRunning)
          if (--leftClickRunning==0)  leftMouseButton=0; 
      
      if (rightClickRunning)
          if (--rightClickRunning==0)  rightMouseButton=0; 
   
      if (middleClickRunning)
          if (--middleClickRunning==0)  middleMouseButton=0; 
  
      if (doubleClickRunning)
      {
          doubleClickRunning--;
          if (doubleClickRunning==clickTime*2)  leftMouseButton=0; 
          else if (doubleClickRunning==clickTime)    leftMouseButton=1; 
          else if (doubleClickRunning==0)    leftMouseButton=0; 
      }
 
      // if any changes were made, update the Mouse buttons
      if(leftMouseButton!=old_leftMouseButton) {
         if (leftMouseButton) Mouse.press(MOUSE_LEFT); else Mouse.release(MOUSE_LEFT);
         old_leftMouseButton=leftMouseButton;
      }
      if  (middleMouseButton!=old_middleMouseButton) {
         if (middleMouseButton) Mouse.press(MOUSE_MIDDLE); else Mouse.release(MOUSE_MIDDLE);
         old_middleMouseButton=middleMouseButton;
      }
      if  (rightMouseButton!=old_rightMouseButton)  {
         if (rightMouseButton) Mouse.press(MOUSE_RIGHT); else Mouse.release(MOUSE_RIGHT);
         old_rightMouseButton=rightMouseButton;
    }
    
    if (reportRawValues)   { 
       if (valueReportCount++ > 10) {      // report raw values !
           Serial.print("VALUES:");Serial.print(pressure);Serial.println(",");  
        valueReportCount=0; 
      }
    }
       
    UpdateLeds();
    
    //we need a workaround for different clock settings on Arduino Pro Micro boards.
    //millis is not working correctly, the delay seems to be 8x higher if wrong clock
    //settings are active. If the "defect" Pro Micro is booted via double reset, it works.
    //The difference does not affect USB, because the Mega32U4 has multiple valid settings
    //for driving USB with 48MHz.
    
    //Good clock settings: CLKPR = 0 (no prescaler); PLLFRQ = 0x4A (96MHz PLL, divide by 2 for USB)
    //"Bad" clock settings: CLKPR = 0x03 (prescaler 8); PLLFRQ = 0x04 (48MHz PLL, USB connected directly)
    #ifdef ARDUINO_PRO_MICRO
		//these register settings might be used with 8MHz too, and there is no problem if F_CPU is also 8MHz.
		#if F_CPU == 16000000l
			if(CLKPR == 0x00 && PLLFRQ == 0x4A) //good settings
			{
				delay(waitTime);
			} else {
				delayMicroseconds((waitTime*1000) / 8);
			}
		#else
			delay(waitTime);
		#endif
    #else
		delay(waitTime);  // to limit movement speed. TBD: remove delay, use millis() !
	#endif
}


void handlePress (int buttonIndex)   // a button was pressed
{   
    performCommand(buttons[buttonIndex].mode,buttons[buttonIndex].value,getKeystring(buttonIndex),1);
}

void handleRelease (int buttonIndex)    // a button was released
{
   switch(buttons[buttonIndex].mode) {
     case CMD_PL: leftMouseButton=0; break;
     case CMD_PR: rightMouseButton=0; break;
     case CMD_PM: middleMouseButton=0; break;
     case CMD_MX: moveX=0; break;      
     case CMD_MY: moveY=0; break;      
     case CMD_KP: releaseSingleKeys(getKeystring(buttonIndex)); break; 
   }
}

void initDebouncers()
{
   for (int i=0; i<NUMBER_OF_BUTTONS; i++)   // initialize button array
   {
      buttonDebouncers[i].pressCount=0;
      buttonDebouncers[i].releaseCount=0;
      buttonDebouncers[i].idleCount=0;
      buttonDebouncers[i].pressState=BUTTONSTATE_NOT_PRESSED;
   }
}

void release_all()  // releases all previously pressed keys
{
    // Serial.println("release all!");
    Keyboard.releaseAll();
    leftMouseButton=0;
    rightMouseButton=0;
    middleMouseButton=0;
    moveX=0;
    moveY=0;
}


// button debouncing and longpress detection  
// (if button i is pressed long and index l>=0, virtual button l is activated !)
void handleButton(int i, int l, uint8_t actState)    
{ 
    if (buttonDebouncers[i].pressState==BUTTONSTATE_IDLE) {
       buttonDebouncers[i].idleCount++;
       if (buttonDebouncers[i].idleCount >= settings.ai) {
        buttonDebouncers[i].idleCount=0;
        buttonDebouncers[i].pressCount=0;
        buttonDebouncers[i].releaseCount=0;
        buttonDebouncers[i].pressState=BUTTONSTATE_NOT_PRESSED;
       }
       return;
    }
            
    if ((actState == BUTTON_PRESSED)) // && (buttonDebouncers[i].pressState == BUTTONSTATE_NOT_PRESSED)) 
    {
       buttonDebouncers[i].releaseCount=0;
       if (buttonDebouncers[i].pressCount<=settings.tt)
          buttonDebouncers[i].pressCount++;           
       if (buttonDebouncers[i].pressCount==settings.ap) {
           handlePress(i);           
           buttonDebouncers[i].pressState=BUTTONSTATE_SHORT_PRESSED;           
       }
       if ((buttonDebouncers[i].pressCount==settings.tt) && (settings.tt<5000) && (l>=0) && (l<NUMBER_OF_BUTTONS)) {
           handleRelease(i);           
           handlePress(l);
           buttonDebouncers[i].pressState=BUTTONSTATE_LONG_PRESSED;           
       }
    }
    
    if ((actState == BUTTON_RELEASED)) // && (buttonDebouncers[i].pressState != BUTTONSTATE_NOT_PRESSED)) 
    {
       buttonDebouncers[i].pressCount=0;           
       if (buttonDebouncers[i].releaseCount<=settings.ar)
         buttonDebouncers[i].releaseCount++;           
       if (buttonDebouncers[i].releaseCount==settings.ar) {
         if(buttonDebouncers[i].pressState==BUTTONSTATE_SHORT_PRESSED) { 
            handleRelease(i);
            buttonDebouncers[i].pressState=BUTTONSTATE_IDLE;
         }
         else if(buttonDebouncers[i].pressState==BUTTONSTATE_LONG_PRESSED) {
            handleRelease(l);
            buttonDebouncers[i].pressState=BUTTONSTATE_IDLE;
         }
       }
    }
}   

char * getKeystring (uint8_t button) 
{
  char *s=keystringBuffer;
  for (int i=0;i<button;i++)
  {
    while (*s) s++;
    s++;    
  }
  return(s);
}

uint16_t keystringMemUsage(uint8_t button)
{
  uint16_t sum=0;
  for (int i=button; i<NUMBER_OF_BUTTONS;i++)
    sum+=strlen(getKeystring(i))+1;
  return(sum);
}

void setKeystring (uint8_t button, char * text)
{
  if (keystringMemUsage(0)-strlen(getKeystring(button))+strlen(text) >= KEYSTRING_BUFFER_LEN)
     return;

  if (button < NUMBER_OF_BUTTONS-1) {
    uint16_t bytesToCopy=keystringMemUsage(button+1);
    int16_t delta = strlen(text)-strlen(getKeystring(button));
    memmove(getKeystring(button+1)+delta, getKeystring(button+1), bytesToCopy);
  }
  strcpy(getKeystring(button),text);
}

void printKeystrings ()
{
  Serial.print("Used RAM for Keystrings:");Serial.print(keystringMemUsage(0));
  Serial.print(" (free: ");Serial.print(KEYSTRING_BUFFER_LEN-keystringMemUsage(0));
  Serial.println(")");

  for (int i=0;i<NUMBER_OF_BUTTONS;i++) {
    Serial.print("Keystring ");Serial.print(i);Serial.print(":");Serial.println(getKeystring(i));
  }
}

void BlinkLed()
{
    for (uint8_t i=0; i < 5;i++)
    {
        digitalWrite (LED_PIN, !digitalRead(LED_PIN));
        delay(100);
    }
    digitalWrite (LED_PIN, HIGH);
}

void UpdateLeds()
{  
   if (actSlot == 1) digitalWrite (led_map[0],LOW); else digitalWrite (led_map[0],HIGH); 
   if (actSlot == 2) digitalWrite (led_map[1],LOW); else digitalWrite (led_map[1],HIGH); 
   if (actSlot == 3) digitalWrite (led_map[2],LOW); else digitalWrite (led_map[2],HIGH); 
}


int freeRam ()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
