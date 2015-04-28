################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../joesTriac.c \
../st7565r.c \
../stateclass.c \
../TriacIntr.c \
../TriacKeyPad.c \
../triacPID.c \
../triacUI.c \
../TStatechart.c \
../TriacDefines.c \
../twi_master.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
joesTriac.o \
st7565r.o \
stateclass.o \
TriacIntr.o \
TriacKeyPad.o \
triacPID.o \
triacUI.o \
TStatechart.o \
TriacDefines.o \
twi_master.o

OBJS_AS_ARGS +=  \
joesTriac.o \
st7565r.o \
stateclass.o \
TriacIntr.o \
TriacKeyPad.o \
triacPID.o \
triacUI.o \
TStatechart.o \
TriacDefines.o \
twi_master.o

C_DEPS +=  \
joesTriac.d \
st7565r.d \
stateclass.d \
TriacIntr.d \
TriacKeyPad.d \
triacPID.d \
triacUI.d \
TStatechart.d \
TriacDefines.d \
twi_master.d

C_DEPS_AS_ARGS +=  \
joesTriac.d \
st7565r.d \
stateclass.d \
TriacIntr.d \
TriacKeyPad.d \
triacPID.d \
triacUI.d \
TStatechart.d \
TriacDefines.d \
twi_master.d

OUTPUT_FILE_PATH +=joesTriac.elf

OUTPUT_FILE_PATH_AS_ARGS +=joesTriac.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler





















./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega644p -c -gdwarf-2 -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="joesTriac.map" -Wl,--start-group -Wl,-lprintf_flt -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -Wl,--defsym=__stack=0x800 -mmcu=atmega644p  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "joesTriac.elf" "joesTriac.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "joesTriac.elf" "joesTriac.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "joesTriac.elf" > "joesTriac.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "joesTriac.elf" "joesTriac.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-size.exe" "joesTriac.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "joesTriac.elf" "joesTriac.a" "joesTriac.hex" "joesTriac.lss" "joesTriac.eep" "joesTriac.map" "joesTriac.srec" "joesTriac.usersignatures"
	