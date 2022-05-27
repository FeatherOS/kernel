#ARCHITECTURE: x86_64
ARCHITECTURE = x86_64
#FIRMWARE: bios, uefi
FIRMWARE = bios

G++ = g++
G++_FLAGS = -ffreestanding -Wall -Wextra -pedantic -Wmissing-declarations
LD = ld
LD_FLAGS = -n -T src/architectures/$(ARCHITECTURE)/linker.ld -nostdlib

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

all_cpp_files = $(call rwildcard,src/architectures/$(ARCHITECTURE)/kernel,*.cpp)
other_cpp_files = $(filter-out src/architectures/$(ARCHITECTURE)/kernel/Main.cpp,$(all_cpp_files))
other_cpp_output_files = $(other_cpp_files:.cpp=.o)

all_object_files = $(call rwildcard,build/temp,*-$(ARCHITECTURE).o)
boot_object_files = build/temp/header-$(ARCHITECTURE).o build/temp/boot-$(ARCHITECTURE).o build/temp/kernel-$(ARCHITECTURE).o
other_object_files = $(filter-out $(boot_object_files),$(all_object_files))

all: clean build.cpp build.kernel build.iso

build.cpp: $(other_cpp_output_files)

$(other_cpp_output_files): %.o : %.cpp
	@echo "Compiling $<..."
	$(G++) $(G++_FLAGS) -c $< -o build/temp/kernel/$(*F)-$(ARCHITECTURE).o
	@echo "$< compiled successfully."

build.kernel:
	@echo "Building Kernel..."
ifeq ($(ARCHITECTURE),x86_64)
	nasm -f elf64 src/architectures/$(ARCHITECTURE)/boot/boot.asm -o build/temp/boot-$(ARCHITECTURE).o
	nasm -f elf64 src/architectures/$(ARCHITECTURE)/boot/header.asm -o build/temp/header-$(ARCHITECTURE).o
endif
ifneq ($(ARCHITECTURE),x86_64)
	nasm -f elf32 src/architectures/$(ARCHITECTURE)/boot/boot.asm -o build/temp/boot-$(ARCHITECTURE).o
	nasm -f elf32 src/architectures/$(ARCHITECTURE)/boot/header.asm -o build/temp/header-$(ARCHITECTURE).o
endif
	$(G++) $(G++_FLAGS) -c src/architectures/$(ARCHITECTURE)/kernel/Main.cpp -o build/temp/kernel-$(ARCHITECTURE).o
	$(LD) $(LD_FLAGS) build/temp/header-$(ARCHITECTURE).o build/temp/boot-$(ARCHITECTURE).o build/temp/kernel-$(ARCHITECTURE).o $(other_object_files) -o build/Kernel-$(ARCHITECTURE).bin
	@echo "Kernel build successfully."

build.iso:
	@echo "Building Iso..."
	@cp build/Kernel-$(ARCHITECTURE).bin build/iso/boot/kernel.bin
	@grub-mkrescue build/iso -o build/FeatherOS-$(ARCHITECTURE).iso
	@echo "Iso build successfully."

clean:
	@rm -rf build/temp/
	@mkdir -p build/temp/kernel