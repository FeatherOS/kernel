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
    .boot : {
        KEEP(*(.multiboot_header))
    }
    .text : {*(.text)}
    .data : {*(.data)}
    .bss  : {*(.bss)}
    . = ALIGN(4096);
    kernel_end = .;
}