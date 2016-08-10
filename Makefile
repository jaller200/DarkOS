# GCC and LD cross-compiler / linker
LD=/usr/local/gcc-4.8.1-for-linux64/bin/x86_64-pc-linux-ld
CC=/usr/local/gcc-4.8.1-for-linux64/bin/x86_64-pc-linux-gcc

# Our compiler and linker flags
CFLAGS = -m32 -ffreestanding
LFLAGS = -Ttext 0x100000 -m elf_i386 --oformat binary

# Sources
C_SOURCES = $(wildcard kernel/*.c drivers/*.c drivers/hal/*.c drivers/screen/*.c drivers/common/*.c)
HEADERS = $(wildcard drivers/*.h drivers/*.h drivers/hal/*.h drivers/screen/*.h drivers/common/*.h)

# Objects
OBJ = ${C_SOURCES:.c=.o}

# Compilation

all: os-image.img

run: all
	qemu-system-x86_64 -fda os-image.img

hdd: all
	qemu-system-x86_64 os-image.img

debug: all
	qemu-system-x86_64 -fda os-image.img -s -S

new:
	make clean
	clear
	make

bochs:
	bochs

os-image.img : boot/boot.bin boot/krnldr.bin kernel/kernel.bin
	rm -fr os-tmp/

	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$< of=$@ bs=512 conv=notrunc

	mkdir os-tmp
	hdiutil mount -mountpoint os-tmp/ $@
	cp boot/krnldr.bin os-tmp/
	cp kernel/kernel.bin os-tmp/
	hdiutil detach os-tmp/
	rm -fr os-tmp/

kernel/kernel.bin : kernel/kernel_entry.o ${OBJ}
	$(LD) $(LFLAGS) -o $@ $^

%.o : %.c ${HEADERS}
	$(CC) $(CFLAGS) -o $@ -c $<

%.o : %.s
	nasm -f elf -o $@ $<

%.bin : %.s
	nasm -f bin -o $@ $<

clean:
	rm -fr boot/*.bin *.img kernel/*.bin kernel/*.o
	rm -fr drivers/*.o
	rm -fr drivers/screen/*.o drivers/common/*.o drivers/hal/*.o