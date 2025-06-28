CC = gcc
NASM = nasm
LD = ld

CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra
LDFLAGS = -m elf_i386

# Path espliciti e completi
KERNEL_SOURCES = $(wildcard kernel/*.c)
DRIVER_SOURCES = $(wildcard drivers/*.c)
APPLICATION_SOURCES = $(wildcard application/*.c)
FILESYSTEM_SOURCES = $(wildcard filesystem/*.c)  
LIBRARY_SOURCES = $(wildcard lib/*.c)  
SOURCES = $(KERNEL_SOURCES) $(DRIVER_SOURCES) $(APPLICATION_SOURCES) $(FILESYSTEM_SOURCES) $(LIBRARY_SOURCES)

# Oggetti
KERNEL_OBJECTS = $(KERNEL_SOURCES:.c=.o)
DRIVER_OBJECTS = $(DRIVER_SOURCES:.c=.o)
APPLICATION_OBJECTS = $(APPLICATION_SOURCES:.c=.o)
FILESYSTEM_OBJECTS = $(FILESYSTEM_SOURCES:.c=.o)
LIBRARY_OBJECTS = $(LIBRARY_SOURCES:.c=.o)
OBJECTS = $(KERNEL_OBJECTS) $(DRIVER_OBJECTS) $(APPLICATION_OBJECTS) $(FILESYSTEM_OBJECTS) $(LIBRARY_OBJECTS)

# Bootloader
BOOT = boot/bootloader.asm

# Obiettivo principale
all: kernel.bin

# Creazione del kernel binario
kernel.bin: $(OBJECTS) boot.o
	$(LD) $(LDFLAGS) -T boot/boot.ld -o $@ boot.o $(OBJECTS)

# Compilazione del bootloader
boot.o: $(BOOT)
	$(NASM) -f elf32 $(BOOT) -o boot.o

# Compilazione dei file .c in .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Pulizia
clean:
	rm -f boot.o $(OBJECTS) kernel.bin

# Creazione dell'immagine ISO
iso: kernel.bin
	chmod +x build_iso.sh
	./build_iso.sh

# Test QEMU
qemu:
	qemu-system-i386 -kernel kernel.bin

help:
	@echo "Usage: make [all|clean|iso|qemu|help]"
	@echo "  all:      Build the kernel"
	@echo "  clean:    Remove all generated files"
	@echo "  iso:      Create the ISO image"
	@echo "  qemu:     Run the kernel in QEMU"
	@echo "  help:     Show this help message"