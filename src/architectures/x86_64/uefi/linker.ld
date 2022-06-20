ENTRY(start)

/*SECTIONS {
    . = 1M;

    .boot :
    {
        KEEP(*(.multiboot_header))
    }
    .text :
    {
        *(.text)
    }
}*/
SECTIONS
{
    . = 1M;
    kernel_start = .;
    .text : ALIGN(0x1000) {*(.text)}
    .data : ALIGN(0x1000) {*(.data)}
    .bss  : ALIGN(0x1000) {*(.bss)}
    . = ALIGN(4096);
    kernel_end = .;
}