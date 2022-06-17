#ARCHITECTURE: x86_64
ARCHITECTURE = x86
#FIRMWARE: bios, uefi
FIRMWARE = bios

#CXX = i386-elf-g++
CXX = g++
CXXFLAGS = -std=c++17 -ffreestanding -Wall -Wextra -pedantic -Wmissing-declarations -I src/includes/ -MMD -MP
#LD = i386-elf-ld
LD = ld
LDFLAGS = -n -T src/architectures/$(ARCHITECTURE)/linker.ld -nostdlib

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

ALL_ASSEMBLY_FILES = $(call rwildcard,src/architectures/$(ARCHITECTURE),*.asm)
OTHER_ASSEMBLY_FILES = $(call rwildcard,src/architectures/$(ARCHITECTURE)/kernel,*.S)

ALL_CPP_FILES = $(call rwildcard,src/architectures/$(ARCHITECTURE)/kernel,*.cpp)
ALL_CPP_OUTPUT_FILES = $(ALL_CPP_FILES:.cpp=.o)
OTHER_CPP_OUTPUT_FILES = $(filter-out src/architectures/$(ARCHITECTURE)/kernel/Main.o,$(ALL_CPP_OUTPUT_FILES))
ALL_CPP_DEPENDENCY_FILES := $(ALL_CPP_OUTPUT_FILES:.o=.d)

ALL_OUTPUT_FILES = src/architectures/$(ARCHITECTURE)/boot/header.o src/architectures/$(ARCHITECTURE)/boot/boot.o $(OTHER_ASSEMBLY_FILES:.S=.o) $(ALL_CPP_FILES:.cpp=.o)
OUTPUT_FILE = build/Kernel-$(ARCHITECTURE).bin

ifeq ($(ARCHITECTURE),x86)
	CXXFLAGS += -m32
	LDFLAGS += -melf_i386
endif

build.kernel: $(OUTPUT_FILE)

$(OUTPUT_FILE): --copy $(ALL_OUTPUT_FILES)
	@echo "Building Kernel..."
	$(LD) $(LDFLAGS) src/architectures/$(ARCHITECTURE)/boot/header.o src/architectures/$(ARCHITECTURE)/boot/boot.o src/architectures/$(ARCHITECTURE)/kernel/Main.o $(OTHER_CPP_OUTPUT_FILES) $(OTHER_ASSEMBLY_FILES:.S=.o) -o build/Kernel-$(ARCHITECTURE).bin
	@find src/architectures/$(ARCHITECTURE) -iname '*.o' -delete
	@find src/architectures/$(ARCHITECTURE) -iname '*.d' -delete
	@echo "Kernel build successfully."

--copy:
	@find src/architectures/$(ARCHITECTURE) -iname '*.o' -delete
	@find src/architectures/$(ARCHITECTURE) -iname '*.d' -delete
	-cp -Rrfp build/temp/$(ARCHITECTURE)/* src/architectures/$(ARCHITECTURE)/

%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@mkdir -p build/temp/$(subst src/architectures/,,$(*D))/
	-cp -Rfp $@ build/temp/$(subst src/architectures/,,$(*D))/$(*F).o
	-cp -Rfp $(*D)/$(*F).d build/temp/$(subst src/architectures/,,$(*D))/$(*F).d
	@echo "$< compiled successfully."

%.o : %.asm
	@echo "Assembling $<..."
	nasm -f elf32 $< -o $@
	@mkdir -p build/temp/$(subst src/architectures/,,$(*D))/
	-cp -Rfp $@ build/temp/$(subst src/architectures/,,$(*D))/$(*F).o
	@echo "$< assembled successfully."

%.o : %.S
	@echo "Assembling $<..."
	nasm -f elf32 $< -o $@
	@mkdir -p build/temp/$(subst src/architectures/,,$(*D))/
	-cp -Rfp $@ build/temp/$(subst src/architectures/,,$(*D))/$(*F).o
	@echo "$< assembled successfully."

toolchain:
	@echo "Building toolchain..."
	@echo "coming soon..."
	@echo "Toolchain build successfully."

build.iso: build.kernel
	@echo "Building Iso..."
	@cp build/Kernel-$(ARCHITECTURE).bin build/iso/boot/kernel.bin
	@grub-mkrescue build/iso -o build/FeatherOS-$(ARCHITECTURE).iso
	@echo "Iso build successfully."

clean:
	@rm -rf build/temp/
	@rm -f build/iso/boot/kernel.bin
	@rm -f build/Kernel*.bin

-include $(ALL_CPP_DEPENDENCY_FILES)