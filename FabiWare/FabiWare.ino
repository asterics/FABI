  
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


#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
//#include "SSD1306Ascii.h"
//#include "SSD1306AsciiWire.h"
//#include <U8g2lib.h>

#include <WS2812.h>
//#include <Adafruit_NeoPixel.h>
 
#include "ssd1306.h"

// #include "nano_gfx.h"



// global variables

#define SIP_BUTTON    9
#define PUFF_BUTTON  10
#define PRESSURE_SENSOR_PIN A0

//BUZZER
#define BEEP_duration 25     // actual duration of beep = BEEP_duration * loop duration; e.g. 100 * 5ms = 500ms

//NEOPixel
#define PIXELS_NUMBER 1   // number of actual NeoPixels on the board
#define PIXELS_PIN 15     // Input Pin for NeoPixels

#define PCB_checkPin 14


#define I2C_ADDRESS 0x3C

//SSD1306AsciiWire oled;


//U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXELS_NUMBER, PIXELS_PIN, NEO_GRB + NEO_KHZ800);


#ifdef TEENSY
  int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS]={16,17,18,19,20,21,22,23,24};  //  map physical button pins to button index 0,1,2  
  int8_t  led_map[NUMBER_OF_LEDS]={1,2,3};                //  maps leds pins   
  uint8_t LED_PIN = 6;                                    //  Led output pin
#endif

#ifdef ARDUINO_PRO_MICRO
  int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS]={2,3,4,5,6,7,8,9,10};
  int8_t  input_map_PCB[NUMBER_OF_PHYSICAL_BUTTONS]={10,16,19,5,6,7,8,9};
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

uint16_t beepTime = BEEP_duration*2;       // beepTime also includes the time without sound after beep 
uint8_t beepCounter = 0;       // how often to beep
uint8_t PCBversion = 0;

uint8_t neoPix_Brightness = 100;
int8_t dimmLEDcounter = neoPix_Brightness;
uint8_t neoPix_r = 0;
uint8_t neoPix_g = 0;
uint8_t neoPix_b = 0;
uint8_t neoPix_r_old = 0;
uint8_t neoPix_g_old = 0;
uint8_t neoPix_b_old = 0;

int inByte=0;
uint16_t pressure=0;
uint8_t reportRawValues = 0;

uint8_t cnt =0,cnt2=0;

uint8_t buzzerPIN = 0;

WS2812 pixels(1); 
cRGB pixColor;


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

  delay(1000);
  
  Serial.begin(9600);
  
  delay(1000);
  
  //while (! Serial);
    
   
   #ifdef DEBUG_OUTPUT  
     Serial.println("Flexible Assistive Button Interface started !");
   #endif

   #ifdef ARDUINO_PRO_MICRO   // only needed for Arduino, automatically done for Teensy(duino)
     Mouse.begin();
     Keyboard.begin();
     TXLED1;    //turn on TX_LED 
   #endif  

   //check if PCB or old (floating wire) FABI is used (checkPin to ground = PCB):
   pinMode(PCB_checkPin, INPUT_PULLUP);
   if(!digitalRead(PCB_checkPin)){              //PCB Version detected
     // Serial.println("FABi PCB Version");

    //Serial.end();      // end USB Serial; causing problems if both active!
     
     /*Serial1.begin(9600);   //start HW UART on TX and RX Pins
     while (! Serial1);
*/
      PCBversion = 1;
      memcpy(input_map, input_map_PCB, NUMBER_OF_PHYSICAL_BUTTONS+1);

      buzzerPIN = 4;
      pinMode(buzzerPIN, OUTPUT);

/*
      u8g2.begin();
      
      u8g2.clearBuffer(); // clear the internal menory
      u8g2.drawXBMP(36, 0, 55, 32, FABIlogo);
      u8g2.sendBuffer(); // transfer internal memory to the display
      */

      //Display: Ascii
     /*
      Wire.begin();
      Wire.setClock(400000L);
      oled.begin(&Adafruit128x32, I2C_ADDRESS);
      oled.setFont(Arial_bold_14); // choose font
      oled.setLetterSpacing(2);
      oled.clear(); 
*/

   


      delay(1000);  

      //setBeepCount(2);
      

      //NeoPixel:
      pixels.setOutput(PIXELS_PIN); 
      pixels.setColorOrderGRB();
     
      pixColor.r = 0; pixColor.g = 0; pixColor.b = 0; // RGB Value
      pixels.set_crgb_at(0, pixColor);
      pixels.sync(); // Sends the value to the LED


      //Pixel & buzzer startup sequenc:
      for(uint8_t i = 0; i < 60; i++){
        pixColor.r = i; 
        pixels.set_crgb_at(0, pixColor);  
        pixels.sync();
        
         
        delay(10);
      }

      //Display:
       /* Select the font to use with menu and all font functions */
      //ssd1306_setFixedFont(ssd1306xled_font6x8);
      ssd1306_setFixedFont(ssd1306xled_font8x16);
      //ssd1306_setFreeFont(free_calibri11x12);
      //ssd1306_print("FABI123");

      ssd1306_128x32_i2c_init();

      ssd1306_clearScreen();

      //ssd1306_print("Normal text");
      ssd1306_drawXBitmap(36, 0, 55, 32, FABIlogo1);

      
      digitalWrite(buzzerPIN, HIGH);


      for(uint8_t i = 60; i > 50; i--){
        pixColor.r = i; 
        pixels.set_crgb_at(0, pixColor);  
        pixels.sync();
        delay(10);
      }
      digitalWrite(buzzerPIN, LOW);

      neoPix_r = 1;

     

      
/* 
      ssd1306_clearScreen();
      ssd1306_printFixed(0, 0, "FABI", STYLE_BOLD);
      ssd1306_printFixed(0, 20, "Button Interface", STYLE_NORMAL);
*/
      
      /*
      pixels.begin();
      for(uint8_t i = 0; i < 50; i++){
        pixels.setPixelColor(0, pixels.Color(i, 0, 0));  
        pixels.show();
        if(i == 30)
         digitalWrite(buzzerPIN, HIGH);
        delay(10);
      }
      digitalWrite(buzzerPIN, LOW);
      for(uint8_t i = 50; i > 50; i--){
        pixels.setPixelColor(0, pixels.Color(i, 0, 0));  
        pixels.show();
        delay(5);
      }
*/
      

     
      //pixels.setPixelColor(1, pixels.Color(15, 0, 0));  
      //pixels.show();


      //Serial1.println("AT");

      delay(1000);  
/*
      Serial1.end();
*/

      Serial1.begin(9600);
      //while(!Serial1);

      

     // Serial.write("AT SA newSlot\n");

      
   }
   else{   
    pinMode(LED_PIN,OUTPUT);
    
    for (int i=0; i<NUMBER_OF_LEDS; i++)   // initialize physical buttons and bouncers
       pinMode (led_map[i], OUTPUT);   // configure the pins for input mode with pullup resistors
   }


   for (int i=0; i<NUMBER_OF_PHYSICAL_BUTTONS; i++)   // initialize physical buttons and bouncers
        pinMode (input_map[i], INPUT_PULLUP);   // configure the pins for input mode with pullup resistors

   for (int i=0; i<NUMBER_OF_BUTTONS; i++)   // initialize button array
   {
      buttons[i].mode=CMD_PL;              // default command for every button is left mouse click
      buttons[i].value=0;
      keystringBuffer[i]=0;
   }


   initDebouncers(); 

   readFromEEPROM(0);  // read button modes from first EEPROM slot      if available !  
   BlinkLed();
   #ifdef DEBUG_OUTPUT  
     Serial.print(F("Free RAM:"));  Serial.println(freeRam());
   #endif


   if(PCBversion){
     writeSlot2Display();

     /*
     oled.clear();
     oled.println("Slot 1:");
     oled.print(settings.slotname);*/
   }


    //deleteSlots();
   
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
/*
      //Serial.println("hello world");
 */

      pressure = analogRead(PRESSURE_SENSOR_PIN);

      //Serial.println(pressure);

      if(!PCBversion){
        while (Serial.available() > 0) {
          // get incoming byte:
            inByte = Serial.read();
            parseByte (inByte);      // implemented in parser.cpp
        }
      }
      else if(PCBversion == 1){
        while (Serial.available() > 0) {
          // get incoming byte:
            inByte = Serial.read();
            parseByte (inByte);      // implemented in parser.cpp
        }
        /*
        while (Serial1.available() > 0) {
          // get incoming byte:
            inByte = Serial1.read();
            parseByte (inByte);      // implemented in parser.cpp
        }

        */
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

        
 
      // if any changes were made, updataaae the Mouse buttons
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
    
    

/*
    if (reportRawValues)   { 
       if (valueReportCount++ > 10) {      // report raw values !
           Serial.print("VALUES:");Serial.print(pressure);Serial.println(",");  
        valueReportCount=0; 
      }
    }*/



    if(PCBversion){
      if(beepCounter > 0){                  //tone
        if(beepTime > BEEP_duration){
          digitalWrite(buzzerPIN, HIGH);
          beepTime--; 
        }
        else if(beepTime > 0){                //no tone
          digitalWrite(buzzerPIN, LOW);
          beepTime--;
        }
        else{
          beepTime = BEEP_duration*2;
          beepCounter--;
        }
      }

      if(dimmLEDcounter < neoPix_Brightness){
        if(dimmLEDcounter < 0)
        {
          pixColor.r = neoPix_r_old*((0-dimmLEDcounter)/2);
          pixColor.g = neoPix_b_old*((0-dimmLEDcounter)/2);
          pixColor.b = neoPix_g_old*((0-dimmLEDcounter)/2);
          //pixels.setPixelColor(0, pixels.Color(neoPix_r_old*((0-dimmLEDcounter)/2), neoPix_b_old*((0-dimmLEDcounter)/2), neoPix_g_old*((0-dimmLEDcounter)/2)));
        }
        else{
          pixColor.r = neoPix_r*(dimmLEDcounter/2);
          pixColor.g = neoPix_b*(dimmLEDcounter/2);
          pixColor.b = neoPix_g*(dimmLEDcounter/2);
          //pixels.setPixelColor(0, pixels.Color(neoPix_r*(dimmLEDcounter/2), neoPix_b*(dimmLEDcounter/2), neoPix_g*(dimmLEDcounter/2)));
        }
        pixels.set_crgb_at(0, pixColor);
        pixels.sync();
        //pixels.show();
        dimmLEDcounter++;
      }
      


    }
    

  
       
   // UpdateLeds();
    


   /*
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
*/
  

  delay(waitTime); 
   
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

void setBeepCount(uint16_t count){
  beepCounter = count;
}

void updateSlot(uint8_t newSlotNumber){

  neoPix_r_old = neoPix_r;
  neoPix_g_old = neoPix_g;
  neoPix_b_old = neoPix_b;

/*
  neoPix_r = (newSlotNumber%2);
  neoPix_g = ((newSlotNumber-1)%2);
  neoPix_b = (newSlotNumber%3)*2;
*/

  switch(newSlotNumber){
    case 1: 
      neoPix_r = 1; neoPix_g = 0; neoPix_b = 0;
      break;
    case 2:
      neoPix_r = 0; neoPix_g = 1; neoPix_b = 0;
      break;
    case 3:
      neoPix_r = 0; neoPix_g = 0; neoPix_b = 1;
      break;
    case 4:
      neoPix_r = 1; neoPix_g = 1; neoPix_b = 0;
      break;
    default:
      neoPix_r = 1; neoPix_g = 0; neoPix_b = 1;
      break;
  }

  dimmLEDcounter = -neoPix_Brightness;
}


//beep 'numberOFbeeps' times to give some audio feedback 
void beepXtimes(uint8_t numberOFbeeps){        
  if(buzzerPIN){  
    for(uint8_t i = 0; i < numberOFbeeps; i++){
       digitalWrite(buzzerPIN, HIGH);
       _delay_ms(125);
       digitalWrite(buzzerPIN, LOW);
       _delay_ms(125);
    }
  }
} 

//write given text to OLED display 
void write2Display(const char* text, uint8_t newLine){
/*
  oled.clear();
  if(newLine)
    oled.println(text);
  else
    oled.print(text);
*/
  
}

void writeSlot2Display(){ 
  ssd1306_clearScreen();

  ssd1306_printFixed(0, 0, "Slot:", STYLE_NORMAL);
  ssd1306_printFixed(0, 20, settings.slotname, STYLE_BOLD);

  /*
   oled.clear();
   oled.print("Slot ");
   oled.print(actSlot);
   oled.println(":");
   oled.print(settings.slotname);
*/
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
       if (buttonDebouncers[i].pressCount<=settings.tt>>2)
          buttonDebouncers[i].pressCount++;           
       if (buttonDebouncers[i].pressCount==settings.ap) {
           handlePress(i);           
           buttonDebouncers[i].pressState=BUTTONSTATE_SHORT_PRESSED;           
       }
       if ((buttonDebouncers[i].pressCount==settings.tt>>2) && (settings.tt<5000) && (l>=0) && (l<NUMBER_OF_BUTTONS)) {
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
