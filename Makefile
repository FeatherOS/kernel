ARCH=x86_64

# --- Files Test --- #
CPP_SOURCE_FILES := ${shell find src/$(ARCH)/kernel -name *.cpp}
# --- Files Test END --- #

all: build.kernel build.iso

build.kernel:
	@echo Building Kernel...
	nasm -f elf64 src/$(ARCH)/boot/boot.asm -o build/$(ARCH)/tmp/boot.o
	nasm -f elf64 src/$(ARCH)/boot/header.asm -o build/$(ARCH)/tmp/header.o

	gcc -ffreestanding -c src/$(ARCH)/kernel/kernel.cpp -o build/$(ARCH)/tmp/kernel.o
	gcc -ffreestanding -c src/$(ARCH)/kernel/IO.cpp -o build/$(ARCH)/tmp/io.o

	ld -n -o build/$(ARCH)/iso/boot/kernel.bin -T src/$(ARCH)/linker.ld build/$(ARCH)/tmp/header.o build/$(ARCH)/tmp/boot.o build/$(ARCH)/tmp/kernel.o build/$(ARCH)/tmp/io.o
	@echo Done.
build.iso:
	@echo Building Iso...
	grub-mkrescue build/$(ARCH)/iso -o build/$(ARCH)/FeatherOS.iso
	@echo Done.