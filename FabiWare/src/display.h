/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: display.h - implementation of the Oled display functions 

     Note: Oled display connects to I2C-1 (Wire) for FlipMouse
     The utilized library is https://github.com/greiman/SSD1306Ascii

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/**
   @name initDisplay
   @brief initalizes the SSD1306 LCD using I2C interface Wire0 or Wire1
   @return true if LCD could be initialized, false if not
*/
uint8_t initDisplay();

/**
   @name isDisplayAvailable
   @brief returns true if LC Display is available at interface Wire0 or Wire1
   @return true if LCD was initialized, false if not
*/
uint8_t isDisplayAvailable();


/**
   @name displayClear
   @brief clears content of SSD1306 LCD
   @return none
*/
void displayClear(void);

/**
   @name displayUpdate
   @brief clears content of SSD1306 LCD according to slot settings
   @return none
*/
void displayUpdate(void);

/**
   @name pauseDisplayUpdates
   @param pause: if true, display updates will be paused
   @brief allows to bypass display updates
   @return none
*/
void pauseDisplayUpdates(uint8_t pause);


/**
   @name displayUpdate
   @brief displays a message on the SSD1306 LCD 
   @param msg message string 
   @return none
*/
void displayMessage(char * msg);

/**
   @name batteryDisplay
   @param refresh: if false (default), only changes will be drawn
   @brief Display the battery status/percentage
   @return none
*/
void batteryDisplay(bool refresh=false);


//  a modified version of the Ardfruit 5x7 font (SSD1306Ascii library)
//  with icons for the battery state:
#define GLCDFONTDECL(_n) static const uint8_t _n[]
// standard ascii 5x7 font
// restricted to 96 characters
GLCDFONTDECL(Adafruit5x7_mod) = {
  0x0, 0x0, // size of zero indicates fixed width font,
  0x05, // width
  0x07, // height
  0x20, // first char
  0x60, // char count
	0x00, 0x00, 0x00, 0x00, 0x00,   // starting with 0x20 -> Space character
	0x3f, 0x21, 0x21, 0x21, 0x21,   // ! modified for battery icon1 
	0x21, 0x21, 0x21, 0x21, 0x21,   // " modified for battery icon2
	0x21, 0x21, 0x3f, 0x1e, 0x1e,   // # modified for battery icon3
	0x3f, 0x3f, 0x3f, 0x3f, 0x21,   // $ modified for battery icon4
	0x23, 0x13, 0x08, 0x64, 0x62,   // kept % sign (useful for percent display)!
	0x3f, 0x3f, 0x3f, 0x1e, 0x1e,   // & modified for battery icon5
	0x00, 0x08, 0x07, 0x03, 0x00,
	0x00, 0x1C, 0x22, 0x41, 0x00,
	0x00, 0x41, 0x22, 0x1C, 0x00,
	0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
	0x08, 0x08, 0x3E, 0x08, 0x08,
	0x00, 0x80, 0x70, 0x30, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x00, 0x00, 0x60, 0x60, 0x00,
	0x20, 0x10, 0x08, 0x04, 0x02,
	0x3E, 0x51, 0x49, 0x45, 0x3E,
	0x00, 0x42, 0x7F, 0x40, 0x00,
	0x72, 0x49, 0x49, 0x49, 0x46,
	0x21, 0x41, 0x49, 0x4D, 0x33,
	0x18, 0x14, 0x12, 0x7F, 0x10,
	0x27, 0x45, 0x45, 0x45, 0x39,
	0x3C, 0x4A, 0x49, 0x49, 0x31,
	0x41, 0x21, 0x11, 0x09, 0x07,
	0x36, 0x49, 0x49, 0x49, 0x36,
	0x46, 0x49, 0x49, 0x29, 0x1E,
	0x00, 0x00, 0x14, 0x00, 0x00,
	0x00, 0x40, 0x34, 0x00, 0x00,
	0x00, 0x08, 0x14, 0x22, 0x41,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x00, 0x41, 0x22, 0x14, 0x08,
	0x02, 0x01, 0x59, 0x09, 0x06,
	0x3E, 0x41, 0x5D, 0x59, 0x4E,
	0x7C, 0x12, 0x11, 0x12, 0x7C,
	0x7F, 0x49, 0x49, 0x49, 0x36,
	0x3E, 0x41, 0x41, 0x41, 0x22,
	0x7F, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x49, 0x49, 0x49, 0x41,
	0x7F, 0x09, 0x09, 0x09, 0x01,
	0x3E, 0x41, 0x41, 0x51, 0x73,
	0x7F, 0x08, 0x08, 0x08, 0x7F,
	0x00, 0x41, 0x7F, 0x41, 0x00,
	0x20, 0x40, 0x41, 0x3F, 0x01,
	0x7F, 0x08, 0x14, 0x22, 0x41,
	0x7F, 0x40, 0x40, 0x40, 0x40,
	0x7F, 0x02, 0x1C, 0x02, 0x7F,
	0x7F, 0x04, 0x08, 0x10, 0x7F,
	0x3E, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x09, 0x09, 0x09, 0x06,
	0x3E, 0x41, 0x51, 0x21, 0x5E,
	0x7F, 0x09, 0x19, 0x29, 0x46,
	0x26, 0x49, 0x49, 0x49, 0x32,
	0x03, 0x01, 0x7F, 0x01, 0x03,
	0x3F, 0x40, 0x40, 0x40, 0x3F,
	0x1F, 0x20, 0x40, 0x20, 0x1F,
	0x3F, 0x40, 0x38, 0x40, 0x3F,
	0x63, 0x14, 0x08, 0x14, 0x63,
	0x03, 0x04, 0x78, 0x04, 0x03,
	0x61, 0x59, 0x49, 0x4D, 0x43,
	0x00, 0x7F, 0x41, 0x41, 0x41,
	0x02, 0x04, 0x08, 0x10, 0x20,
	0x00, 0x41, 0x41, 0x41, 0x7F,
	0x04, 0x02, 0x01, 0x02, 0x04,
	0x40, 0x40, 0x40, 0x40, 0x40,
	0x00, 0x03, 0x07, 0x08, 0x00,
	0x20, 0x54, 0x54, 0x78, 0x40,
	0x7F, 0x28, 0x44, 0x44, 0x38,
	0x38, 0x44, 0x44, 0x44, 0x28,
	0x38, 0x44, 0x44, 0x28, 0x7F,
	0x38, 0x54, 0x54, 0x54, 0x18,
	0x00, 0x08, 0x7E, 0x09, 0x02,
	0x18, 0xA4, 0xA4, 0x9C, 0x78,
	0x7F, 0x08, 0x04, 0x04, 0x78,
	0x00, 0x44, 0x7D, 0x40, 0x00,
	0x20, 0x40, 0x40, 0x3D, 0x00,
	0x7F, 0x10, 0x28, 0x44, 0x00,
	0x00, 0x41, 0x7F, 0x40, 0x00,
	0x7C, 0x04, 0x78, 0x04, 0x78,
	0x7C, 0x08, 0x04, 0x04, 0x78,
	0x38, 0x44, 0x44, 0x44, 0x38,
	0xFC, 0x18, 0x24, 0x24, 0x18,
	0x18, 0x24, 0x24, 0x18, 0xFC,
	0x7C, 0x08, 0x04, 0x04, 0x08,
	0x48, 0x54, 0x54, 0x54, 0x24,
	0x04, 0x04, 0x3F, 0x44, 0x24,
	0x3C, 0x40, 0x40, 0x20, 0x7C,
	0x1C, 0x20, 0x40, 0x20, 0x1C,
	0x3C, 0x40, 0x30, 0x40, 0x3C,
	0x44, 0x28, 0x10, 0x28, 0x44,
	0x4C, 0x90, 0x90, 0x90, 0x7C,
	0x44, 0x64, 0x54, 0x4C, 0x44,
	0x00, 0x08, 0x36, 0x41, 0x00,
	0x00, 0x00, 0x77, 0x00, 0x00,
	0x00, 0x41, 0x36, 0x08, 0x00,
	0x02, 0x01, 0x02, 0x04, 0x02,
	0x3C, 0x26, 0x23, 0x26, 0x3C,
};
#endif


/*
  here is a GPT-generated font editor for the 5x7 font 
  (save this as "fontedit.py", usage: python fontedit.py)

<-----   python code 

import tkinter as tk
from tkinter import messagebox

# Dimensions and settings
COLS = 5
ROWS = 7
CELL_SIZE = 30
PADDING = 5  # Padding around cells

# Colors for on/off cells
ON_COLOR = "black"
OFF_COLOR = "white"

class DotMatrixEditor(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("5x7 Dot Matrix Editor (Reversed Orientation)")
        
        # Initialize the grid state as a 2D list (ROWS x COLS) with all dots off (0)
        self.matrix = [[0 for _ in range(COLS)] for _ in range(ROWS)]
        
        # Create canvas to display the dot matrix
        self.canvas = tk.Canvas(self, width=COLS*CELL_SIZE+2*PADDING,
                                     height=ROWS*CELL_SIZE+2*PADDING, bg="gray")
        self.canvas.grid(row=0, column=0, padx=10, pady=10, columnspan=COLS)
        self.canvas.bind("<Button-1>", self.on_canvas_click)
        
        # Create Entry widgets for each column's hex value
        self.hex_entries = []
        for col in range(COLS):
            entry = tk.Entry(self, width=5, justify="center")
            entry.grid(row=1, column=col, padx=5, pady=5)
            # Bind events to update the grid when the hex value is changed.
            entry.bind("<Return>", lambda event, c=col: self.update_column_from_entry(c))
            entry.bind("<FocusOut>", lambda event, c=col: self.update_column_from_entry(c))
            self.hex_entries.append(entry)
        
        # Initially update both views
        self.update_all_hex_entries()
        self.draw_matrix()
    
    def draw_matrix(self):
        """Redraw the grid on the canvas based on the current matrix state."""
        self.canvas.delete("all")
        for row in range(ROWS):
            for col in range(COLS):
                x1 = PADDING + col * CELL_SIZE
                y1 = PADDING + row * CELL_SIZE
                x2 = x1 + CELL_SIZE
                y2 = y1 + CELL_SIZE
                color = ON_COLOR if self.matrix[row][col] else OFF_COLOR
                self.canvas.create_rectangle(x1, y1, x2, y2, fill=color, outline="black")
    
    def on_canvas_click(self, event):
        """Toggle the cell corresponding to the canvas click."""
        col = (event.x - PADDING) // CELL_SIZE
        row = (event.y - PADDING) // CELL_SIZE
        if 0 <= col < COLS and 0 <= row < ROWS:
            # Toggle the dot
            self.matrix[row][col] = 0 if self.matrix[row][col] else 1
            # Update the corresponding hex entry for this column
            self.update_hex_entry_for_column(col)
            self.draw_matrix()
    
    def update_hex_entry_for_column(self, col):
        """Compute and update the hex value for a given column with inverted bit mapping.
           Now, bit 0 is the top row and bit 6 is the bottom row.
        """
        value = 0
        for row in range(ROWS):
            # Shift the bit by 'row' instead of (ROWS - 1 - row)
            value |= (self.matrix[row][col] << row)
        # Format as hex (using two hex digits; values are from 0x00 to 0x7F)
        hex_str = f"0x{value:02X}"
        self.hex_entries[col].delete(0, tk.END)
        self.hex_entries[col].insert(0, hex_str)
    
    def update_all_hex_entries(self):
        """Update hex entries for all columns."""
        for col in range(COLS):
            self.update_hex_entry_for_column(col)
    
    def update_column_from_entry(self, col):
        """Update the grid state for a column from the hex value in the entry using inverted bit mapping."""
        text = self.hex_entries[col].get().strip()
        try:
            # Allow entries with or without '0x'
            value = int(text, 16)
        except ValueError:
            messagebox.showerror("Invalid Input", f"Column {col+1}: '{text}' is not a valid hex number.")
            self.update_hex_entry_for_column(col)
            return
        
        if not (0 <= value < 128):
            messagebox.showerror("Invalid Range", f"Column {col+1}: Hex value must be between 0x00 and 0x7F.")
            self.update_hex_entry_for_column(col)
            return
        
        # Update the column bits in the matrix using the inverted mapping.
        for row in range(ROWS):
            # Extract the bit for the current row; now bit 0 corresponds to the top row.
            bit = (value >> row) & 1
            self.matrix[row][col] = bit
        
        # Redraw the matrix after update.
        self.draw_matrix()

if __name__ == "__main__":
    app = DotMatrixEditor()
    app.mainloop()

*/
