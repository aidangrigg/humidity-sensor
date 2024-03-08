#ifndef _TWI_H
#define _TWI_H

#include <stdint.h>
#include <stdlib.h>

void twi_init(void);
unsigned char getTWSR(void);
unsigned char twi_start(void);
void twi_send(uint8_t addr, uint8_t *data, size_t size);

#endif // !_TWI_H
