#!/bin/bash
i586-elf-as ./bootstrap.s -o ../link/boot.o
i586-elf-gcc -c ./kmain.c -o ../link/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
cd ../link/
i586-elf-gcc -T linker.ld -o GameOS.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
cd ..
rm isodir/boot/GameOS.bin
cp ./link/GameOS.bin ./isodir/boot/GameOS.bin
rm GameOS.iso
grub-mkrescue -o GameOS.iso isodir
