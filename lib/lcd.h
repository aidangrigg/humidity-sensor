#ifndef _LCD_H
#define _LCD_H

#ifndef LCD_SLAVE_ADDR
#define LCD_SLAVE_ADDR 0x27
#endif // !LCD_SLAVE_ADDR

#define En 0b00000100
#define LCD_BACKLIGHT   0x08  // On

#define LCD_CHR  1 // Mode - Sending data
#define LCD_CMD  0 // Mode - Sending command

enum lcd_line {
  LINE1 = 0x80,
  LINE2 = 0xC0
};

/**
 * @brief Initialises the LCD for use.
 */
void lcd_init();

/**
 * @brief Internal function that toggles the enable pin on the lcd. Used to in
 * {@function lcd_send}
 *
 * @param bits to be sent.
 */
void lcd_toggle_enable(char bits);

/**
 * @brief Sends a command or data to the LCD.
 *
 * @param bits included in the command/lcd
 * @param mode R/s bit of the message being sent. 1 is sending data, 0
 * is sending a command.
 */
void lcd_send(int bits, int mode);

/**
 * @brief Sends the clear instruction to the lcd.
 */
void lcd_clear(void);

/**
 * @brief Sets the cursor to the begining of a line.
 *
 * @param line cursor is being brought to
 */
void lcd_set_cursor(enum lcd_line line);

/**
 * @brief Writes a single character to the current position of the cursor
 * on the display.
 *
 * @param val the character being sent.
 */
void lcd_write_char(char val);

/**
 * @brief Writes a string to the display
 *
 * @param s the string being written to the display.
 */
void lcd_write(const char *s);

/**
 * @brief Writes a float to the display at the current position of the cursor.
 *
 * @param f float being written
 * @param dec decimal places
 */
void lcd_write_float(float f, int dec);

#endif // !_LCD_H
