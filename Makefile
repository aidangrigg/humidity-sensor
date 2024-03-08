TARGET=main
PORT=/dev/ttyACM0

AVRDUDE=avrdude

LIB_OBJS= obj/usart.o obj/twi.o obj/lcd.o

CC=avr-gcc
OBJ2HEX=avr-objcopy

MCU=atmega328p
F_CPU=16000000L

AVRDUDE_FLAGS = -F -V -c arduino -p atmega328p -P $(PORT) -b 115200
AVRDUDE_WRITE_FLASH = -U flash:w:obj/$(TARGET).hex

CCFLAGS = -Os -Wall -g -mmcu=$(MCU) -c

# flashing
program : $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

# convert to hex
$(TARGET).hex : $(TARGET)
	$(OBJ2HEX) -j .text -j .data -O ihex obj/$(TARGET) obj/$(TARGET).hex

# linking
$(TARGET) : $(TARGET).o $(LIB_OBJS)
	$(CC) -mmcu=atmega328p -o obj/$(TARGET) obj/$(TARGET).o $(LIB_OBJS)

# compile source
$(TARGET).o : $(TARGET).c
	$(CC) $(CCFLAGS) -o obj/$(TARGET).o $(TARGET).c

# compile libraries
obj/usart.o : ./lib/usart.c ./lib/usart.h
	$(CC) -c ./lib/usart.c -o ./obj/usart.o

obj/lcd.o : ./lib/lcd.c ./lib/lcd.h
	$(CC) -Os -c ./lib/lcd.c -o ./obj/lcd.o

obj/twi.o : ./lib/twi.c ./lib/twi.h
	$(CC) -c ./lib/twi.c -o ./obj/twi.o

clean:
	rm -r obj/*
