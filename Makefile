#ARCHITECTURE: x86_64
ARCHITECTURE = x86_64
#FIRMWARE: bios, uefi
FIRMWARE = bios

GXX = g++
GXX_FLAGS = -ffreestanding -Wall -Wextra -pedantic -Wmissing-declarations -I src/includes/ -MMD -MP
LD = ld
LD_FLAGS = -n -T src/architectures/$(ARCHITECTURE)/linker.ld -nostdlib

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

ALL_CPP_FILES = $(call rwildcard,src/architectures/$(ARCHITECTURE)/kernel,*.cpp)

CPP_OUTPUT_FILES = $(ALL_CPP_FILES:.cpp=.o)
OTHER_CPP_OUTPUT_FILES = $(filter-out src/architectures/$(ARCHITECTURE)/kernel/Main.o,$(CPP_OUTPUT_FILES))
CPP_DEPENDENCY_FILES = $(CPP_OUTPUT_FILES:.o=.d)

#all_object_files = $(call rwildcard,build/temp,*-$(ARCHITECTURE).o)
#boot_object_files = build/temp/header-$(ARCHITECTURE).o build/temp/boot-$(ARCHITECTURE).o build/temp/kernel-$(ARCHITECTURE).o
#other_object_files = $(filter-out $(boot_object_files),$(all_object_files))

all: build.kernel
ARCHITECTURES_FOLDER := src/architectures/
$(CPP_DEPENDENCY_FILES): %.d : %.cpp
	-cp -Rfp build/temp/$(subst $(ARCHITECTURES_FOLDER),,$(*D))/$(*F).o $(*D)/$(*F).o
	-cp -Rfp build/temp/$(subst $(ARCHITECTURES_FOLDER),,$(*D))/$(*F).d $@
$(CPP_OUTPUT_FILES): %.o : %.cpp
	@echo "Compiling $<..."
	$(GXX) $(GXX_FLAGS) -c $< -o $@
	@mkdir -p build/temp/$(subst $(ARCHITECTURES_FOLDER),,$(*D))/
	-cp -Rfp $@ build/temp/$(subst $(ARCHITECTURES_FOLDER),,$(*D))/$(*F).o
	-cp -Rfp $(*D)/$(*F).d build/temp/$(subst $(ARCHITECTURES_FOLDER),,$(*D))/$(*F).d
	@echo "$< compiled successfully."

build.kernel: $(CPP_OUTPUT_FILES)
	@echo "Building Kernel..."
	@mkdir -p build/temp/$(ARCHITECTURE)/
ifeq ($(ARCHITECTURE),x86_64)
	nasm -f elf64 src/architectures/$(ARCHITECTURE)/boot/boot.asm -o build/temp/$(ARCHITECTURE)/boot.o
	nasm -f elf64 src/architectures/$(ARCHITECTURE)/boot/header.asm -o build/temp/$(ARCHITECTURE)/header.o
else
	nasm -f elf32 src/architectures/$(ARCHITECTURE)/boot/boot.asm -o build/temp/$(ARCHITECTURE)/boot.o
	nasm -f elf32 src/architectures/$(ARCHITECTURE)/boot/header.asm -o build/temp/$(ARCHITECTURE)/header.o
endif
	$(LD) $(LD_FLAGS) build/temp/$(ARCHITECTURE)/header.o build/temp/$(ARCHITECTURE)/boot.o src/architectures/$(ARCHITECTURE)/kernel/Main.o $(OTHER_CPP_OUTPUT_FILES) -o build/Kernel-$(ARCHITECTURE).bin
	@find src/architectures/$(ARCHITECTURE) -iname '*.o' -delete
	@find src/architectures/$(ARCHITECTURE) -iname '*.d' -delete
	@echo "Kernel build successfully."

build.iso: build.kernel
	@echo "Building Iso..."
	@cp build/Kernel-$(ARCHITECTURE).bin build/iso/boot/kernel.bin
	@grub-mkrescue build/iso -o build/FeatherOS-$(ARCHITECTURE).iso
	@echo "Iso build successfully."

clean:
	@rm -rf build/temp/
-include $(CPP_DEPENDENCY_FILES)