# --
# Copyright (c) 2016, Lukasz Marcin Podkalicki <lpodkalicki@gmail.com>
# --

MCU=atmega328p
FUSE_L=0xE2
FUSE_H=0xD1
F_CPU=8000000
CC=avr-g++
LD=avr-ld
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude
CFLAGS=-std=c++11 -Wall -g -Os -gdwarf-2 -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=main

SRCS = main.cpp

all:
	${CC} ${CFLAGS} -o ${TARGET}.o ${SRCS}
	${LD} -o ${TARGET}.elf ${TARGET}.o
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.o ${TARGET}.hex
	${OBJCOPY} -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex ${TARGET}.elf ${TARGET}.eep
	${SIZE} -C --mcu=${MCU} ${TARGET}.elf

flash:
	${AVRDUDE} -p ${MCU} -c stk500v1 -P COM13 -b 19200 -V -U flash:w:${TARGET}.hex:i

fuse:
	$(AVRDUDE) -p ${MCU} -c stk500v1 -P COM13 -b 19200 -U hfuse:w:${FUSE_H}:m -U lfuse:w:${FUSE_L}:m

clean:
	rm -f *.c~ *.o *.elf *.hex *.eep
