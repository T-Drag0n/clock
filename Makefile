CC      = avr-gcc
CFLAGS += \
    -DF_CPU=16000000ULL \
    -Og \
    -Wall \
    -Wextra \
    -Wpedantic \
    -g \
    -mmcu=atmega328p \
    -std=gnu11 \

.PHONY: all
all: maine.elf

.PHONY: clean
clean:	
	@rm -rvf *.d *.elf *.o

maine.elf: maine.o twi.o rtc.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -MM -MF $*.d $<
	$(CC) $(CFLAGS) -c $<

-include $(wildcard *.d)
