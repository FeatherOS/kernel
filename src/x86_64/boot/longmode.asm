extern main
start_longmode:
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
	call main
	cli
hang:
    hlt
    jmp hang
check_longmode:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb no_longmode
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz no_longmode
    ret
no_longmode:
    mov al, "N";"no longmode available"
    jmp error