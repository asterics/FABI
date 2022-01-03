/* 
     Flexible Assistive Button Interface (FABI) - AsTeRICS Foundation - http://www.asterics-foundation.org
     for controlling HID functions via momentary switches and/or serial AT-commands  
     More Information: https://github.com/asterics/FABI

     Module: toneFABI.cpp - tone generation / buzzer control
        
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, see:
     http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include <Arduino.h>
#include "toneFABI.h"


static volatile uint32_t ovf_counter = 0;
uint16_t freqtable[] = {523, 587, 659, 698, 784, 880, 988, 1047};

/**
	@name ISR_TIMER3_COMPA
	@param none
	@return none
	
	In CTC mode this is our overflow interrupt. We will clear the
	pin and if necessary stop the timer
*/
ISR(TIMER3_COMPA_vect) {
	PORTD &= ~(1<<PD4); //clear bit 4 of PORTD (buzzer)
	ovf_counter--;
	if(ovf_counter == 0) //finished
	{
		TCCR3B = 0; //disable timer
	}
}

/**
	@name ISR_TIMER3_COMPB
	@param none
	@return none
	
	OCR3A is set to half the wavelength; the buzzer pin is set here.
	@warning This ISR is declared naked, because we only set one bit
	we will use ASM.
*/
ISR(TIMER3_COMPB_vect)
{
  PORTD |= (1<<PD4);
}

/*
ISR(TIMER3_COMPB_vect, ISR_NAKED) {
	//set PD4
	///@note The sbi instruction is usable for registers <0x1F, therefor
	///we cannot use any defines (e.g. PORTD -> 0x2B)
	asm volatile("sbi 0x0B,4");
  reti();
	__builtin_unreachable(); //according to https://www.avrfreaks.net/forum/isr-naked-question
}*/

void toneFABI(uint16_t frequency, uint16_t time)
{
  //calculate the count of overflows, which is the frequency in [Hz]
  //multiplied by the time it should run, but in [ms] -> divide by 1k
  
  if ((frequency>0) && (frequency<=8))   // select one note of the tempered scale
     frequency = freqtable[frequency-1];
  
	uint32_t temp = ((uint32_t)frequency * (uint32_t)time) / 1000;
  //enclose the copy in an atomic block
  cli();
  ovf_counter = temp;
  sei();
  //determine the OCR values, we have prescaler 64 -> 250kHHz
  //divide by the desired frequency to get the timer ticks
  OCR3A = (uint16_t)((uint32_t)250000 / (uint32_t)frequency); 
  OCR3B = OCR3A / 2; //50% DC
  //set FastPWM mode (TOP -> OCR3A), enable prescaler
  TCCR3B = (1<<WGM33)|(1<<WGM32)|(1<<CS31)|(1<<CS30);
}

/**
   @name initBuzzer
   @param none
   @return none

   initialises the buzzer pin and creates initial tone
*/
void initBuzzer() {
  DDRD &= ~(1<<PD4);
  TIMSK3 = (1<<OCIE3A)|(1<<OCIE3B);
  TCCR3A = (1<<WGM31)|(1<<WGM30);
  sei();
  toneFABI(1,100);
  delay(300);
  DDRD |= (1<<PD4);
  toneFABI(1,200);  delay(200);
  toneFABI(3,200);  delay(200);
  toneFABI(5,100);

}
