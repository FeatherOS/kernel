# kernel
This is the Kernel from FeatherOS.<br><br>
![](https://img.shields.io/github/issues/FeatherOS/kernel)
![](https://img.shields.io/github/issues-pr/FeatherOS/kernel)
![](https://github.com/FeatherOS/kernel/actions/workflows/c-cpp.yml/badge.svg)

<!--# Building
## Dependencies
**Install the following Packages**
### Debian Based Distro
- gcc
- nasm
## Cross Compiler (recommended)
**Build your own Cross Compiler (recommended)**<br>
Run:
```
make toolchain
```-->
# Todo
- [ ] x86_64
    - [ ] System
        - [ ] GDT (Global Descriptor Table)
        - [ ] IDT (Interrupt Descriptor Table)
        - [ ] TSS (Task State Segment)
        - [ ] PCI (Peripheral Component Interconnect)
        - [ ] PCIe (Peripheral Component Interconnect Express)
        - [ ] MSI (Message Signaled Interrupts)
        - [ ] MSI-X (Message Signaled Interrupts)
    - [ ] Memory
        - [ ] PMM (Physical Memory Manager)
        - [ ] VMM (Virtual Memory Manager)
        - [ ] Heap
    - [ ] ACPI (Advanced Configuration and Power Interface)
        - [ ] LAPIC (Local Advanced Programmable Interrupt Controller)
        - [ ] IOAPIC (I/O Advanced Programmable Interrupt Controller)
        - [ ] Shutdown
        - [ ] Reboot
    - [ ] Userspace
        - [ ] System Calls
        - [ ] ELF (Executable and Linking Format)
    - [ ] Partition Tables
        - [ ] MBR
        - [ ] GPT
    - [ ] Filesystems
        - [ ] NTFS
        - [ ] Ext2
    - [ ] Network Stack
        - [ ] Ethernet
        - [ ] TCP
        - [ ] UDP
        - [ ] ARP
        - [ ] DHCP
        - [ ] IPv4
        - [ ] IPv4 fragmentation support