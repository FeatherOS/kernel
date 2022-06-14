global start

section .text
extern kernel_main
start:
    mov esp, stack_top
    push ebx
    push eax
    call kernel_main
    cli
hang:
    hlt
    jmp hang

section .bss
align 16
stack_bottom:
    resb 4096 * 4
stack_top: