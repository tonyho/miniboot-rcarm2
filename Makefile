.phony: all

all: miniboot.bin

CC  = arm-none-linux-gnueabi-gcc
LD  = arm-none-linux-gnueabi-ld
COPY= arm-none-linux-gnueabi-objcopy
DUMP= arm-none-linux-gnueabi-objdump

miniboot.bin: miniboot.elf
	$(COPY) -O binary $< $@
	$(DUMP) -D $< > dump.log
	
miniboot.elf: *.S *.c
	$(CC) -c -mcpu=cortex-a15 -mlittle-endian -msoft-float -Wall -Os -nostdlib $^
	$(LD) -T miniboot.lds -o $@ *.o libgcc.a -static
#	$(LD) -T miniboot.lds -Bstatic -Ttext 0xE6300000 -o $@ *.o
	
clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~ *.log
	
	
