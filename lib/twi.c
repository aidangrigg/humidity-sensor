#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#include "avr/sfr_defs.h"
#endif

#include "twi.h"

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

#define bitSet(reg, ind) (reg |= (1 << ind))
#define bitClear(reg, ind) (reg &= ~(1 << ind))
#define bitCheck(reg, ind) ((reg >> ind) & 1)

void twi_init(void) {
  TWBR = 8; // set TWI prescalar
  bitSet(TWCR, TWEN); // enable TWI
}

unsigned char getTWSR(void) {
  return TWSR & (~3);
}

/**
 * @brief Try to become the master of the TWI wire.
 *
 * @return 1 if success, 0 if fail
 */
unsigned char twi_start(void) {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
  while (!bitCheck(TWCR, TWINT));

  return getTWSR() == 0x08;
}

void twi_send(uint8_t addr, uint8_t *data, size_t size) {
  char flag_status = 0;
  twi_init();

  // become the master on the wire
  do {
    flag_status = twi_start();
  } while (!flag_status);

  // write the address bit
  TWDR = addr << 1 | 0;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!bitCheck(TWCR, TWINT));

  // write the date bytes
  int i;
  for (i = 0; i < size; i++) {
    TWDR = data[i];
    TWCR = (1 << TWINT) | (1 << TWEN) | 1 << TWEA;
    while (!bitCheck(TWCR, TWINT));
  }

  // send the stop signal
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}
