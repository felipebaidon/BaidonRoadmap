

SOURCES = *.c
INCLUDES = *.h

CC= arm-none-eabi-gcc
CFLAGS = -Wall -Werror -O0 -g 
ARCH = armv7e-m 
SPECS = nosys.specs
CPU =  cortex-m4


.PHONY: compile-all
compile-all:
	$(CC) $(CFLAGS) -march=$(ARCH) -mcpu=$(CPU) -mthumb --specs=$(SPECS) $(SOURCES) $(INCLUDES) -c $^
.PHONY:build 
build: 
	$(CC) $(CFLAGS) -march=$(ARCH) -mcpu=$(CPU) -mthumb --specs=$(SPECS)  $(SOURCES) $(INCLUDES)\
 -o main.out -Wl,-Map=main.map

.PHONY: clean
clean:
	rm -f main.out main.map	
