#ARCHITECTURE: x86_64, i386
ARCHITECTURE = i386
#FIRMWARE: bios, uefi
FIRMWARE = bios

CXX = g++
CXXFLAGS = -std=c++17 -ffreestanding -Wall -Wextra -pedantic -Wmissing-declarations -I src/includes/ -MMD -MP
LD = ld
LDFLAGS = -n -T src/architectures/$(ARCHITECTURE)/$(FIRMWARE)/linker.ld -nostdlib

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

ALL_ASSEMBLY_FILES = $(call rwildcard,src/architectures/$(ARCHITECTURE)/$(FIRMWARE),*.asm)
ALL_STUB_FILES = $(call rwildcard,src/architectures/$(ARCHITECTURE)/$(FIRMWARE),*.S)

ALL_CPP_FILES = $(call rwildcard,src/architectures/$(ARCHITECTURE)/$(FIRMWARE),*.cpp)
ALL_CPP_OUTPUT_FILES = $(ALL_CPP_FILES:.cpp=.o)
ALL_CPP_DEPENDENCY_FILES := $(ALL_CPP_OUTPUT_FILES:.o=.d)

ALL_OUTPUT_FILES = $(ALL_ASSEMBLY_FILES:.asm=.o) $(ALL_STUB_FILES:.S=.o) $(ALL_CPP_FILES:.cpp=.o)
OUTPUT_FILE = Kernel-$(ARCHITECTURE)-$(FIRMWARE).bin

ifeq ($(ARCHITECTURE),i386)
	CXXFLAGS += -m32
	LDFLAGS += -m elf_i386
endif

build.kernel: $(OUTPUT_FILE)

$(OUTPUT_FILE): --copy $(ALL_OUTPUT_FILES)
	@echo "Building Kernel..."
	$(LD) $(LDFLAGS) $(ALL_CPP_FILES:.cpp=.o) $(ALL_ASSEMBLY_FILES:.asm=.o) $(ALL_STUB_FILES:.S=.o) -o build/Kernel-$(ARCHITECTURE)-$(FIRMWARE).bin
	@find src/architectures/$(ARCHITECTURE)/$(FIRMWARE) -iname '*.o' -delete
	@find src/architectures/$(ARCHITECTURE)/$(FIRMWARE) -iname '*.d' -delete
	@echo "Kernel build successfully."

--copy:
	@find src/architectures/$(ARCHITECTURE)/$(FIRMWARE) -iname '*.o' -delete
	@find src/architectures/$(ARCHITECTURE)/$(FIRMWARE) -iname '*.d' -delete
	-cp -Rrfp build/temp/$(ARCHITECTURE)/$(FIRMWARE)/* src/architectures/$(ARCHITECTURE)/$(FIRMWARE)/

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
ifeq ($(ARCHITECTURE), ...)
	@echo "Building toolchain..."
	@echo "coming soon..."
	@echo "Toolchain build successfully."
else
	@echo "You don't need a Custom Toolchain!"
endif

build.iso: build.kernel
ifeq ($(FIRMWARE), uefi)
	@echo "Building Iso..."
	dd if=/dev/zero of=build/FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).img bs=512 count=93750
	mformat -i build/FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).img -f 1440 ::
	mmd -i build/FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).img ::/EFI
	mmd -i build/FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).img ::/EFI/BOOT
	mcopy -i build/FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).img ../Bootloader/uefi/x86_64/bootloader/main.efi ::EFI/BOOT
	mcopy -i build/FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).img build/startup.nsh ::
	mcopy -i build/FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).img build/Kernel-$(ARCHITECTURE)-$(FIRMWARE).bin ::
	mv build/FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).img build/FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).iso
	@echo "Iso build successfully."
	@echo "qemu-system-$(ARCHITECTURE) -cdrom FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).iso -drive if=pflash,format=raw,unit=0,file=OVMF_CODE-pure-efi.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=OVMF_VARS-pure-efi.fd" > build/run-$(ARCHITECTURE)-$(FIRMWARE).bat
	@echo "qemu-system-$(ARCHITECTURE) -cdrom FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).iso -drive if=pflash,format=raw,unit=0,file=OVMF_CODE-pure-efi.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=OVMF_VARS-pure-efi.fd" > build/run-$(ARCHITECTURE)-$(FIRMWARE).sh
	@echo "created start files."
else
	@echo "Building Iso..."
	@cp build/Kernel-$(ARCHITECTURE)-$(FIRMWARE).bin build/iso/boot/kernel.bin
	@grub-mkrescue build/iso -o build/FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).iso
	@echo "Iso build successfully."
	@echo "qemu-system-$(ARCHITECTURE) -cdrom FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).iso" > build/run-$(ARCHITECTURE)-$(FIRMWARE).bat
	@echo "qemu-system-$(ARCHITECTURE) -cdrom FeatherOS-$(ARCHITECTURE)-$(FIRMWARE).iso" > build/run-$(ARCHITECTURE)-$(FIRMWARE).sh
	@echo "created start files."
endif

clean:
	@rm -rf build/temp/
	@rm -f build/iso/boot/kernel.bin
	@rm -f build/Kernel*.bin

-include $(ALL_CPP_DEPENDENCY_FILES)