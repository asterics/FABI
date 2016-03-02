  
/* 
     Flexible Assistive Button Interface (FABI)  Version 2.0  - AsTeRICS Academy 2015 - http://www.asterics-academy.net
      allows control of HID functions via momentary switches and/or AT-commands  
   

   requirements:  USB HID capable Arduino (Leonardo / Micro / Pro Micro) - see #define in fabi.h !
                  or Teensy 2.0++ with Teensyduino AddOn setup as USB composite device (Mouse + Keyboard + Serial)
       optional:  Momentary switches connected to GPIO pins / force sensors connected to ADC pins
       
   for a list of supported AT commands, see commands.h / commands.cpp    

*/

#include "fabi.h"        //  Bounce library used for button debouncing
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
    "empty", 3,  1000, 0, 1023        // slotname, wheel step, threshold time (short/longpress), sip threshold, puff threshold
}; 


struct buttonType buttons [NUMBER_OF_BUTTONS];                     // array for all buttons - type definition see fabi.h 
struct buttonDebouncerType buttonDebouncers [NUMBER_OF_BUTTONS];   // array for all buttonsDebouncers - type definition see fabi.h 

int clickTime=DEFAULT_CLICK_TIME;
int waitTime=DEFAULT_WAIT_TIME;

int EmptySlotAddress = 0;
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
char * keystring=0;
char * writeKeystring=0;
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
      buttons[i].keystring[0]=0;
   }


   initDebouncers(); 

   readFromEEPROM(0);  // read button modes from first EEPROM slot if available !  
   BlinkLed();
   if (DebugOutput==1) {  
     Serial.print(F("Free RAM:"));  Serial.println(freeRam());
   }
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
          handleButton(i, -1, digitalRead(input_map[i]) == LOW ? 1 : 0);    

      if (settings.ts>0)    handleButton(SIP_BUTTON, -1, pressure < settings.ts ? 1 : 0); 
      if (settings.tp<1023) handleButton(PUFF_BUTTON, -1, pressure > settings.tp ? 1 : 0);

        
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
    
     // handle Keyboard output (single key press/release is done seperately via setKeyValues() ) 
     if (writeKeystring) {
        sendToKeyboard(writeKeystring);
        writeKeystring=0;
    }    


    if (reportRawValues)   { 
       if (valueReportCount++ > 10) {      // report raw values !
           Serial.print("VALUES:");Serial.print(pressure);Serial.println(",");  
        valueReportCount=0; 
      }
    }
       
    UpdateLeds();
    delay(waitTime);  // to limit move movement speed. TBD: remove delay, use millis() !
}


void handlePress (int buttonIndex)   // a button was pressed
{   
    performCommand(buttons[buttonIndex].mode,buttons[buttonIndex].value,buttons[buttonIndex].keystring,1);
}

void handleRelease (int buttonIndex)    // a button was released
{
   switch(buttons[buttonIndex].mode) {
     case CMD_PL: leftMouseButton=0; break;
     case CMD_PR: rightMouseButton=0; break;
     case CMD_PM: middleMouseButton=0; break;
     case CMD_MX: moveX=0; break;      
     case CMD_MY: moveY=0; break;      
     case CMD_KP: releaseKeys(buttons[buttonIndex].keystring); break; 
   }
}

void initDebouncers()
{
   for (int i=0; i<NUMBER_OF_BUTTONS; i++)   // initialize button array
   {
      buttonDebouncers[i].bounceState=0;
      buttonDebouncers[i].stableState=0;
      buttonDebouncers[i].bounceCount=0;
      buttonDebouncers[i].longPressed=0;
   }
}

void release_all()  // releases all previously pressed keys
{
    Keyboard.releaseAll();
    leftMouseButton=0;
    rightMouseButton=0;
    middleMouseButton=0;
    moveX=0;
    moveY=0;
}


void handleButton(int i, int l, uint8_t state)    // button debouncing and longpress detection  
{                                                 //   (if button i is pressed long and index l>=0, virtual button l is activated !)
   if ( buttonDebouncers[i].bounceState == state) {
     if (buttonDebouncers[i].bounceCount < DEFAULT_DEBOUNCING_TIME) {
       buttonDebouncers[i].bounceCount++;
       if (buttonDebouncers[i].bounceCount == DEFAULT_DEBOUNCING_TIME) {
          if (state != buttonDebouncers[i].stableState)
          { 
            buttonDebouncers[i].stableState=state;
            if (state == 1) { 
              handlePress(i); 
              buttonDebouncers[i].timestamp=millis();
            }
            else {
              if (buttonDebouncers[i].longPressed)
              {
                 buttonDebouncers[i].longPressed=0;
                 handleRelease(l);
              }
              else handleRelease(i);  
            }
          }
       }
     }
     else { 
       if ((millis()-buttonDebouncers[i].timestamp > settings.tt ) && (l>=0))
       {
            if ((state == 1) && (buttonDebouncers[i].longPressed==0) && (buttons[l].mode!=CMD_NC)) {
           buttonDebouncers[i].longPressed=1; 
           handleRelease(i);
           handlePress(l);
          }
       }
     }
   }
   else {
     buttonDebouncers[i].bounceState = state;
     buttonDebouncers[i].bounceCount=0;     
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
