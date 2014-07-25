.phony: all

all: miniboot.bin

CC  = arm-none-linux-gnueabi-gcc
LD  = arm-none-linux-gnueabi-ld
COPY= arm-none-linux-gnueabi-objcopy
DUMP= arm-none-linux-gnueabi-objdump

miniboot.bin: miniboot.elf
	$(COPY) -O binary $< $@
	$(DUMP) -D $< > dump.log
	
miniboot.elf: start.S board.c uart.c spiflash.c
	$(CC) -c -mcpu=cortex-a15 -mlittle-endian -msoft-float -Wall -Os -nostdlib start.S
	$(CC) -c -mcpu=cortex-a15 -mlittle-endian -msoft-float -Wall -Os -nostdlib board.c
	$(CC) -c -mcpu=cortex-a15 -mlittle-endian -msoft-float -Wall -Os -nostdlib uart.c
	$(CC) -c -mcpu=cortex-a15 -mlittle-endian -msoft-float -Wall -Os -nostdlib spiflash.c
	$(LD) -T miniboot.lds -o $@ start.o board.o uart.o spiflash.o

#secure.elf: *.S *.c
#	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -Os -nostdlib $^
#	$(LD) -T secure.lds -o $@ monitor.o secure_startup.o secure_main.o board.o
	
clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~ *.log
	
	
