#ifndef _USART_H
#define _USART_H

#ifndef BAUD_RATE
#define BAUD_RATE 9600
#endif // !BAUD_RATE

#ifndef UBRR
#define UBRR 103
#endif // !UBRR

void usart_init(void);
void usart_write_char(unsigned char dataChar);
void usart_write_string(char *dataString);
void usart_write_float(float number);
void usart_read_string(char buff[], int len);
unsigned char usart_read_char(void);

#endif // !_USART_H
