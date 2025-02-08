#!/bin/bash

# Create directories
rm -rf isodir
mkdir -p isodir/boot/grub

# Copy kernel binary
cp kernel.bin isodir/boot/

# Create GRUB2 config
cat > isodir/boot/grub/grub.cfg << EOF
set timeout=0
set default=0

menuentry "AsfaleiaOS" {
    multiboot /boot/kernel.bin
    boot
}
EOF

# Create ISO image
grub2-mkrescue -o AsfaleiaOS.iso isodir/