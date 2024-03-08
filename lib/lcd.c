#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#include "avr/sfr_defs.h"
#endif

#define F_CPU 16000000UL

#include "lcd.h"
#include "twi.h"

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

#define bitSet(reg, ind) (reg |= (1 << ind))
#define bitClear(reg, ind) (reg &= ~(1 << ind))
#define bitCheck(reg, ind) ((reg >> ind) & 1)

void lcd_init() {
  lcd_send(0x33, LCD_CMD); // Initialise
  lcd_send(0x32, LCD_CMD); // Initialise
  lcd_send(0x06, LCD_CMD); // Cursor move direction
  lcd_send(0x0C, LCD_CMD); // 0x0F On, Blink Off
  lcd_send(0x28, LCD_CMD); // Data length, number of lines, font size

  lcd_clear(); // clear the display
  _delay_us(500);
}

void lcd_toggle_enable(char bits) {
  _delay_us(500);
  uint8_t first, second;
  first = (bits | En);
  second = (bits & ~En);

  twi_send(LCD_SLAVE_ADDR, &first, 1);
  _delay_us(500);
  twi_send(LCD_SLAVE_ADDR, &second, 1);
  _delay_us(500);
}

void lcd_send(int bits, int mode)   {
  //Send byte to data pins
  // bits = the data
  // mode = 1 for data, 0 for command
  uint8_t bits_high, bits_low;
  // uses the two half byte writes to LCD
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

  // High bits
  twi_send(LCD_SLAVE_ADDR, &bits_high, 1);
  lcd_toggle_enable(bits_high);

  // Low bits
  twi_send(LCD_SLAVE_ADDR, &bits_low, 1);
  lcd_toggle_enable(bits_low);
}

void lcd_clear(void)   {
  lcd_send(0x01, LCD_CMD);
  lcd_set_cursor(LINE1);
}

void lcd_set_cursor(enum lcd_line line)   {
  lcd_send(line, LCD_CMD);
}

void lcd_write_char(char val)   {
  lcd_send(val, LCD_CHR);
}

void lcd_write(const char *s)   {
  while ( *s ) lcd_send(*(s++), LCD_CHR);
}

void lcd_write_float(float f, int dec) {
  const char MAX_LEN = 8;
  char *str = (char *)malloc(MAX_LEN);
  dtostrf(f, MAX_LEN, dec, str);
  str[MAX_LEN] = '\0';

  lcd_write(str);
  free(str);
}
