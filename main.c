#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#define F_CPU 16000000UL

// #include "lib/twi.h"
#include "lib/usart.h"
// #include "lib/lcd.h"

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define bitSet(reg, ind) (reg |= (1 << ind))
#define bitClear(reg, ind) (reg &= ~(1 << ind))
#define bitCheck(reg, ind) ((reg >> ind) & 1)

#define DHT11_DATA_DDR DDRD
#define DHT11_DATA_PIN PD5

typedef struct {
    uint8_t integral_rh;
    uint8_t decimal_rh;
    uint8_t integral_t;
    uint8_t decimal_t;
    uint8_t checksum;
} dht11_data;

dht11_data data;

void init_dht11(void);
unsigned char read_from_dht11(void);
void read_dht11_byte(uint8_t *output);

void init_dht11(void) {
    bitSet(DHT11_DATA_DDR, DHT11_DATA_PIN); // set pin to output
    bitClear(PORTD, DHT11_DATA_PIN); // pull to low for 18ms
    _delay_ms(18);
    bitSet(PORTD, DHT11_DATA_PIN); // pull to high for 40us
    _delay_us(40);
    bitClear(DHT11_DATA_DDR, DHT11_DATA_PIN); // set pin to input
}

unsigned char read_from_dht11(void) {
    int i, j;

    init_dht11();

    // check for low response from dht11
    if(bitCheck(PIND, DHT11_DATA_PIN)) {
        usart_write_string("here\r\n");
        return 1;
    }
    _delay_us(80);

    // check for high response from dht11
    if(!bitCheck(PIND, DHT11_DATA_PIN)) {
        usart_write_string("here2\r\n");
        return 1;
    }
    _delay_us(80);

    read_dht11_byte(&data.integral_rh);
    read_dht11_byte(&data.decimal_rh);
    read_dht11_byte(&data.integral_t);
    read_dht11_byte(&data.decimal_t);
    read_dht11_byte(&data.checksum);

    // verify the checksum
    if((unsigned char)(data.integral_rh + data.decimal_rh + data.integral_t + data.decimal_t) != data.checksum) {
        return 1;
    }

    return 0;
}

void read_dht11_byte(uint8_t *output) {
    int i;
    for(i = 0; i < 8; i++) {
        while (!bitCheck(PIND, DHT11_DATA_PIN));
        _delay_us(30);
        if(bitCheck(PIND, DHT11_DATA_PIN)) {
            *output |= (1 << (7 - i));
        }
        while (bitCheck(PIND, DHT11_DATA_PIN));
    }
}

int main(void) {
    // usart
    usart_init();

    while (1) {
        if(read_from_dht11()) {
            usart_write_string("\r\nerror reading dht11");
        }

        float humidity = (float)data.integral_rh + (float)data.decimal_rh / 10.0;
        float temp = (float)data.integral_t + (float)data.decimal_t / 10.0;

        usart_write_string("\r\ntemp: ");
        usart_write_float(temp);
        usart_write_string("\r\nrelative humidity: ");
        usart_write_float(humidity);
        _delay_ms(2e3);
    }
}
