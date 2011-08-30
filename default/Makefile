###############################################################################
# Makefile for the project joesTriac
###############################################################################

## General Flags
PROJECT = joesTriac
MCU = atmega644p
TARGET = joesTriac.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -O0 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += -Wl,-u,vfprintf-Wl,--defsym=__stack=0x800 -Wl,-Map=joesTriac.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Libraries
LIBS = -lm -lprintf_flt 

## Objects that must be built in order to link
OBJECTS = joesTriac.o st7565r.o stateclass.o TriacIntr.o TriacKeyPad.o triacPID.o triacUI.o TStatechart.o TriacDefines.o twi_master.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) joesTriac.hex joesTriac.eep joesTriac.lss size

## Compile
joesTriac.o: ../joesTriac.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

st7565r.o: ../st7565r.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

stateclass.o: ../stateclass.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

TriacIntr.o: ../TriacIntr.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

TriacKeyPad.o: ../TriacKeyPad.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

triacPID.o: ../triacPID.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

triacUI.o: ../triacUI.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

TStatechart.o: ../TStatechart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

TriacDefines.o: ../TriacDefines.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

twi_master.o: ../twi_master.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) joesTriac.elf dep/* joesTriac.hex joesTriac.eep joesTriac.lss joesTriac.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

