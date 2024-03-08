#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#include "avr/sfr_defs.h"
#endif

#include "usart.h"
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>

void usart_init(void) {
  UBRR0 = UBRR;
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
  UCSR0C = (3 << UCSZ00);
}

void usart_write_char(unsigned char dataChar) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = dataChar;
}

void usart_write_string(char *dataString) {
  while (*dataString != '\0') {
    usart_write_char(*dataString);
    dataString++;
  }
}

void usart_write_float(float number) {
  const char MAX_LEN = 8;
  char *str = (char *)malloc(MAX_LEN);
  dtostrf(number, MAX_LEN, 3, str);
  str[MAX_LEN] = '\0';

  usart_write_string(str);
  free(str);
}

unsigned char usart_read_char(void) {
  unsigned char res1;

  while(!(UCSR0A & (1<<RXC0)));
  res1 = UDR0;

  return res1;
}

void usart_read_string(char buff[], int len) {
  char c;
  int i = 0;

  do {
    c = usart_read_char();
    buff[i++] = c;
  } while ((i < len) && (c != '\r'));


  buff[i] = '\0';
}
