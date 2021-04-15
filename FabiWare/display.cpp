
#include "fabi.h"
#include "display.h"

/*  initialize Display and display FABI Logo  */
void initDisplay(){
       /* Select the font to use with menu and all font functions */
      //ssd1306_setFixedFont(ssd1306xled_font6x8);
      ssd1306_setFixedFont(ssd1306xled_font8x16);

      ssd1306_128x32_i2c_init();
      ssd1306_clearScreen();

      //ssd1306_print("Normal text");
      ssd1306_drawXBitmap(36, 0, 55, 32, FABIlogo1);
}

void writeSlot2Display(){ 
  ssd1306_clearScreen();

  ssd1306_printFixed(0, 0, "Slot:", STYLE_NORMAL);
  ssd1306_printFixed(0, 20, settings.slotname, STYLE_BOLD);
}