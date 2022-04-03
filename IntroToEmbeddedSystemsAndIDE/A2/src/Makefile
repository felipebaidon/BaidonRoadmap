#******************************************************************************
# Copyright (C) 2017 by Alex Fosdick - University of Colorado
#
# Redistribution, modification or use of this software in source or binary
# forms is permitted as long as the files maintain this copyright. Users are 
# permitted to modify this and use it to learn about the field of embedded
# software. Alex Fosdick and the University of Colorado are not liable for any
# misuse of this material. 
#
#*****************************************************************************

#------------------------------------------------------------------------------
# <Put a Description Here>
#
# Use: make [TARGET] [PLATFORM-OVERRIDES]
#
# Build Targets:
#      This Makefile supports the compilation processfor the Host Machine and
#	the MSP432 microcontroller
#
# Platform Overrides:
#      Host machine and MSP432
#
#------------------------------------------------------------------------------
include sources.mk

# Platform Overrides
PLATFORM =

# Architectures Specific Flags
LINKER_FILE = msp432p401r.lds
CPU = -mcpu=cortex-m4
ARCH = -march=armv7e-m
SPECS = --specs=nosys.specs

# Compiler Flags and Defines

ifeq ($(PLATFORM),HOST)
	CC = gcc
else
	CC = arm-none-eabi-gcc
endif   
GENFLAGS = -g -O0 -std=c99
MSPFLAGS = $(CPU) -mthumb $(ARCH)
LDFLAGS = -T
ifeq ($(PLATFORM),HOST)
	CFLAG = $(GENFLAGS) -DHOST
else
	CFLAG = $(GENFLAGS) $(MSPFLAGS) -DMSP432
endif
 
CPPFLAGS = -Wall -Werror
 
OBJS = $(SOURCES:.c=.o)
ASMS = $(SOURCES:.c=.s)
PPFS = $(SOURCES:.c=.i)

%.i: %.c
	$(CC) $(CPPFLAGS) $(CFLAG) $(INCLUDES)  -E $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAG) $(INCLUDES) -c $^

%.asm: %.c
	$(CC) $(CPPFLAGS) $(CFLAG) $(INCLUDES)  -S $^


.PHONY: preprocess-all
preprocess-all:
	$(CC) $(CPPFLAGS) $(CFLAG) $(INCLUDES) -E $(SOURCES)

.PHONY: assemble-all
assemble-all:
	$(CC) $(CPPFLAGS) $(CFLAG) $(INCLUDES) -S $(SOURCES)

.PHONY: compile-all
compile-all:
	$(CC) $(CPPFLAGS) $(CFLAG) $(INCLUDES) -c $(SOURCES)
.PHONY: Build
Build: compile-all
ifeq  ($(PLATFORM),MSP432)
	$(CC) $(OBJS) $(SPECS) -o C1M2.out -Wl,$(LDFLAGS)$(LINKER_FILE)
else ifeq ($(PLATFORM),HOST)
	$(CC) $(OBJS) -o C1M2.out
endif

.PHONY: clean
clean:
	rm -f $(OBJS) $(ASMS) $(PPFS) main.out
